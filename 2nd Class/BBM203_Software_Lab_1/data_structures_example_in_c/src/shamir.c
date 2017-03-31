#include "shamir.h"

int main(int argc, char *argv[])
{
    FILE *keysFile = NULL, *plainTextFile = NULL, *encryptedTextFile = NULL, *subPolyFile = NULL;
    char  *buff = malloc(100 * sizeof(char));
    int  *partyId = NULL, *partyKey = NULL, writeSubs = 0;
    float key = 0, *keyPtr = &key;
    polyPointer head = NULL, last = NULL;

    if( strcmp(argv[1],"-e") == 0 ) {
        openFiles1(&keysFile, &plainTextFile, &encryptedTextFile,argv[2],argv[3],argv[4]);

        while( readFile(keysFile, buff) == TRUE ){
            parseKey(buff, &partyId, &partyKey);
            listAddRow(&head, &last, partyId, partyKey);
        }

        listAddColumn(head);
        writeSubPolys(head, keyPtr, subPolyFile, writeSubs);
        encryptText(plainTextFile, encryptedTextFile, keyPtr);

        fclose(keysFile);
        fclose(plainTextFile); fclose(encryptedTextFile);
    }
    else {
        openFiles2(&keysFile, &subPolyFile, argv[2],argv[3]);

        while( readFile(keysFile, buff) == TRUE ){
            parseKey(buff, &partyId, &partyKey);
            listAddRow(&head, &last, partyId, partyKey);
        }

        listAddColumn(head);
        writeSubs = 1;
        writeSubPolys(head, keyPtr, subPolyFile, writeSubs);

        fclose(keysFile);
        fclose(subPolyFile);
    }
    return 0;
}

int openFiles1 (FILE **keyFilePtr, FILE **textFilePtr, FILE **encryptFilePtr, char *pathOfKeysFile, char *pathOfTextFile, char *pathOfEncryptFile) {
    /* Takes files' pointers and files' path as arguments.
       Then Opens file.
       If occurs any error while opening files, function returns 0 (FALSE) value
       otherwise returns 1 (TRUE).                                               */

    *keyFilePtr  = fopen(pathOfKeysFile,"r");
    *textFilePtr = fopen(pathOfTextFile,"r");
    *encryptFilePtr = fopen(pathOfEncryptFile,"w");

    if (*keyFilePtr == NULL || *textFilePtr == NULL || *encryptFilePtr == NULL )
        return FALSE;

    return TRUE;
}

int openFiles2 (FILE **keyFilePtr, FILE **subPolyFile,  char *pathOfKeysFile, char *pathOfSubPolyFile) {
    /* Takes files' pointers and files' path as arguments.
       Then Opens file.
       If occurs any error while opening files, function returns 0 (FALSE) value
       otherwise returns 1 (TRUE).                                               */

    *keyFilePtr  = fopen(pathOfKeysFile,"r");
    *subPolyFile = fopen(pathOfSubPolyFile,"w");

    if (*keyFilePtr == NULL || *subPolyFile == NULL )
        return FALSE;

    return TRUE;
}

int readFile (FILE *filePtr, char *buff) {
    /* Takes files' pointers and a array.
       Then read the file which point from filePtr and write inside buff array */

    if(fgets(buff, 100, filePtr) != NULL)
        return TRUE;

    return FALSE;
}

void parseKey (char *buff, int **partyId, int **partyKey) {
    *partyId = NULL;
    *partyKey = NULL;
    *partyId = (int *)atoi(strtok(buff," : "));
    *partyKey = (int *)atoi(strtok(NULL," : "));
}

void listAddRow (polyPointer *head, polyPointer *last, int *partyId, int *partyKey) {

     polyPointer temp = NULL;

     temp = (polyPointer)malloc(sizeof(struct listNode));
     temp->partyId = partyId;
     temp->partyKey = partyKey;
     temp->nextColumn = NULL;
     temp->nextRow = NULL;
     temp->nextSlice = NULL;

     if( IS_EMPTY(*head) ) {
        *head = temp;
        *last = temp;
     }
     else {
        (*last)->nextRow = temp;
        *last = temp;
     }
}

void listAddColumn (polyPointer head) {
    /* creates subPolynomials and add they to list rows */

    polyPointer traveller = NULL, headColumn = NULL, headSlice = NULL, prevSlice = NULL;

    headColumn = head;
    traveller = head->nextRow;
    while( headColumn != NULL ) {
        headSlice = (polyPointer)malloc(sizeof(struct listNode));
        headColumn->nextColumn = headSlice;
        while( traveller != NULL ){
            if( traveller->partyId == headColumn->partyId ) {
                traveller = traveller->nextRow;
                continue;
            }

            headSlice->expon = 1;
            headSlice->coef = (float) 1 / (headColumn->partyId - traveller->partyId);
            headSlice->nextSlice = (polyPointer)malloc(sizeof(struct listNode));
            headSlice->nextSlice->expon = 0;
            headSlice->nextSlice->coef = (float) -1 * traveller->partyId / (headColumn->partyId - traveller->partyId);
            headSlice->nextSlice->nextSlice = NULL;

            headSlice->nextColumn = (polyPointer)malloc(sizeof(struct listNode));
            prevSlice = headSlice;
            headSlice = headSlice->nextColumn;
            traveller = traveller->nextRow;
        }
        prevSlice->nextColumn = NULL;
        calculateSubPoly(headColumn);
        traveller = head;
        headColumn = headColumn->nextRow;
    }
}

