#include<stdio.h>
#include<stdlib.h>

#define BLOCK_SIZE 8
#define NUMBER_OF_BLOCKS 4
#define OFFSET_SIZE 8

typedef struct{

    size_t M: 3;

} MemData;

typedef struct{

    size_t V : 1;
    size_t Tag : 2;
    size_t usedTime;
    MemData Data[8];

} CacheSet;

void initalizeCache(CacheSet c[NUMBER_OF_BLOCKS][BLOCK_SIZE]){
    size_t i, j, k;

    for(i = 0; i < NUMBER_OF_BLOCKS; i++){
        for(j = 0; j < BLOCK_SIZE; j++){
            c[i][j].V = 0;
            c[i][j].Tag = 0;
            c[i][j].usedTime = 0;
            for(k = 0; k < OFFSET_SIZE; k++){
                c[i][j].Data[k].M = 0;
            }
        }
    }
}

int main(){

    CacheSet Cache[NUMBER_OF_BLOCKS][BLOCK_SIZE];
    char RAM_Data[2048][OFFSET_SIZE];
    //printf("Size of SET: %ld\n", sizeof(Cache[0][0]));
    initalizeCache(Cache);
    for(int i = 0; i < 9; i++){
        printf("V: %d\t\tTag: %d\t\tData: %d\n", Cache[0][0].V++, Cache[0][0].Tag++, Cache[0][0].Data[0].M++);
    }

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("./RAM-Data.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i, j;
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        for(j = 0; j < OFFSET_SIZE; j++){
            RAM_Data[i][j] = line[j];
        }
        i++;
        //printf("%s", line);
    }

    fclose(fp);
    if(line){
        free(line);
    }

    /* This is for printing the lines and verifying
    for(i = 0; i < 2048; i ++){
        printf("LINE %d =\t", i+1);
        for(j = 0; j < OFFSET_SIZE; j++){
            putchar(RAM_Data[i][j]);
        }
        putchar('\n');
    }
    */
    
    return 0;
}
