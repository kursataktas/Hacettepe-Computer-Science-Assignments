#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Mutex for file_buffer */
pthread_mutex_t mutex;

/* Conditional mutexes for consumer and procuders of file_buffer */
pthread_cond_t condc, condp;

/* Mutex for log file */
pthread_mutex_t mutex_log;

/* The buffer */
char  **file_buffer;

/* Command line arguments */
int number_of_minions = 0, size_of_buffer = 0;

/* Fifo head and tail variables */
int head = 0, tail = 0;

/* For ending the program */
int finish = 0;

/* For minion naming */
int minion_count = 0;

/* File pointer to log file */
FILE *lf;

/* The producer */
void file_searcher();

/* The consumer */
void controller();

/**
* Pushes new txt file to FIFO buffer array
*/ 
int push(char* str) {
	tail = (tail + 1) % size_of_buffer;

	free(file_buffer[tail]);
	file_buffer[tail] = malloc(((unsigned)strlen(str)) * sizeof(char *));
	strcpy(file_buffer[tail], str);
	puts(file_buffer[tail]);
	return 1;
}

/**
* Pops txt file path from FIFO buffer
*/
char* pop() {
	if(head == tail) 
		return NULL;

	head = (head + 1) % size_of_buffer;
	return file_buffer[head];
}

/**
* Checks if FIFO is full
*/ 
int is_full() {
	if(head == ((tail + 1) % size_of_buffer) )
		return 1;
	return -1;
}

/**
* Checks if fifo is empty
*/
int is_empty() {
	if(head == tail)
		return 1;
	return -1;
}