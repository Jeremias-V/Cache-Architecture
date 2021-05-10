#include<stdio.h>
#include<stdlib.h>
#include "../include/CacheStruct.h"
#include "../include/Initialize.h"

#define TRUE 1
#define FALSE 0

char RAM_Data[2048][OFFSET_SIZE], binaryTag[5], binaryIndex[3], binaryOffset[3];
CacheSet Cache[NUMBER_OF_BLOCKS][BLOCK_SIZE];
size_t firstCall = TRUE, missCount = 0, queries = 0, hitCount = 0, time = 0;

void clearBuffer(){
    char tmp = getchar();
    while(tmp != '\n'){
        tmp = getchar();
    }
}

size_t getLesserBlock(size_t idx){
    // This means we are searching for the least frequently used block for a set.
    size_t i = 0, minTime = 65535, block = -1;
    for(i = 0; i < NUMBER_OF_BLOCKS; i++){
        if(Cache[i][idx].usedTime < minTime){
            minTime = Cache[i][idx].usedTime;
            block = i;
        }
    }
    return block;
}

int readCache(size_t idx, size_t offset, size_t tag){
    // Si el tag de la cache es igual al tag de la direccion que entra y el bit de validez esta en uno es hit
    // El bit de validez se pone en uno siempre que se traiga algo de la RAM
    size_t i = 0, j;
    for(i = 0; i < NUMBER_OF_BLOCKS; i++){
        if(Cache[i][idx].V == 1 && tag == Cache[i][idx].Tag){
            Cache[i][idx].usedTime = ++time;
            return Cache[i][idx].Data[offset].M;
        }
    }
    return -1;
}

size_t intToBin3(size_t n, char ans[3]){
    if(n < 0 || n > 7){
        return FALSE;
    }else{
        size_t i = 3;
        while(n > 0){
            i--;
            ans[i] = ((n % 2) + '0');
            n /= 2;
        }
        while(i > 0){
            ans[--i] = '0';
        }
        return TRUE;
    }
}

size_t binToInt11(char * bin){
    size_t size = 11, tmp, tmp2;
    tmp2 = 1;
    tmp = 0;
    for(int i = size-1; i >= 0; i--){
        tmp += tmp2 * (*(bin+i) - '0');
        tmp2 *= 2;
    }
    return tmp;
}

size_t binToInt8(char * bin){
    size_t size = 8, tmp, tmp2;
    tmp2 = 1;
    tmp = 0;
    for(int i = size-1; i >= 0; i--){
        tmp += tmp2 * (*(bin+i) - '0');
        tmp2 *= 2;
    }
    return tmp;
}

size_t getAddress(size_t offset){
    char address[11], offsetAddress[3];
    size_t i = 0;
    for(i = 0; i < 5; i++){
        address[i] = binaryTag[i];
    }
    for(i = 5; i < 8; i++){
        address[i] = binaryIndex[i-5];
    }
    intToBin3(offset, &offsetAddress);
    for(i = 8; i < 11; i++){
        address[i] = offsetAddress[i-8];
    }
    return binToInt11(&address);
}

void writeOffset(size_t block, size_t idx){
    size_t i;
    for(i = 0; i < 8; i++){
        Cache[block][idx].Data[i].M = binToInt8(&RAM_Data[getAddress(i)]);
    }
}

void writeCache(size_t idx, size_t tag){
    size_t i;
    
    for(i = 0; i < NUMBER_OF_BLOCKS; i++){
        if(Cache[i][idx].V == 0){
            Cache[i][idx].V = 1;
            Cache[i][idx].usedTime = ++time;
            Cache[i][idx].Tag = tag;
            writeOffset(i, idx);
            return;
        }
    }
    
    // LRU
    size_t block = getLesserBlock(idx);
    Cache[block][idx].usedTime = ++time;
    Cache[block][idx].Tag = tag;
    writeOffset(block, idx);
}

size_t intToBin(size_t n, char ans[11]){
    if(n < 0 || n > 2047){
        return FALSE;
    }else{
        size_t i = 11;
        while(n > 0){
            i--;
            ans[i] = ((n % 2) + '0');
            n /= 2;
        }
        while(i > 0){
            ans[--i] = '0';
        }
        return TRUE;
    }
}

size_t intToBin8(size_t n, char ans[8]){
    if(n < 0 || n > 255){
        return FALSE;
    }else{
        size_t i = 8;
        while(n > 0){
            i--;
            ans[i] = ((n % 2) + '0');
            n /= 2;
        }
        while(i > 0){
            ans[--i] = '0';
        }
        return TRUE;
    }
}

void getOffset(char bin[11]){
    size_t i;
    for(i = 8; i < 11; i++){
        binaryOffset[i-8] = bin[i];
    }
}

void getIndex(char bin[11]){
    size_t i;
    for(i = 5; i < 8; i++){
        binaryIndex[i-5] = bin[i];
    }
}

void getTag(char bin[11]){
    size_t i;
    for(i = 0; i < 5; i++){
        binaryTag[i] = bin[i];
    }
}

size_t binToInt3(char * bin){
    size_t size = 3, tmp, tmp2;
    tmp2 = 1;
    tmp = 0;
    for(int i = size-1; i >= 0; i--){
        tmp += tmp2 * (*(bin+i) - '0');
        tmp2 *= 2;
    }
    return tmp;
}

size_t binToInt5(char * bin){
    size_t size = 5, tmp, tmp2;
    tmp2 = 1;
    tmp = 0;
    for(int i = size-1; i >= 0; i--){
        tmp += tmp2 * (*(bin+i) - '0');
        tmp2 *= 2;
    }
    return tmp;
}

