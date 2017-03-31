#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define barSize 5
#define FALSE 0
#define TRUE !(FALSE)
#define IS_EMPTY(ptr) (!(ptr))
#define IS_FULL(ptr) (!(ptr))
#define COMPARE(x,y) ( (x)<(y) ? -1 : (y)<(x) ? 1 : 0 )

#define INPUTFILE "Input.txt"
#define OUTPUTFILE "Result.txt"

/**
    -> Oyun 25*81 node'dan oluþur. 0 sütun -1.sütun olarak kullanýlýr ve bar bu sütunun içindedir. ayný þekilde 81.sütunda.
    -> 2 tür struct var. ilki boxNode-> 25*81 lik node'lar herbiri birbirine 4lü döngüsel bað ile baðladýr. Örn: 10. sütun 24. satýrýn bir üstü 10. sütun 0.satýrdýr,
     böylece move için tek bir bað deðiþtirmek yeterli olmaktadýr. Ayrýca bu node'larýn hepsinin tagý vardýr, bu taglar ile yansýma, geçme ve score gibi durumlar
     bir sonraki baðýn tagýna bakýlarak yapýlabiliyor.
    -> 25*81 lik nodelar arasýnda topun bulunduðu node, dataNode structýna ait bir göstergeye sahip olur. bu gösterge oyunun o anki tüm bilgilerini taþýr.
    -> her score olduðunda oyun datalarý çiftbaðlý dataNode structýndan oluþan listede saklanýr ve goback komutu geldiðinde bu listeden bilgiler çekilir.
**/
typedef enum {greenSide, yellowSide, standart, bar, cortex} boxType;

typedef struct dataNode *dataNodePtr;
typedef struct dataNode{
    int playerScore1;
    int playerPosition1;
    int playerScore2;
    int playerPosition2;
    int ballPositionX;
    int ballPositionY;
    int ballSpeedX;
    int ballSpeedY;
    int mode;
    dataNodePtr right;
    dataNodePtr left;
};

typedef struct boxNode *boxNodePtr;
typedef struct boxNode {
    int row;
    int col;
    boxType type;
    dataNodePtr ballInfo;
    boxNodePtr right;
    boxNodePtr left;
    boxNodePtr up;
    boxNodePtr down;
};

void changeSpeed( int speedX, int speedY );
void changeMode( int mode );
void addGameData( );                                                                                                /* score olduðunda bilgileri ekle */
void setGameTable( int aPos, int bPos, int ballPosX, int ballPosY, int ballSpeedX, int ballSpeedY, int mode );      /* 4 baðlý tabloyu kur */
void processCommands( char *buff );
int openFile( FILE **filePtr, FILE **filePtr2 );
int readFile( FILE *filePtr, char *buff );
dataNodePtr newDataNode( void );
boxNodePtr newBoxNode( void );
void goBack( int back );
void move(int move1, int move2);
void writeFile(FILE *filePtr);

boxNodePtr ballPtr = NULL, bars[2], headCols[82];
dataNodePtr data = NULL;

int main(int argc, char *argv[]) {
    FILE *readFilePtr, *resultFilePtr;
    char  *buff = malloc(100 * sizeof(char));

    openFile(&readFilePtr, &resultFilePtr);
    while(readFile(readFilePtr, buff) == TRUE)
        processCommands(buff);
    writeFile(resultFilePtr);
}

