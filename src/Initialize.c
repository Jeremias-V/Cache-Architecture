#include "../include/Initialize.h"

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
