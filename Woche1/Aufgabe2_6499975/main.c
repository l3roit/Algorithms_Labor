#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define MAXNUM 1000
#define MAXITEMS 10000

//Jonas Scherer Matrikelnummer 6499975
//----------------------------------------------------------------------------------------------------------------------
typedef struct m_ReadInFile{
    int readInNumber;

    struct m_ReadInFile *before;
    struct m_ReadInFile *after;
}t_ReadInFile;

typedef struct{
    int readInNumber;

    struct m_ReadInFile *mom;
    struct m_ReadInFile *start;
    struct m_ReadInFile *between;
}t_field;
//----------------------------------------------------------------------------------------------------------------------
void double_num(t_field *f, char argv[]);
void func_list_add(t_field *f, int t);
void func_print_shit(t_field *f);
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    t_field field;
    t_field *f = &field;
    double_num(f, argv[1]);
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
void double_num(t_field *f, char argv[]){
    f->mom = NULL;
    f->start = NULL;
    f->between = NULL;
    FILE *readin;
    char numberinstring[255+1];
    int t, i = 0;
    //readin = fopen(argv[1], "r");
    readin = fopen("Datei.txt", "r");
    if (!readin){
        printf("We are sorry, the file you were seeking for couldn't be opened or found");
    }else{
        fgets(numberinstring, 255, readin);
        while (!feof(readin)){
            t = (atoi(numberinstring)*2);
            func_list_add(f, t);
            i++;
            if (i == MAXNUM){break;}
            fgets(numberinstring, 255, readin);
        }
        fclose(readin);
        func_print_shit(f);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void func_list_add(t_field *f, int t){
    f->mom = (t_ReadInFile *) malloc(sizeof(t_ReadInFile));
    if (f->mom == NULL){
        printf("\nThere is no more storage available at the moment, we are very sorry");
    }else {
        f->mom->readInNumber = t;
        f->mom->before = f->between;
        f->mom->after = NULL;
        if (!f->between) {
            f->start = f->mom;
        } else {
            f->between->after = f->mom;
        }
        f->between = f->mom;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void func_print_shit(t_field *f){
    f->mom = f->between;
    do {
        printf("%d\n", f->mom->readInNumber);
        f->mom = f->mom->before;
    }while (f->mom != NULL);
}
