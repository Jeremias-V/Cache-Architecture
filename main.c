#include<stdio.h>

#define BLOCK_SIZE 8
#define NUMBER_OF_BLOCKS 4

typedef struct{

    unsigned int V : 1;
    unsigned int Tag : 2;
    unsigned int Data : 3;

} Set;


int main(){

    Set test[NUMBER_OF_BLOCKS][BLOCK_SIZE];
    //printf("Size of SET: %ld\n", sizeof(test[0][0]));
    for(int i = 0; i < 9; i++){
        printf("V: %d\t\tTag: %d\t\tData: %d\n", test[0][0].V++, test[0][0].Tag++, test[0][0].Data++);
    }
    return 0;
}
