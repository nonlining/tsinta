/*Analyze training data*/
/*Author: tsinta*/
#include <stdio.h>
#include <stdlib.h>
#include "AnalyzeTrainingData.h"

PLAData* convertToPLAData(int **data, size_t numData, size_t numVal)
{
    /*data: input data from file*/
    /*out: Converted data*/
    size_t i;    /*idx of numData*/
    
    PLAData *pData = (PLAData*)malloc(sizeof(PLAData) * numData);
    
    if(pData == NULL) {
        fprintf(stderr, "Failed to malloc in convertToPLAData\n");
        return NULL;
    }
    for (i = 0; i < numData; ++i) {
        pData[i].val = data[i];
        pData[i].isGood = (data[i][numVal - 1] >= 0) ? GOOD : BAD;
    }
    return pData;
}

Weight genInitWeight(size_t numPLAVal)
{
    /*out: init weight*/
    size_t i;   /*idx of wt.w*/
    Weight wt;  /*init weight, it will malloc wt.w*/
    
    wt.w = (int*)malloc(sizeof(int) * numPLAVal);
    if(wt.w == NULL) {
        fprintf(stderr, "Failed to malloc in genInitWeight\n");
        wt.threshold = 0;
        return wt;
    }
    for (i = 0; i < numPLAVal; ++i)
        wt.w[i] = 0;
    wt.threshold = 0;
    return wt;
}

int checkPLAData(PLAData pData, Weight wt, size_t numPLAVal)
{
    /*out: 0->correct, not 0->not correct*/
    size_t i;   /*idx of pData.val*/
    int innerProduct = 0;
    
    for (i = 0; i < numPLAVal; ++i)
        innerProduct += pData.val[i] * wt.w[i];
    if (innerProduct == wt.threshold)
        return 0;
    return ((innerProduct > wt.threshold) ? GOOD : BAD) - pData.isGood;
}

void adjustWeight(PLAData pData, Weight *wt, size_t numPLAVal)
{
    size_t i;   /*idx of pData.val*/
    
    for (i = 0; i < numPLAVal; ++i)
        wt->w[i] += ((pData.isGood == GOOD) ? pData.val[i] : -pData.val[i]);
    wt->threshold += ((pData.isGood == GOOD) ? -1 : 1);
}

int checkIfWeightIsZero(Weight wt, size_t numPLAVal)
{
    /*Check if the weight is 0 vector*/
    /*out: 0->0 vector, not 0->none 0 vector*/
    size_t i;   /*idx of wt.w*/
    
    for (i = 0; i < numPLAVal; ++i) {
        if (wt.w[i] != 0)
            return wt.w[i];
    }
    return wt.threshold;
}

size_t countNumCorrect(PLAData *pData, Weight wt, size_t numData, size_t numPLAVal)
{
    /*out: # of training result is correct*/
    if (checkIfWeightIsZero(wt, numPLAVal) == 0)
        return 0;
    
    size_t i;   /*idx of pData*/
    size_t numCorrect = 0;
    
    for (i = 0; i < numData; ++i) {
        if (checkPLAData(pData[i], wt, numPLAVal) == 0)
            ++numCorrect;
    }
    return numCorrect;
}

void showWeight(Weight wt, size_t numPLAVal)
{
    if (numPLAVal == 0)
        return;
    printf("%d*X1", wt.w[0]);
    
    size_t i;
    
    for(i = 1; i < numPLAVal; ++i)
        printf(" + %d*X%u", wt.w[i], (unsigned int)i + 1);
    printf(" > %d -> O\n", wt.threshold);
    printf("%d*X0", wt.w[0]);
    for(i = 1; i < numPLAVal; ++i)
        printf(" + %d*X%u", wt.w[i], (unsigned int)i + 1);
    printf(" < %d -> X\n", wt.threshold);
}

void closeWeight(Weight wt)
{
    if (wt.w != NULL) {
        free(wt.w);
        wt.w = NULL;
    }
}

/*
#include "ReadTrainingData.h"

int main()
{
    char fileName[100];
    scanf("%s", fileName);
    size_t numData = 0, numVal = 0;
    int **data = ReadTrainingData(fileName, &numData, &numVal);
    PLAData *pData = convertToPLAData(data, numData, numVal);
    size_t i,j;
    for (i = 0; i < numData; ++i) {
        for (j = 0; j < numVal - 1; ++j)
            printf("%d , ", pData[i].val[j]);
        printf("%c\n", (pData[i].isGood == GOOD) ? 'O' : 'X');
    }
    size_t numPLAVal = numVal - 1;
    int idx;
    if (idx < numData) {
        Weight wt = genInitWeight(numPLAVal);
        while(1) {
            showWeight(wt, numPLAVal);
            scanf("%d", &idx);
            if (idx >= numData)
                break;
            int check = 0;
            printf("check = %d\n", (check = checkPLAData(pData[idx], wt, numPLAVal)));
            if (check != 0 || (checkIfWeightIsZero(wt, numPLAVal) == 0))
                adjustWeight(pData[idx], &wt, numPLAVal);
        }
        size_t nc = countNumCorrect(pData, wt, numData, numPLAVal);
        printf("correct: %u / %u, %g%%\n", (unsigned int)nc
            , (unsigned int)numData, (double)(nc * 100) / (double)numData);
        closeWeight(wt);
    }
    if (data != NULL)
        closeTrainingData(data, numData);
    return 0;
}
*/
