#include "header.h"

int main (int argc, char *argv[]) {
 	int i, ignore_case = 0, only_file = 0, only_directory = 0;
    int permission = 0;
 	char *db = DEFAULT_DB, *pattern = "";
 	FILE *db_ptr;
 	radixPtr root_radix = NULL;

 	if ( argc < 2 ) {
 		printf("Usage: ./locater [OPTION] pattern\n");   
 		return 1;
 	}
 	for (i = 1; i < argc; ++i) {
 		if ( (strcmp(argv[i],"-i") == 0) || (strcmp(argv[i],"--ignore-case") == 0) ) 
 			ignore_case = 1;
 		else if( (strcmp(argv[i],"-f") == 0) || (strcmp(argv[i],"--file") == 0) )
 			only_file = 1;
 		else if( (strcmp(argv[i],"-db") == 0) || (strcmp(argv[i],"--databasefile") == 0) ) {
 			db = argv[i+1];
 			i++;
 		}
 		else if( (strcmp(argv[i],"-d") == 0) || (strcmp(argv[i],"--directory") == 0) )
 			only_directory = 1;
 		else if( (strcmp(argv[i],"-p") == 0) ) {
 			permission = strtol(argv[i+1], NULL, 8);
 			i++;
 		}
 		else
 			pattern = argv[i];
 	}
  
 	db_ptr = fopen(db, "r");
 	setRadixFromDb(&root_radix, db_ptr);	db_ptr = fopen(db, "r");
 	searchRadixTree(root_radix, i, permission, only_directory, only_file, ignore_case, pattern);
    return 0;
}