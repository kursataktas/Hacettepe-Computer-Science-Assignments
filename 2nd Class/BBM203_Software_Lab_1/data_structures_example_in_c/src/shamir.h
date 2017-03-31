#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE !(FALSE)
#define IS_EMPTY(ptr) (!(ptr))
#define COMPARE(x,y) ( (x)<(y) ? -1 : (y)<(x) ? 1 : 0 )

#ifndef SHMR_H_INCLUDED
#define SHMR_H_INCLUDED

typedef struct listNode *polyPointer;
typedef struct listNode {
    int partyId;
    int partyKey;
    int expon;
    float coef;
    polyPointer nextColumn;
    polyPointer nextRow;
    polyPointer nextSlice;
};


int openFiles (FILE **keyFilePtr, FILE **textFilePtr, FILE **encryptFilePtr, char *pathOfKeysFile, char *pathOfTextFile, char *pathOfEncryptFile);
int openFiles2 (FILE **keyFilePtr, FILE **subPolyFile,  char *pathOfKeysFile, char *pathOfSubPolyFile);
int readFile (FILE *filePtr, char buff[100]);
void parseKey (char buff[100], int **partyId, int **partyKey);
void listAddRow (polyPointer *head, polyPointer *last, int *partyId, int *partyKey);
void listAddColumn (polyPointer head);
void calculateSubPoly (polyPointer headColumn);
void deleteTerm (polyPointer prev);
polyPointer multiplyPoly (polyPointer aHead, polyPointer bHead);
polyPointer addTerm ( int expon, float coef, polyPointer prev);
void writeSubPolys ( polyPointer head, float *keyPtr, FILE *filePtr, int writeSubs );
void encryptText(FILE *plainTextFilePtr, FILE *ecryptTextFilePtr, float *keyPtr);

#endif
