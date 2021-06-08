#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAXSTRING 255+1;

typedef struct lin_list{
    char *payload;
    struct lin_list *next;
}LinListCell, *LinList_p;

LinList_p LinListAllocCell(char* payload);
void LinListFreeCell(LinList_p junk);
void LinListFree(LinList_p *junk);
LinList_p LinListInsertExtractFirst(LinList_p *anchor);

int main(int argv, char* argc[]) {
    char input[255+1];

    printf("Geben Sie ihre Zahlen ein\n");

    scanf("%s", &input);
    fflush(stdin);

    return 0;
}

LinList_p LinListAllocCell(char* payload){
    LinList_p liste = malloc(sizeof(struct lin_list));
    liste->payload = payload;
    return &liste;
}