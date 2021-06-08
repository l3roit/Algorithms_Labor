/*-----------------------------------------------------------------------
  
  File  : mystat_u.c
  
Author: Stephan Schulz (schulz@eprover.org)
  
Contents

  Library implementing singly linked lists (of strings), and a test
  program that uses it to implement the functionality of sort | uniq
  -c (give a sorted list of unique lines preceeded by a frequency
  counter. 
  
Copyright 2015 by the author.
This code is released under the GNU General Public Licence, version
  2, or, at your choice, any later version.
  
Changes

<1> Tue Jun  2 17:36:03 CEST 2015
    Started with code from Week 3 ;-)
    
-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE  256


/* Data type: Linear list cell */

typedef struct lin_list
{
   char            *payload; /* User data (in this case just strings) */
   int             counter;
   struct lin_list *next;    /* Pointer to rest of list (or NULL) */ 
}LinListCell, *LinList_p;

/*-----------------------------------------------------------------------
//
// Function: xmalloc()
//
//   Thin wrapper around malloc() - fail noisily (with error message)
//   if no memory is available.
//
// Global Variables: -
//
// Side Effects    : Only via malloc() or in case of error
//
/----------------------------------------------------------------------*/

void* xmalloc(size_t size)
{
   void *mem = malloc(size);
   if(!mem)
   {
      fprintf(stderr, "lin_list: Out of memory\n");
      exit(EXIT_FAILURE);
   }
   return mem;
}

/*-----------------------------------------------------------------------
//
// Function: xstrdup()
//
//   Thin wrapper around strdup() - fail noisily (with error message)
//   if no memory is available.
//
// Global Variables: -
//
// Side Effects    : Only via malloc() or in case of error
//
/----------------------------------------------------------------------*/

char* xstrdup(char* str)
{
   void *newstr = strdup(str);
   if(!newstr)
   {
      fprintf(stderr, "lin_list: Out of memory\n");
      exit(EXIT_FAILURE);
   }
   return newstr;
}


/*-----------------------------------------------------------------------
//
// Function: LinListAllocCell()
//
//   Allocate a Linear List Cell initialised with the given payload
//   string. The string is copied!
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

LinList_p LinListAllocCell(char* payload)
{
   LinList_p cell = xmalloc(sizeof(LinListCell));
   
   cell->payload = xstrdup(payload);
   cell->counter = 1;
   cell->next    = NULL;

   return cell;
}

/*-----------------------------------------------------------------------
//
// Function: LinListFreeCell()
//
//   Free a linear list cell, including payload, but not possible
//   sublists. 
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

void LinListFreeCell(LinList_p junk)
{
   free(junk->payload);
   free(junk);
}

/*-----------------------------------------------------------------------
//
// Function: LinListFree()
// 
//   Free a linear list.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

void LinListFree(LinList_p junk)
{
   LinList_p next;

   while(junk)
   {
      next = junk->next;
      LinListFreeCell(junk);
      junk = next;
   }
}

/*-----------------------------------------------------------------------
//
// Function: LinListInsertFirst()
//
//   Insert a new cell as the first cell of the existing list (which
//   may be empty). Returns new list.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

LinList_p LinListInsertFirst(LinList_p *anchor, LinList_p newcell)
{
   newcell->next = *anchor;
   *anchor = newcell;

   return newcell;
}

/*-----------------------------------------------------------------------
//
// Function: LinListExtractFirst()
//
//   Extract and return the first cell of a linear list. If the list
//   is empty, return NULL.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

LinList_p LinListExtractFirst(LinList_p *anchor)
{
   LinList_p res = *anchor;
   
   if(res)
   {
      *anchor = res->next;
      res->next = NULL;
   }
   return res;
}

/*-----------------------------------------------------------------------
//
// Function: LinListRevert()
//
//   Revert the list at *anchor in place.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

LinList_p LinListRevert(LinList_p *anchor)
{
   LinList_p l=NULL, handle;

   while(*anchor)
   {
      handle = LinListExtractFirst(anchor);
      LinListInsertFirst(&l, handle);
   }
   *anchor =l;
   return l;
}

/*-----------------------------------------------------------------------
//
// Function: LinListPrint()
//
//   Print the list in order.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void LinListPrint(FILE* out, LinList_p list)
{
   while(list)
   {
      fprintf(out, "%4d %s", list->counter, list->payload);
      list = list->next;
   }
}


/*-----------------------------------------------------------------------
//
// Function: LinListSplit()
//
//   Split a list into two approximate equal sublists.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

void LinListSplit(LinList_p list, LinList_p *l1, LinList_p *l2)
{
   LinList_p handle;
   *l1 = NULL;
   *l2 = NULL;
   
   while(list)
   {
      handle = LinListExtractFirst(&list);
      LinListInsertFirst(l1, handle);
      if(!list)
      {
         break;
      }
      handle = LinListExtractFirst(&list);
      LinListInsertFirst(l2, handle);
   }   
}


/*-----------------------------------------------------------------------
//
// Function: LinListMergeSort()
//
//   Sort a list based on its payload string. Uses MergeSort.
//
// Global Variables: -
//
// Side Effects    : Destroys the original list.
//
/----------------------------------------------------------------------*/

LinList_p LinListMergeSort(LinList_p *list)
{
   LinList_p l1 = NULL, l2 = NULL, handle;
   
   if(!*list ||  !(*list)->next)
   {
      return *list;
   }
   LinListSplit(*list, &l1, &l2);
   *list = NULL;

   LinListMergeSort(&l1);
   LinListMergeSort(&l2);
   
   while(l1 || l2)
   {
      if(!l2)
      {
         handle = LinListExtractFirst(&l1);
      }
      else if(!l1)
      {
         handle = LinListExtractFirst(&l2);
      }
      else if(strcmp(l1->payload, l2->payload)<0)
      {
         handle = LinListExtractFirst(&l1);
      }
      else
      {
         handle = LinListExtractFirst(&l2);
      }
      LinListInsertFirst(list, handle);
   }
   LinListRevert(list);
   return *list;
}

/*-----------------------------------------------------------------------
//
// Function: LinListFind()
//
//   Given a list and a string, return the first cell that contains
//   the string (or NULL).
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

LinList_p LinListFind(LinList_p list, char *payload)
{
   while(list)
   {
      if(strcmp(list->payload, payload)==0)
      {
         return list;
      }
      list = list->next;
   }
   return NULL;
}



/*-----------------------------------------------------------------------
//
// Function: main()
//
//   
//
// Global Variables: 
//
// Side Effects    : 
//
/----------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
   FILE *in = stdin;
   char line[MAXLINE];
   LinList_p l1 = NULL, handle;
   char* inp; 
   
   if(argc > 2)
   {
      fprintf(stderr, "Usage: %s [<file>]\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   if(argc == 2)
   {
      in = fopen(argv[1], "r");
      if(!in)
      {
         perror(argv[0]);
         exit(EXIT_FAILURE);
      }
   }
   while((inp = fgets(line, MAXLINE, in)))
   {
      if((handle = LinListFind(l1, line)))
      {
         handle->counter++;
      }
      else
      {
         handle = LinListAllocCell(line);
         LinListInsertFirst(&l1, handle);
      }
   }
   printf("Output:\n=======\n");
   LinListMergeSort(&l1);
   LinListPrint(stdout, l1);

   LinListFree(l1);

   if(in!=stdin)
   {
      fclose(in);
   }
   
   exit(EXIT_SUCCESS);
}

/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
