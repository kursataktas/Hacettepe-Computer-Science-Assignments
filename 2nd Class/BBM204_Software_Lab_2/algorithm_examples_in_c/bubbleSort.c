

void generateBubbleSort ( int arrSize );
float getTimeForBubbleSort( int arr[], int arrSize );
void fillArray( int arr[], int arrSize );
void bubbleSort( int arr[], int arrSize );

void generateBubbleSort ( int arrSize ) {
    int arr[maxArrSize],i;
    float time;

    fillArray(arr, arrSize);
    time = getTimeForBubbleSort(arr, arrSize);
    printf("%-10.2f", time);
}

float getTimeForBubbleSort( int arr[], int arrSize ) {
    int i;
    float total = 0.0;
    clock_t start, stop;


    for(i = 0; i < repeatTime; i++) {
        start = clock();
        bubbleSort(arr, arrSize);
        stop  = clock();
        total += (float)(1000*(stop-start)/CLOCKS_PER_SEC);
        fillArray(arr, arrSize);
    }
    total = (float) total / repeatTime;
    return total;
}

void bubbleSort( int arr[], int arrSize ) {
    int i, j, swaps = 0, temp;

    for(i = 0; i < arrSize; i++) {
        swaps = 0;
        for(j = 0; j < arrSize-1; j++) {
            if(arr[j] > arr[j+1]) {
                SWAP(arr[j], arr[j+1], temp);
                swaps++;
            }
        }
        if( swaps == 0 )
            break;
    }
}