void processCommands( char *buff ) {
    int aPos, bPos, ballPosX, ballPosY, ballSpeedX, ballSpeedY, mode, backCount;
    char *command, *delimeter = " ,";
    command = strtok(buff, delimeter);

    if(strcmp(command, "set") == 0){
        aPos = atoi(strtok(NULL, delimeter));
        bPos = atoi(strtok(NULL, delimeter));
        ballPosX = atoi(strtok(NULL, delimeter));
        ballPosY = atoi(strtok(NULL, delimeter));
        ballSpeedX = atoi(strtok(NULL, delimeter));
        ballSpeedY = atoi(strtok(NULL, delimeter));
        mode = atoi(strtok(NULL, delimeter));
        setGameTable(aPos, bPos, ballPosX, ballPosY, ballSpeedX, ballSpeedY, mode );
      }
    else if(strcmp(command, "move") == 0){
        aPos = atoi(strtok(NULL, delimeter));
        bPos = atoi(strtok(NULL, delimeter));
        move(aPos, bPos);

    }
    else if(strcmp(command, "changemode") == 0){
        mode = atoi(strtok(NULL, delimeter));
        changeMode(mode);
    }
    else if(strcmp(command, "changespeed") == 0){
        ballSpeedX = atoi(strtok(NULL, delimeter));
        ballSpeedY = atoi(strtok(NULL, delimeter));
        changeSpeed(ballSpeedX, ballSpeedY);
    }
    else if(strcmp(command, "goback") == 0){
        backCount = atoi(strtok(NULL, delimeter));
        goBack(backCount);
    }
}

void move(int move1, int move2) {
    boxNodePtr temp, temp2;
    int i, aNewPos, bNewPos, goX, goY,score = 0;

    goX = ballPtr->ballInfo->ballSpeedX;    goY = ballPtr->ballInfo->ballSpeedY;

    aNewPos = ballPtr->ballInfo->playerPosition1 = ballPtr->ballInfo->playerPosition1 + move1;
    bNewPos = ballPtr->ballInfo->playerPosition2 = ballPtr->ballInfo->playerPosition2 + move2;
    temp = bars[0];   temp2 = bars[1];
    for(i = 0; i < 25; i++) {       /* tüm bar sütunlarýný standart olarak taglandýr */
        temp->type = standart;
        temp2->type = standart;
        temp = temp->up;
        temp2 = temp2->up;
    }

    for(i = 0; i < 25; i++) {                 /* bar taglarýný güncelle */
        temp->type = ( i >= aNewPos && i < aNewPos + 5 ) ? bar : standart;
        temp2->type = ( i >= bNewPos && i < bNewPos + 5 ) ? bar : standart;
        temp = temp->up;
        temp2 = temp2->up;
    }

    for(i = 0; i < abs(goX) + abs(goY); i++){       /* harekete baþla */
        if(i%2 == 0){                               /* x ekseninde hareket */
            if(goX > 0){           /* hareket + yönde ise */
                if(ballPtr->type == yellowSide || ballPtr->type == cortex){     /* tansýma olacak mý ? */
                    ballPtr->left->ballInfo = ballPtr->ballInfo;
                    ballPtr = ballPtr->left;
                    ballPtr->ballInfo->ballSpeedX = -1 * (ballPtr->ballInfo->ballSpeedX);   /* topun hýzýný deðiþtir ve bilgiyi günceller */
                    goX = (-1 * goX);
                    continue;
                }
                else{               /* hareket - yönde ise */
                    temp = ballPtr->right;
                    if(temp->type == yellowSide){           /* score olabilme durumu */
                        temp2 = temp->right;
                        if(temp2->type != bar)                  /* bar yoksa score olsun */
                            score = 1;
                        if(score == 1)                             /* score olduysa bilgiyi günle */
                            ballPtr->ballInfo->playerScore1++;
                    }
                    ballPtr->right->ballInfo = ballPtr->ballInfo;
                    ballPtr = ballPtr->right;
                }
            }
            else{
                if(ballPtr->type == yellowSide || ballPtr->type == cortex){
                    ballPtr->right->ballInfo = ballPtr->ballInfo;
                    ballPtr = ballPtr->right;
                    ballPtr->ballInfo->ballSpeedX = -1 * (ballPtr->ballInfo->ballSpeedX);
                    goX = (-1 * goX);
                    continue;
                }
                else{
                    temp = ballPtr->left;
                    if(temp->type == yellowSide){
                        temp2 = temp->left;
                        if(temp2->type != bar)
                            score = 1;
                        if(score == 1)
                            ballPtr->ballInfo->playerScore2++;
                    }
                    ballPtr->left->ballInfo = ballPtr->ballInfo;
                    ballPtr = ballPtr->left;
                }
            }
        }
        else{           /* hareket y ekseninde */
                printf("top: %d %d %d\n",ballPtr->col, ballPtr->row, ballPtr->type);
            if(goY > 0){

                if(ballPtr->type == greenSide || ballPtr->type == cortex ){     /* yansýma yada geçiþme */
                    if(ballPtr->ballInfo->mode == 1){                           /* mode1 ise yansýt */
                        ballPtr->down->ballInfo = ballPtr->ballInfo;
                        ballPtr = ballPtr->down;
                        ballPtr->ballInfo->ballSpeedY = -1 * (ballPtr->ballInfo->ballSpeedY);
                        goY = (-1 * goY);
                        printf("tukardan yansıdı %d %d %d\n----------\n",ballPtr->col, ballPtr->row, ballPtr->type);
                    }
                    else{
                        ballPtr->up->ballInfo = ballPtr->ballInfo;      /* mode 2 ise geçiþ yap. döngüsel liste olduðu için 0.row'a geçiyor */
                        ballPtr = ballPtr->up;
                        printf("tukardan geçti %d %d %d\n----------\n",ballPtr->col, ballPtr->row, ballPtr->type);
                    }
                }
                else{
                    ballPtr->up->ballInfo = ballPtr->ballInfo;
                    ballPtr = ballPtr->up;
                    printf("normal geçti %d %d %d\n----------\n",ballPtr->col, ballPtr->row, ballPtr->type);
                }
            }
            else{
                if(ballPtr->type == greenSide || ballPtr->type == cortex ){
                    if(ballPtr->ballInfo->mode == 1){
                        ballPtr->up->ballInfo = ballPtr->ballInfo;
                        ballPtr = ballPtr->up;
                        ballPtr->ballInfo->ballSpeedY = -1 * (ballPtr->ballInfo->ballSpeedY);
                        goY = (-1 * goY);
                        printf("aşşadan yansıdı %d %d %d\n----------\n",ballPtr->col, ballPtr->row, ballPtr->type);
                    }
                    else{
                        ballPtr->down->ballInfo = ballPtr->ballInfo;
                        ballPtr = ballPtr->down;
                        printf("aşşadan ygetçi %d %d %d\n----------\n",ballPtr->col, ballPtr->row, ballPtr->type);
                    }
                }
                else{
                    ballPtr->down->ballInfo = ballPtr->ballInfo;
                    ballPtr = ballPtr->down;
                    printf("normal ygetçi %d %d %d\n----------\n",ballPtr->col, ballPtr->row, ballPtr->type);
                }
            }
        }
    }

    if(score == 1)
        addGameData();
}

