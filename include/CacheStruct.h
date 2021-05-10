#ifndef CACHE_STRUCT_H
#define CACHE_STRUCT_H

#define BLOCK_SIZE 8
#define NUMBER_OF_BLOCKS 4
#define OFFSET_SIZE 8

#include<stdio.h>

typedef struct{

    size_t M: 3;

} MemData;

typedef struct{

    size_t V : 1;
    size_t Tag : 2;
    size_t usedTime;
    MemData Data[8];

} CacheSet;

#endif