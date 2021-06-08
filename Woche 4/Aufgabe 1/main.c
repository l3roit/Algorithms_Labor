//######################################################################################################################
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM 256

//----------------------------------------------------------------------------------------------------------------------
typedef struct lin_list                                                                                                 //I take a couple of names from older code
{
    char            *content;                                                                                           //content of cell, in this case some letters
    struct lin_list *after;                                                                                             //pointer on next list element
}LinListCell, *LinList_p;
//----------------------------------------------------------------------------------------------------------------------
LinList_p LinListMakeNewCell(char* content);
LinList_p LinListNewFirstCell(LinList_p *anchor, LinList_p newcell);
LinList_p LinListGetFirstCell(LinList_p *anchor);

LinList_p LinListSortByMergeSort(LinList_p *list);

void LinListSplitListUp(LinList_p list, LinList_p *l1, LinList_p *l2);
LinList_p LinListRevertList(LinList_p *anchor);

void LinListPrintList(FILE* out, LinList_p list);
void LinListKill(LinList_p junk);
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    FILE *input = stdin;                                                                                                //input gets set on stdin, that's the terminal input

    char line[MAXIMUM];
    LinList_p firstList = NULL, newCell;
    char* inputString;

    if(argc > 2)                                                                                                        //More arguemtns then 2 does not work -> error
    {
        fprintf(stderr, "Error\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(argc == 2)                                                                                                       //2 Arguments does work
    {
        input = fopen(argv[1], "r");                                                                     //input gets read
        if(!input)                                                                                                      //if file input is not available, then error too
        {
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }
    }                                                                                                                   //fgets delivers a pointer to the string buffer if successfull, if not it returns NULL -> while does not work
    printf("\nYour input:\n\n");
    while((inputString = fgets(line, MAXIMUM, input)))                                                                  //but if file input is available -> as long as there is a new line available
    {                                                                                                                   //fgets(x, y, z) means: x is where it gets stored, y is Maximum numbers of chars that can be read, z is where it's read from
        newCell = LinListMakeNewCell(line);                                                                             //make a new cell with the content read from the file
        LinListNewFirstCell(&firstList, newCell);                                                                       //Add the new created cell to the existing list as first list element
    }
    printf("\nYour output is:\n\n");

    LinListSortByMergeSort(&firstList);
    LinListPrintList(stdout, firstList);                                                                                //print List in Command line

    LinListKill(firstList);                                                                                             //Killing (free) list and contents

    fclose(input);                                                                                                      //close file

    return(0);
}
//----------------------------------------------------------------------------------------------------------------------
//Function for making a cell of the linear list with the given content
LinList_p LinListMakeNewCell(char* content)
{
    LinList_p cell = malloc(sizeof(LinListCell));                                                                       //Tries to allocate new cell if there is enough memory
    if(cell == NULL)                                                                                                    //If not -> error
    {
        fprintf(stderr, "it's out of memory\n");
        exit(EXIT_FAILURE);
    }

    void *tmp = strdup(content);                                                                                        //It tries to allocate enough memory to hold the old string (plus a '\0' character to mark the end of the string) -> Info from stackoverflow :)
    if(tmp == NULL)                                                                                                     //If not possible -> error
    {
        fprintf(stderr, "it's out of memory\n");
        exit(EXIT_FAILURE);
    }
    cell->content = tmp;                                                                                                //fill content of new cell
    cell->after    = NULL;                                                                                              //At this time there is no next cell -> cell->after = NULL
    return cell;
}
//----------------------------------------------------------------------------------------------------------------------
//Function for new cell at first place
LinList_p LinListNewFirstCell(LinList_p *anchor, LinList_p newFirstCell)
{
    newFirstCell->after = *anchor;                                                                                      //pointer from new cell gets set on the old fist cell
    *anchor = newFirstCell;                                                                                             //ancor gets set on new first cell
    return newFirstCell;
}
//----------------------------------------------------------------------------------------------------------------------
//Function for pulling out First cell from list
LinList_p LinListGetFirstCell(LinList_p *anchor)
{
    LinList_p singleElement = *anchor;                                                                                  //anchor has to be saved for this function
    if(singleElement)
    {
        *anchor = singleElement->after;                                                                                 //anchor gets set on second list element
        singleElement->after = NULL;                                                                                    //first list element gets cut out by not pointing on next element but pointing on NULL
    }
    return singleElement;                                                                                               //Single element gets returned
}
//----------------------------------------------------------------------------------------------------------------------
//Sorts a list by splitting it up in as tiny as possible lists and orders this tniy list back together to one bg list: Theory at: https://de.wikipedia.org/wiki/Mergesort
//Using Merge sort, because we did quicksort and bubblesort last semester and i wanted to try new things
//Thanks for the Divide and Quonquer Theorie lesson, it really helped me to understand the merge sort
//But i got some inspiration ( ;) ) from the following links
//https://hackr.io/blog/merge-sort-in-c
//https://www.geeksforgeeks.org/merge-sort/
//https://www.happycoders.eu/de/algorithmen/mergesort/
//https://www.thecrazyprogrammer.com/2014/03/c-program-for-implementation-of-merge-sort.html
LinList_p LinListSortByMergeSort(LinList_p *inputList)
{
    LinList_p list1 = NULL, list2 = NULL, tmp;
    if(!*inputList ||  !(*inputList)->after)                                                                            //if there is no list or there is just one element -> return
    {
        return *inputList;
    }
    LinListSplitListUp(*inputList, &list1, &list2);                                                                     //Split List in 2 lists ->Divide and Quonquer
    *inputList = NULL;
    LinListSortByMergeSort(&list1);                                                                                     //Recursion with first half list
    LinListSortByMergeSort(&list2);                                                                                     //Recursion with second half list

    while(list1 || list2)                                                                                               //while there are list1 and list2
    {
        if(!list2)                                                                                                      //if there is no second list -> the case if list1 has one element and list2 none
        {
            tmp = LinListGetFirstCell(&list1);                                                                          //get the last element from list1
        }
        else if(!list1)                                                                                                 //same thing with list2
        {
            tmp = LinListGetFirstCell(&list2);
        }
        else if(strcmp(list1->content, list2->content)<0)                                                               //whats more "ASCII-betisch" ;)
        {
            tmp = LinListGetFirstCell(&list1);
        }
        else
        {
            tmp = LinListGetFirstCell(&list2);
        }
        LinListNewFirstCell(inputList, tmp);
    }
    LinListRevertList(inputList);                                                                                       //cause the most ASCII-betisch value gets written first in the new array -> gets pushed to the end, i have to revert it, so the most ASACII-betisch is at the beginning
    return *inputList;
}
//----------------------------------------------------------------------------------------------------------------------
//Function for Splitting List Up into 2 lists -> for merge sort necessary
void LinListSplitListUp(LinList_p list, LinList_p *listOne, LinList_p *listTwo)
{
    LinList_p tmp;
    *listOne = NULL;
    *listTwo = NULL;

    while(list)                                                                                                         //as long as there is a list
    {
        tmp = LinListGetFirstCell(&list);                                                                               //save first cell intp tmp var
        LinListNewFirstCell(listOne, tmp);                                                                              //put the first cell from alternate list into new list 1
        if(!list)                                                                                                       //If this was the last element -> stop
        {
            break;
        }
        tmp = LinListGetFirstCell(&list);                                                                               //next element saved in tmp var
        LinListNewFirstCell(listTwo, tmp);                                                                              //second elementfrom alternative list gets put into listTwo
    }
}                                                                                                                       //this happens over and over again, till its no longer possible to split the input list up
//----------------------------------------------------------------------------------------------------------------------
//Function for reversing the already existing list
LinList_p LinListRevertList(LinList_p *anchor)
{
    LinList_p revertedList=NULL, tmpCell;                                                                               //new reverted List and a temporary cell for getting the first cell of old list and the put it into the new list

    while(*anchor)                                                                                                      //while there is an anchor
    {
        tmpCell = LinListGetFirstCell(anchor);                                                                          //temporaryCell is the first cell from the normal list
        LinListNewFirstCell(&revertedList, tmpCell);                                                                    //Put temporaryCell into the new reverted List--------- Step 1: revertedList[x], Step 2: revertedList[firstOld], Step 3: revertedList[secondOld, firstOld], ...........
    }
    *anchor =revertedList;                                                                                              //set the anchor to the new list which is reverted
    return revertedList;                                                                                                //return the reverted list
}
//----------------------------------------------------------------------------------------------------------------------
//Function for printing the linear list
void LinListPrintList(FILE* printer, LinList_p printList)
{
    while(printList)                                                                                                    //as long as there is something to print
    {
        fprintf(printer, "%s", printList->content);                                                              //print it at stdout, as string an print the content of the cell
        printList = printList->after;                                                                                   //the new cell to print is the the cell after the cell before
    }
}
//----------------------------------------------------------------------------------------------------------------------
//Function for killing
void LinListKill(LinList_p trash)
{
    LinList_p after;
    while(trash)                                                                                                        //While there is trash to remove
    {
        after = trash->after;                                                                                           //next trash element is the next after the mom element -> has to be saved, because i am removing the cell and then want to jump to the next one
        free(trash);                                                                                                    //kill the whole trash
        trash = after;                                                                                                  //jump on the next trash
    }
}
//######################################################################################################################