void interface(){
    char opt, binNum[11], binVal[8];
    size_t b10offset, b10index, b10tag, i, j, k;
    int value, r;
    int cacheAns;
    if(firstCall){
        printf("==================================================\n");
        printf("||\tSet Associative LRU Cache Simulation\t||\n");
        printf("==================================================\n");
        printf("> '1' Read from Cache.\n");
        printf("> '2' Write to Mem.\n");
        printf("> '3' Exit.\n");
        printf("> '4' Manual read from cache.\n");
        printf("> '5' Print Cache contents.\n");
        printf("==================================================\n");
        firstCall = FALSE;
    }
    scanf(" %c", &opt);
    switch (opt){
    case '1':
        r = rand() % 2048;
        intToBin(r, &binNum);
        getOffset(binNum);
        getIndex(binNum);
        getTag(binNum);
        printf("\n========================================\nRandom Address = %s\nAddress in Base 10 = %d\n", binNum, r);
        b10tag = binToInt5(binaryTag);
        b10index = binToInt3(binaryIndex);
        b10offset = binToInt3(binaryOffset);
        cacheAns = readCache(b10index, b10offset, b10tag);
        if(cacheAns == -1){
            missCount++;
            printf("\n----- MISS -----\nRate = %.2f%\n---------------\n========================================\n", (1 - ((float)hitCount/((float)hitCount + (float)missCount))) * 100);
            writeCache(b10index, b10tag);
        }else{
            hitCount++;
            printf("\n+++++ HIT +++++\nLine in .txt = %d\nValue = %d\n+++++++++++++++\n========================================\n", r+1, cacheAns);
        }
        interface();
        break;
    case '2':
        scanf(" %d %d", &r, &value);
        if(((r >= 0) && (r <= 2047))){
            if(((value >= 0) && (value < 256))){
                intToBin(r, &binNum);
                getOffset(binNum);
                getIndex(binNum);
                getTag(binNum);
                b10tag = binToInt5(binaryTag);
                b10index = binToInt3(binaryIndex);
                printf("\n========================================\nUser Address = %s\nAddress in Base 10 = %d\nValue to store = %d\n========================================\n", binNum, r, value);
                intToBin8(value, &binVal);
                for(i = 0; i < NUMBER_OF_BLOCKS; i++){
                    if(Cache[i][b10index].Tag == b10tag){
                        Cache[i][b10index].V = 0;
                    }
                }
                for(i = 0; i < 8; i++){
                    RAM_Data[r][i] = binVal[i];
                }
                writeCache(b10index, b10tag);
                writeRAM();
            }
        }else{
            printf("\nERROR: After selecting 2. you must type ADDRESS VALUE where 1 <= ADRESS <= 2048, 0 <= VALUE <= 255.\n\n");
        }
        interface();
        break;
    case '3':
        break;
    case '4':
        scanf(" %d", &r);
        if(((r >= 0) && (r <= 2047))){
            intToBin(r, &binNum);
            getOffset(binNum);
            getIndex(binNum);
            getTag(binNum);
            b10tag = binToInt5(binaryTag);
            b10index = binToInt3(binaryIndex);
            b10offset = binToInt3(binaryOffset);
            printf("\n========================================\nUser Address = %s\nAddress in Base 10 = %d\n========================================\n", binNum, r);
            cacheAns = readCache(b10index, b10offset, b10tag);
            if(cacheAns == -1){
                missCount++;
                printf("----- MISS -----\nRate = %.2f%\n---------------\n========================================\n", (1 - ((float)hitCount/((float)hitCount + (float)missCount))) * 100);
                writeCache(b10index, b10tag);
            }else{
                hitCount++;
                printf("+++++ HIT +++++\nLine in .txt = %d\nValue = %d\n+++++++++++++++\n========================================\n", r+1, cacheAns);
            }
        }else{
            printf("\nERROR: After selecting 2. you must type ADDRESS VALUE where 1 <= ADRESS <= 2048, 0 <= VALUE <= 255.\n\n");
        }
        interface();
        break;
    case '5':
        for(i = 0; i < NUMBER_OF_BLOCKS; i++){
            printf("\n\n============================ BLOCK #%d ============================\n", i+1);
            for(j = 0; j < BLOCK_SIZE; j++){
                printf("\n+ SET #%d: V = %d Tag = %d Time = %d\nData Contents:\nM = [", j, Cache[i][j].V, Cache[i][j].Tag, Cache[i][j].usedTime);
                for(k = 0; k < 8; k++){
                    printf(" %d", Cache[i][j].Data[k].M);
                }
                printf(" ]\n");
            }
        }
        printf("\n\n============================ END OF CONTENTS =======================\n\n");
        interface();
        break;
    default:
        printf("\nInvalid option, try '1' or '2' or '3'.\n");
        interface();
        break;
    }
    return;
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

void writeRAM(){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("RAM-Data.txt", "w");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    size_t i;
    for(i = 0; i < 2048; i++){
        if(i == 2047){
            fprintf(fp, "%.*s", sizeof(RAM_Data[i]), RAM_Data[i]);
        }else{
            fprintf(fp, "%.*s\n", sizeof(RAM_Data[i]), RAM_Data[i]);
        }
    }

    fclose(fp);
    if(line){
        free(line);
    }
}

int main(){

    // Cache Declaration
    initalizeCache(Cache);

    // Read RAM File.
    readRAM();
   
    // Call read function
    interface();

    return 0;
}
