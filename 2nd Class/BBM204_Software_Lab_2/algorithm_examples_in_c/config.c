/* This file includes all headers,necessary C files and defined terms */

#define maxArrSize 2500
#define repeatTime 5000
#define size 10
#define TRUE 1
#define FALSE 0
#define SWAP(x,y,t) ((t)=(x), (x)=(y), (y)=(t))

# if !defined __STRICT_ANSI__ && !defined __USE_XOPEN2K
#  ifndef CLK_TCK
#   define CLK_TCK      CLOCKS_PER_SEC
#  endif
# endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "matrixMultiplication.c"
#include "bubbleSort.c"
#include "quickSort.c"
#include "binarySearch.c"
#include "findingMaxElement.c"

