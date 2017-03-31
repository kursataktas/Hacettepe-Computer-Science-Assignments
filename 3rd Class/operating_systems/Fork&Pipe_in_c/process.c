#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void multiply_matrices();
void fetch_matrix();
void write_matrix();
void free_matrix();

int main (int argc, char **argv) {
	int n, fd[2], pid, **temp, result, i, j, k;
	long long **m;
	char *end;
	int child_count;

	if(argc < 2) {
		printf("Usage: ./process child_count \n");
		return 0;
	}

	child_count = (int)strtol(argv[1], &end, 10);
	fetch_matrix(&m, &n);
	pipe(fd);
	
	/*Write matrix to the pipe for first child*/
	for (i = 0; i < n; ++i) 
		for (j = 0; j < n; ++j)
			if((result = write(fd[1], &m[i][j], sizeof(long long))) < 0) 
				fprintf(stderr, "Bad things happened!\n");

	for(k = 0; k < child_count; k++) {

		/*Wait all children*/
		if((pid = fork()) > 0 ) {
			wait(NULL);
		}

		/*Child*/
		else {
			/*Read matrix from pipe*/
			for (i = 0; i < n; ++i) 
				for (j = 0; j < n; ++j)
					if((result = read(fd[0], &m[i][j], sizeof(long long))) < 0) 
						fprintf(stderr, "Bad things happened!\n");
			
			/*Calculate square of matrix*/
			multiply_matrices(m, n, &m);

			/*Output to Screen*/
			write_matrix(m, n, k + 1, stdout);

			/* Output to File*/
			write_matrix(m, n, k + 1, NULL);

			/*Write new matrix to the pipe*/
			for (i = 0; i < n; ++i) 
				for (j = 0; j < n; ++j)
					if((result = write(fd[1], &m[i][j], sizeof(long long))) < 0) 
						fprintf(stderr, "Bad things happened!\n");

			/*Close file descriptors whichs points to pipe ends and terminate child*/
			close(fd[0]);
			close(fd[1]);
			return 0;
		}
	}

	/*Read current matrix from LAST CHILD */
	for (i = 0; i < n; ++i) 
		for (j = 0; j < n; ++j)
			if((result = read(fd[0], &m[i][j], sizeof(long long))) < 0) 
				fprintf(stderr, "Bad things happened!\n");

	/*Close file descriptors whichs points to pipe ends*/
	close(fd[0]);
	close(fd[1]);
	free_matrix(m, n);
}

/**
* Multiplies given matrix with itself and write new matrix to 3rd argument
*@param m -> matrix 
*@param n -> column or row length of matrix
*@param m_result -> address of the two dimensional matrix. return of the operation will be loaded into this argument
*/
void multiply_matrices(long long **m, int n, long long ***m_result) {
	int i,j,k;
	long long sum = 0;
	long long **m_square = malloc(n * sizeof(long long *));

	for(i = 0; i<n; ++i)
		m_square[i] = malloc(n * sizeof(long long *));

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			for (k = 0; k < n; ++k) {
				sum += (m[i][k] * m[k][j]);
			}
			m_square[i][j] = sum;
			sum = 0;
		}
	}

	*m_result = m_square;
}

/**
* Fetches matrix from file 
*/
void fetch_matrix(long long ***m, int *n) {
	FILE *reader = fopen("matrix.txt", "r");
	char *buff = malloc(100 * sizeof(char));

	if(reader == NULL) {
		printf("File couldn't open");
		exit(-1);
	}

	char *cell, *end;
	int i;
	for(i=-1; fgets(buff, 100, reader) != NULL; i++){
		
		/* clean new line and other stupid characters */
		buff[strcspn(buff, "\r\n")] = 0;

		cell = strtok(buff, ",");
		
		if(i == -1) {
			*n = (int)strtol(cell, &end, 10);
			*m = malloc((*n) * sizeof(long long *));

			int k;
			for(k = 0; k < *n; k++)
				(*m)[k] = malloc((*n) * sizeof(long long *));

			continue;
		}

		int j = 0;
		while(cell != NULL) {
			(*m)[i][j] = (long long)strtol(cell, &end, 10);
			cell = strtok(NULL, ",");
			j++;
		}
	}

	free(buff);
	fclose(reader);
}

/**
* Writes content of matrix to output file. 
*@param m -> two dimensional matrix
*@param n -> column or row length of matrix
*@param process_number -> child number of current process
*@param s -> stdout for standart output, NULL for file
*/ 
void write_matrix(long long **m, int n, int process_number, FILE *s) {
	int i,j;

	if ( s == NULL ) {
		char buf[10];
		sprintf(buf, "%d.txt", process_number);
		s = fopen(buf, "w+");
	}
	
	fprintf(s, "Process-%d %d\n", process_number, getpid());
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if(j != 0)
				fprintf(s, "\t");
			fprintf(s, "%lld", m[i][j]);
		}
		fprintf(s, "\n");
	}

	if(s == NULL)
		fclose(s);
}

void free_matrix(long long **m, int n) {
	int i;

	for (i = 0; i < n; ++i) 
		free(m[i]);
	free(m);
}