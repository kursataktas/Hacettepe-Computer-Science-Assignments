#include "main.h"


int main (int argc, char **argv) {
	char *end, *search_string = "", *path = "";
	pthread_t searcher, minions[50];
	int i;

	/*Invalid parameter count.*/
	if(argc < 4) {
		printf("Usage: ./main <number of minions> <size of file buffer (# of elements)> <search string> <search path> \n");
		return 0;
	}

	/* get arguments */
	number_of_minions = (int)strtol(argv[1], &end, 10);
	size_of_buffer = (int)strtol(argv[2], &end, 10);

	search_string = malloc(((unsigned)strlen(argv[3])) * sizeof(char *));
	strncpy(search_string, argv[3], (unsigned)strlen(argv[3]));

	path = malloc(((unsigned)strlen(argv[4])) * sizeof(char *));
	strncpy(path, argv[4], (unsigned)strlen(argv[4]));


	/* Construct File buffer */
	file_buffer = malloc(size_of_buffer * sizeof(char *));

	/* Open and initialize log file */
	lf = fopen("searchlog.txt", "w");
	fprintf(lf, "%s\n", "Log File");
	fprintf(lf, "Search for \"%s\" string\n", search_string);
	fprintf(lf, "----------------------\n");
	fflush(lf);

	/* Initialize mutex and conditional mutexes */
	pthread_mutex_init(&mutex, NULL);	
	pthread_mutex_init(&mutex_log, NULL);	
	pthread_cond_init(&condc, NULL);		
	pthread_cond_init(&condp, NULL);		

	pthread_create(&searcher, NULL, file_searcher, (void *)path);
	
	for(i = 0; i < number_of_minions; i++) 
		pthread_create(&minions[i], NULL, controller, (void *)search_string);

	pthread_join(searcher, NULL);
	for(i = 0; i < number_of_minions; i++) 
		pthread_join(minions[i], NULL);
	
	/* Destroy all mutexes */
	pthread_mutex_destroy(&mutex);	
	pthread_mutex_destroy(&mutex_log);	
	pthread_cond_destroy(&condc);		
	pthread_cond_destroy(&condp);

	return 0;
}

/**
* Producer which search given directory and saves the .txt path to file_buffer
* @param path, .txt file path for adding to file buffer
*/
void file_searcher(char* path) {
	DIR *dir_ptr;
	struct dirent *dir_struct;

	dir_ptr = opendir (path);
	if (dir_ptr != NULL){

		while (dir_struct = readdir (dir_ptr)) {

			/* Lock the buffer */
			pthread_mutex_lock(&mutex);

			/* Critical Section */

			/* If buffer is full, then wait */
			while(is_full() == 1) 
				pthread_cond_wait(&condp, &mutex);
			
			if (strstr(dir_struct->d_name, ".txt") != NULL) {
				char *full_path = malloc((strlen(path) + strlen("/") + strlen(dir_struct->d_name)) * sizeof(char));
				strcat(strcat(strcat(full_path, path), "/"), dir_struct->d_name);
				push(full_path);
			}

			/* Critical Section End */

			/* release file_buffer mutexes */
			pthread_cond_signal(&condc);
			pthread_mutex_unlock(&mutex);
		}
	  	(void) closedir (dir_struct);
	}
	else
		perror ("Couldn't open the directory");

	finish = 1;
	pthread_exit(0);
}

/* 
* File buffer Consumer 
* @param str, seach string
*/
void controller(char* str) {
	int fd_path_pipe[2], fd_log_pipe[2];
	int pid, result;
	char buf[1024];
	char minion_name[8];

	sprintf(minion_name, "minion%d", ++minion_count);

	/* Controller sends data to minion via this pipe */
	pipe(fd_path_pipe);

	/* Minion sends data to controller via this pipe */
	pipe(fd_log_pipe);

	/* Parent */
	if((pid = fork()) > 0) {
		
		/* Don't need this ends of pipes */
		close(fd_path_pipe[0]);
		close(fd_log_pipe[1]);

		while(1) {
			/* Lock the buffer */
			pthread_mutex_lock(&mutex);

			/* CRITICAL SECTION */
			
			/* If buffer is empty, then wait */
			while(is_empty() == 1) {
				
				/* 
				current thread job is done. kill the child process for this controller, release the file buffer mutex 
				and finally kill this controller thread 
				 */
				if(finish == 1){
					write(fd_path_pipe[1], "finito", strlen("finito"));
					pthread_mutex_unlock(&mutex);
					pthread_exit(0);
				}

				pthread_cond_wait(&condc, &mutex);
			}

			char *search_path = pop();

			/* CRITICAL SECTION END */

			/* release file_buffer mutexes */
			pthread_cond_signal(&condp);
			pthread_mutex_unlock(&mutex);

			write(fd_path_pipe[1], search_path, strlen(search_path));
			
			while(1) {
				memset(buf, 0, 1024);
				if((result = read(fd_log_pipe[0], &buf, 1024 * sizeof(char)))) {

					/* Lock the log file */
					pthread_mutex_lock(&mutex_log);

					/* Critical Section */
					
					/* Write to log file */
					fprintf(lf, "%s", buf);
					fflush(lf);

					/* Critical Section End */

					/* release log mutex */
					pthread_mutex_unlock(&mutex_log);

					usleep(200);
					break;
				}
			}
			sleep(1);
		}
	}

	/* Child ( Minion )*/
	else {
		/* Duplicate pipe end with standart input and outputs */
		dup2(fd_path_pipe[0], STDIN_FILENO);
		dup2(fd_log_pipe[1], STDOUT_FILENO);

		/* Don't need this ends of pipes in child process */
		close(fd_path_pipe[1]);
		close(fd_log_pipe[0]);

		char *newargv[] = { "./minion", str, minion_name, NULL};
		execvp(newargv[0], newargv);
	}
}