void calculateSubPoly (polyPointer headColumn) {

    polyPointer temp = NULL, headSlice = NULL, removed1, removed2;

    headSlice = headColumn->nextColumn;
    while( headSlice->nextColumn != NULL ){
        temp = multiplyPoly(headSlice, headSlice->nextColumn);
        temp->nextColumn = headSlice->nextColumn->nextColumn;
        removed1 = headSlice;
        removed2 = headSlice->nextColumn;
        headColumn->nextColumn = temp;
        free(removed1);
        free(removed2);
        headSlice = headColumn->nextColumn;
    }
}

polyPointer multiplyPoly (polyPointer aHead, polyPointer bHead) {
    /* return a polynomial which is the sum of a and b */

    polyPointer aTraveller, bTraveller, rTraveller, startIndex = NULL, rHead = NULL;
    int rExpon = 0;
    float rCoef = 0;

    for( aTraveller = aHead; aTraveller; aTraveller = aTraveller->nextSlice ) {
        rTraveller = startIndex;
        for( bTraveller = bHead; bTraveller; bTraveller = bTraveller->nextSlice ) {
            rExpon = aTraveller->expon + bTraveller->expon;
            rCoef  = aTraveller->coef * bTraveller->coef;

            if( rHead ) {
                while( rTraveller->nextSlice && ( rTraveller->nextSlice->expon > rExpon ) )
                    rTraveller = rTraveller->nextSlice;

                if( !rTraveller->nextSlice || ( rTraveller->nextSlice->expon < rExpon ) )
                    addTerm(rExpon, rCoef, rTraveller);
                else if( ( rTraveller->nextSlice->coef += rCoef ) == 0 )
                    deleteTerm(rTraveller);
            }
            else
                rHead = rTraveller = startIndex = addTerm(rExpon, rCoef, NULL);

            if( bTraveller == bHead )
                startIndex = rTraveller;
        }
    }
    return rHead;
}

void deleteTerm (polyPointer prev) {
    polyPointer temp = prev->nextSlice;
    prev->nextSlice = prev->nextSlice->nextSlice;
    free(temp);
}

polyPointer addTerm ( int expon, float coef, polyPointer prev) {

    polyPointer term = (polyPointer)malloc(sizeof(struct listNode));
    term->expon = expon;
    term->coef  = coef;

    if( prev ) {
        term->nextSlice = prev->nextSlice;
        prev->nextSlice = term;
    }
    else
        term->nextSlice = NULL;

    return term;
}

void writeSubPolys ( polyPointer head, float *keyPtr, FILE *filePtr, int writeSubs ) {

    int i = 0;
    polyPointer headSlice, headColumn = head;
    while( headColumn != NULL ){
        if( writeSubs == TRUE ) {
            i++;
            fprintf(filePtr,"%d : ",i);
        }

        headSlice = headColumn->nextColumn;
        while( headSlice != NULL ){
            switch(headSlice->expon){
                case 0   :  if( writeSubs == TRUE ) fprintf(filePtr,"%.2f", headSlice->coef);
                            *keyPtr += headSlice->coef * headColumn->partyKey;
                            break;
                case 1   :  if( writeSubs == TRUE ) fprintf(filePtr,"%.2fx ", headSlice->coef);
                            break;
                default  :  if( writeSubs == TRUE ) fprintf(filePtr,"%.2fx^%d ", headSlice->coef, headSlice->expon);
                            break;
            }
            headSlice = headSlice->nextSlice;
        }

        if( writeSubs == TRUE ) fprintf(filePtr,"\n");
        headColumn = headColumn->nextRow;
    }
}

void encryptText(FILE *plainTextFilePtr, FILE *ecryptTextFilePtr, float *keyPtr) {

    int key = (int)*keyPtr, encrytptInt;
    char c, ecryptedC;

    fprintf(ecryptTextFilePtr,"%d\n\n", key);
    while( (c = fgetc(plainTextFilePtr)) != EOF ){
        if( c == ' ' )
            continue;
        encrytptInt = key ^ (int)c;
        fprintf(ecryptTextFilePtr,"%c", (char)encrytptInt);
    }
    encrytptInt = key ^ 10;
    fprintf(ecryptTextFilePtr,"%c", (char)encrytptInt);
}





