
void generateBinarySearch ( int left, int right );
float getTimeForBinarySearch( int arr[], int left, int right );
void fillArray( int arr[], int arrSize );
int binarySearch( int arr[], int left, int right, int value );

void generateBinarySearch ( int left, int right ) {
    int arr[maxArrSize], value;
    float time;

    time = getTimeForBinarySearch(arr, left, right);
    printf("%-10.4f", time);
}

float getTimeForBinarySearch( int arr[], int left, int right ) {
    int result, i, value;
    float total = 0.0;
    clock_t start, stop;

    for(i = 0; i < repeatTime; i++) {
        value = rand();
        fillArray(arr, right);
        quickSort(arr, left, right);

        start = clock();
        result = binarySearch(arr, left, right, value);
        stop  = clock();
        total += (float)(1000*(stop-start)/CLOCKS_PER_SEC);
    }
    total = (float) total / repeatTime;

    return total;
}

int binarySearch( int arr[], int left, int right, int value ) {
    int mid = 0;

    while(left <= right) {
        mid = floor((right - left) / 2) + left;
        if(arr[mid] == value)
            return mid;
        if(value < arr[mid])
            right = mid - 1;
        else
            left = mid + 1;
    }
    return FALSE;
}




