#include "config.c"

int isCopyElemnt ( int value, int arr[] );
void fillSizeList ( int arr[] );


int main()
{
    int count = 0;
    int sizeList[size];
    int i, j;
    char *typeOfAlgorithms[6] = {"Algorithm / n ", "Matrix Multiplication", "Bubble Sort", "Quick Sort", "Binary Search", "Finding Max. Element"};

    sizeList[0] = sizeof(sizeList)/sizeof(int) - 1;         /* first element holds length of the sizeList */
    srand(time(NULL));

    fillSizeList(sizeList + 1);                     /* generates random array sizes for each algorithms */

    for( i = 0; i < 6; i++ ) {
        printf("%-25s", typeOfAlgorithms[i]);
        for( j = 1; j < sizeList[0] + 1; j++) {
            switch( i ) {
                case 0 : printf("%-10d",sizeList[j]);
                    break;
                case 1 : generateMatrixMultiplation(sizeList[j]);
                    break;
                case 2 : generateBubbleSort(sizeList[j]);
                    break;
                case 3 : generateQuickSort(0, sizeList[j]);
                    break;
                case 4 : generateBinarySearch(0,sizeList[j]);
                    break;
                case 5 :  generateFindingMaxElement(sizeList[j]);
                    break;
                default:
                    break;

            }
        }
        printf("\n");
    }

    return 0;
}

int isCopyElemnt ( int value, int arr[] ) {
    /* check the given value is it already in array */
    int i = 0;

    for( i = 0; i < arr[-1]; i++ ) {
        if( arr[i] == value )
            return TRUE;
    }
    return FALSE;
}

void fillSizeList ( int arr[] ) {
    /* fillling given array with random numbers */
    int i = 0, temp = 0;

    while( i < arr[-1] ) {
        temp = (rand()%25) * 100;
        if( isCopyElemnt(temp, arr) || temp == 0 )
            continue;
        arr[i] = temp;
        i++;
    }
}

void fillArray( int arr[], int arrSize ) {
    int i = 0;

    for( i = 0; i < arrSize; i++ ) {
        arr[i] = rand();
    }
}


