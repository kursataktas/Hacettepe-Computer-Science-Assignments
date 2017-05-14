#include "minion.h"


int main (int argc, char **argv) {
	char *str = argv[1];
	char *minion_name = argv[2];
	char buf[1024];
	char out_file[20];
	int result;

	/* Output file */
	FILE *w = fopen(strcat(out_file, strcat(minion_name, ".out")), "w");
	fprintf(w, "Search for \"%s\" starting on %s\n", str, minion_name);
	fprintf(w, "----------------------\n");
	fflush(w);
	
	/* Output Pipe */
	FILE *pp = fdopen(1, "w");

	while(1) {

		memset(buf, 0, 1024);
		if((result = read(0, &buf, 255))) {

			FILE *r = fopen(buf, "r");
			char temp[1024] = {0x0};
			int line = 0, col = 0;

			/* Read from pipe */
			while(r != NULL && fgets(temp, sizeof(temp), r) != NULL) {

				/* clean new line and other stupid characters */
				temp[strcspn(temp, "\r\n")] = 0;

				char *sub_string = strcasestr(temp, str);
				if(sub_string != NULL) {
					col = sub_string - temp + 1;

					/* Write to file */
					fprintf(w, "%s: %s:%d:%d\n", minion_name, buf, line, col);
					fflush(w);

					/* Check program is finitoo */
					if(strcmp(buf, "finito") == 0) {
						if(w != NULL)
							fclose(w);
						return 0;
					}

					/* Send logs to controller via pipe */
					fprintf(pp, "%s: %s:%d:%d\n", minion_name, buf, line, col);
					fflush(pp);

					usleep(200);
				}
				line++; 
			}
		}
	}

	
}