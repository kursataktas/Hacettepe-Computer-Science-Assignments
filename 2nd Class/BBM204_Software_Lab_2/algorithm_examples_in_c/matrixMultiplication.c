
void generateMatrixMultiplation ( int matrixSize );
int** setMatrix ( int matrixSize );
void fillMatrix ( int **matrix, int matrixSize );
float multiplyMatrixes ( int **matrix1, int **matrix2, int matrixSize);

void generateMatrixMultiplation ( int matrixSize ) {
    /* main matrix multiply organizate function */
    int **matrix1 = NULL, **matrix2 = NULL;
    int i, j;
    float time;

    matrix1 = setMatrix(matrixSize);
    fillMatrix(matrix1, matrixSize);

    matrix2 = setMatrix(matrixSize);
    fillMatrix(matrix2, matrixSize);

    time = multiplyMatrixes(matrix1, matrix2, matrixSize);
    free(matrix1);
    free(matrix2);
    printf("%-10.2f", time);
}

int** setMatrix ( int matrixSize ) {
    /* creating two dimensional array ( matrix ) with given length */
    int **matrix;
    int i;

    matrix = (int**)malloc( matrixSize * sizeof(int*) );
    for( i = 0; i < matrixSize; i++ )
        matrix[i] = (int*)malloc( matrixSize * sizeof(int));

    return matrix;
}

void fillMatrix ( int **matrix, int matrixSize ) {
    /* fills given matrix with random numbers */
    int i, j;

    for( i = 0; i < matrixSize; i++ ) {
        for( j = 0; j < matrixSize; j++ ) {
            matrix[i][j] = rand()%5;
        }
    }
}

float multiplyMatrixes ( int **matrix1, int **matrix2, int matrixSize) {
    /* multiplies two matrixes */
    clock_t start, stop;
    float total = 0;
    int i, j, k;
    int **resultMatrix = setMatrix(matrixSize);

    start = clock();
    for( i = 0; i < matrixSize; i++ ) {
        for( j = 0; j < matrixSize; j++ ) {
            resultMatrix[i][j] = 0;
            for( k = 0; k < matrixSize; k++ ) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    stop  = clock();
    total = (float)(1000*(stop-start)/CLOCKS_PER_SEC);

    free(resultMatrix);
    return total;
}
