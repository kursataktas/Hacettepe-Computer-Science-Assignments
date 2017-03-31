
void generateFindingMaxElement ( int arrSize );
float getTimeForFindMaxElement( int arr[], int arrSize);
void fillArray( int arr[], int arrSize );
void findMaxElement( int arr[], int arrSize );

void generateFindingMaxElement ( int arrSize ) {
    int arr[maxArrSize], i;
    float time;

    fillArray(arr, arrSize);
    time = getTimeForFindMaxElement(arr, arrSize);
    printf("%-10.3f", time);
}

float getTimeForFindMaxElement( int arr[], int arrSize) {
    int i;
    float total = 0.0;
    clock_t start, stop;

    for(i = 0; i < repeatTime; i++) {
        start = clock();
        findMaxElement(arr, arrSize);
        stop  = clock();
        total += (float)(1000*(stop-start)/CLOCKS_PER_SEC);
    }
    total = (float) total / repeatTime;

    return total;
}

void findMaxElement( int arr[], int arrSize ) {
    int max, i;

    max = arr[0];
    for(i = 1; i < arrSize; i++) {
        if(max < arr[i])
            max = arr[i];
    }
}



