
void generateQuickSort ( int low, int high );
float getTimeForQuickSort( int arr[], int low, int high );
void fillArray( int arr[], int arrSize );
void quickSort( int arr[], int low, int high );
int partition( int arr[], int low, int high );

void generateQuickSort ( int low, int high ) {
    int arr[maxArrSize];
    float time;

    fillArray(arr, high);
    time = getTimeForQuickSort(arr, low, high);
    printf("%-10.3f", time);
}

float getTimeForQuickSort( int arr[], int low, int high ) {
    int i;
    float total = 0.0;
    clock_t start, stop;

    for(i = 0; i < repeatTime; i++) {
        start = clock();
        quickSort(arr, low, high);
        stop  = clock();
        total += (float)(1000*(stop-start)/CLOCKS_PER_SEC);
        fillArray(arr, high);
    }
    total = (float) total / repeatTime;

    return total;
}

void quickSort( int arr[], int low, int high ) {
    int pivotLocation;

    if(low < high) {
        pivotLocation = partition(arr, low, high);
        quickSort(arr, low, pivotLocation -1 );
        quickSort(arr, pivotLocation + 1, high);
    }
}

int partition( int arr[], int low, int high ) {
    int pivot = arr[low], leftWall = low, i, temp;

    for(i = low + 1; i < high; i++) {
        if(arr[i] < pivot ){
            leftWall = leftWall + 1;
            SWAP(arr[i], arr[leftWall], temp);
        }
    }
    SWAP(arr[low], arr[leftWall], temp);
    return leftWall;
}