void setGameTable( int aPos, int bPos, int ballPosX, int ballPosY, int ballSpeedX, int ballSpeedY, int mode ) {
    int i, j, k;
    boxNodePtr temp, pre, last;
    dataNodePtr temp2;

    for(i = 0; i < 82; i++){            /* 82 sütun için header niteliði taþýyan ilk nodelar oluþturulur. */
        headCols[i] = newBoxNode();
        headCols[i]->type = ((i == 0 && aPos == 0) || (i == 81 && bPos == 0)) ? bar : standart;     /* tag isimlendirme */
        headCols[i]->type = ( i > 1 && i < 80 ) ?  greenSide : headCols[i]->type;
        headCols[i]->type = ( i == 1 || i == 80 ) ?  yellowSide : headCols[i]->type;
        headCols[i]->row  = 0;
        headCols[i]->col  = i - 1;
        headCols[i]->type = (i == 1 || i == 80) ? cortex : temp->type;
        if(headCols[i]->type == cortex)
            printf("%d %d",headCols[i]->row,headCols[i]->col);
    }

    for(i = 0; i < 82; i++){            /* satýrlar oluþturulup birbirine dikeysel olarak baðlanýr */
        last = headCols[i];
        for(j = 1; j < 25; j++){
            temp = newBoxNode();
            temp->down = last;
            temp->row  = j;
            temp->col  = last->col;
            temp->type = ((i == 0 && j >= aPos && j < aPos + 5) || (i == 81 && j >= bPos && j < bPos + 5)) ? bar : standart;
            temp->type = ( i == 1 || i == 80 ) ?  yellowSide : temp->type;
            temp->type = ( j == 24 && i > 1 && i < 80 ) ?  greenSide : temp->type;
            temp->type = ( (j == 24 && i == 1) || (j == 24 && i == 80) ) ? cortex : temp->type;
            if(temp->type == cortex)
            printf("%d %d",temp->row,temp->col);
            if(ballPosX + 1 == i && ballPosY == j){
                temp2 = newDataNode();

                temp2->mode = mode;
                temp2->ballSpeedX = ballSpeedX;
                temp2->ballSpeedY = ballSpeedY;
                temp2->playerScore2 = 0;
                temp2->playerScore1 = 0;
                temp2->playerPosition1 = aPos;
                temp2->playerPosition2 = bPos;

                temp->ballInfo = temp2;
                ballPtr = temp;
                addGameData();
            }
            last->up   = temp;
            last       = temp;
        }
        last->up           = headCols[i];
        headCols[i]->down  = last;
    }

    for(i = 0; i < 81; i++){            /* baðlý olan bütün satýrlar yatay olarak baþtan aþaðý birbirine baðlanýr */
        pre  = headCols[i];
        last = headCols[i+1];
        for(j = 0; j < 25; j++) {
            pre->right  = last;
            last->left  = pre;
            pre = pre->up;
            last = last->up;
        }
    }

    bars[0] = headCols[0];   bars[1] = headCols[81];        /* player barlarý */
    free(temp);  free(last); free(pre);
}

