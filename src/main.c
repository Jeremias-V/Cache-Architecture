#include<stdio.h>
#include<stdlib.h>
#include "../include/CacheStruct.h"
#include "../include/Initialize.h"

char RAM_Data[2048][OFFSET_SIZE];
size_t firstCall = 1;

void interface(){
    if(firstCall){
        printf("==================================================\n");
        printf("||\tSet Associative LRU Cache Simulation\t||\n");
        printf("==================================================\n");
        firstCall = 0;
    }
    //printf();
}

void readRAM(){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("RAM-Data.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    size_t i, j;
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        for(j = 0; j < OFFSET_SIZE; j++){
            RAM_Data[i][j] = line[j];
        }
        i++;
    }

    fclose(fp);
    if(line){
        free(line);
    }

}

int main(){

    // Cache Declaration
    CacheSet Cache[NUMBER_OF_BLOCKS][BLOCK_SIZE];
    initalizeCache(Cache);

    // Read RAM File.
    readRAM();
   
    // Call read function
    interface();

    /* This is for printing the lines and verifying
    size_t i, j;
    for(i = 0; i < 2048; i ++){
        printf("LINE %ld =\t", i+1);
        for(j = 0; j < OFFSET_SIZE; j++){
            putchar(RAM_Data[i][j]);
        }
        putchar('\n');
    }
    */

    return 0;
}
