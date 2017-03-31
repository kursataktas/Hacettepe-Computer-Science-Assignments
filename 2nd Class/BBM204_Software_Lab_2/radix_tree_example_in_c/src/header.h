#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASCII_SIZE 256
#define MAX_NAME 255
#define DEFAULT_DB "radixT.db"

typedef enum {DIRECTORY, REGULAR} fileType;

typedef struct fileInfo stats;
typedef struct fileInfo *statPtr;
struct fileInfo {
	char *name;
	fileType type; 		
	int permission;
};

typedef struct radixNode *radixPtr;
struct radixNode{
	int permission;
	int end;
	fileType type;
	radixPtr link[ASCII_SIZE];
};


stats getStats (char *path);
void createRadixTree (char *path, radixPtr *root);
char* changeDirectory (char* target);
radixPtr createRadixNode (void);
radixPtr insertRadixTree (radixPtr *root, stats file_info);
void writeRadixTree (radixPtr root, char x[MAX_NAME], FILE *db_ptr);
void setRadixFromDb (radixPtr *root, FILE *db_ptr);
void searchRadixTree(radixPtr root_radix, int i, int permission, int only_directory, int only_file, int ignore_case, char* pattern);
