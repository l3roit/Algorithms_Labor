#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define MAXNUM 1000

//Jonas Scherer Matrikelnummer 6499975

void rand_seq(int countNums);

int main(int argc, char *argv[]) {
    if (argc==1){
        printf("Please enter an argument");
        return 1;
    }else if (argc>2){
        printf("Please enter just one number as argument");
        return 1;
    }
    srand(time(NULL));
    rand_seq(atoi(argv[1]));
    return 0;
}

void rand_seq(int countNums){
    for(int j = 0; j < countNums; j++){
        printf("%d\n", (rand() % MAXNUM));
    }
}