void addGameData() {
    dataNodePtr temp;

    temp = newDataNode();
    *temp = *(ballPtr->ballInfo);
    temp->ballPositionX = ballPtr->col;
    temp->ballPositionY = ballPtr->row;
    temp->right = NULL;
    temp->left  = NULL;

    if(IS_EMPTY(data))
        data = temp;
    else {
        data->right = temp;
        temp->left = data;
        data = temp;
    }
}

int openFile( FILE **filePtr, FILE **filePtr2 ) {
    *filePtr  = fopen(INPUTFILE, "r");
    *filePtr2 = fopen(OUTPUTFILE, "w");
    if (*filePtr == NULL)
        return FALSE;
    return TRUE;
}

int readFile( FILE *filePtr, char *buff ) {
    if(fgets(buff, 100, filePtr) != NULL)
        return TRUE;
    return FALSE;
}

dataNodePtr newDataNode( void ) {
    dataNodePtr temp;
    temp = (dataNodePtr) malloc(sizeof(struct dataNode));
    if(IS_FULL(temp)) {
        printf("The memory is full");
        exit(1);
    }
    return temp;
}

boxNodePtr newBoxNode( void ) {
    boxNodePtr temp;
    temp = (boxNodePtr) malloc(sizeof(struct boxNode));
    if(IS_FULL(temp)) {
        printf("The memory is full");
        exit(1);
    }
    return temp;
}

void changeMode( int mode ) {
    ballPtr->ballInfo->mode = mode;
}

void changeSpeed( int speedX, int speedY ) {
    ballPtr->ballInfo->ballSpeedX = speedX;
    ballPtr->ballInfo->ballSpeedY = speedY;
}

void goBack( int back ) {
    int i;

    for(i = 0; i < back; i++)
        data = data->left;
    ballPtr->col = data->ballPositionX;
    ballPtr->row = data->ballPositionY;
    *(ballPtr->ballInfo) = *data;
}

void writeFile(FILE *filePtr){
    fprintf(filePtr,"%d %d %d %d %d %d",
    ballPtr->ballInfo->playerPosition1, ballPtr->ballInfo->playerPosition2, ballPtr->col, ballPtr->row, ballPtr->ballInfo->playerScore1,ballPtr->ballInfo->playerScore2);
}
