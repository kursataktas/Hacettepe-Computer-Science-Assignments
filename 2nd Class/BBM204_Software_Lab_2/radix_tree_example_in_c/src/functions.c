#include "header.h"

void createRadixTree (char *path, radixPtr *root) {
  DIR *dir_ptr;
  struct dirent *dir_struct;
  stats file_info;
  char *name, *new_directory;
  radixPtr new_node;

  dir_ptr = opendir(path);
  if (dir_ptr == NULL) {
    printf("Cannot open directory '%s' \n", path);
    return;
  }

  while( (dir_struct = readdir(dir_ptr)) != NULL ){
    name = dir_struct->d_name;
    if (strcmp(name,".") != 0 && strcmp(name,"..") != 0){
      file_info = getStats(name);
      new_node = insertRadixTree(root, file_info);

      if (file_info.type == 0) {
        new_directory = changeDirectory(file_info.name);
        createRadixTree(new_directory,  &new_node->link[47]);
        path = changeDirectory(path);
      }
    }
  }
  closedir(dir_ptr);
}

stats getStats (char *path) {
   struct stat sb;
   stats file_info;

   if (stat(path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

   switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:  file_info.type = DIRECTORY;          break;
    case S_IFREG:  file_info.type = REGULAR;            break;
   }

   file_info.name = path;
   file_info.permission = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
   return file_info;
}

char* changeDirectory (char* target) {
  char *directory = (char *)malloc(1024);

  getcwd(directory, 1024);

  if(chdir(target) ==  0) {
    getcwd(directory, 1024);
    return directory;
  }

  else {
    perror("change directory");
    exit(EXIT_FAILURE);
  } 
}

radixPtr createRadixNode () {
  radixPtr radix_node = (radixPtr)malloc(sizeof(struct radixNode));
  int i;

  if(radix_node == NULL) {
    perror("There is no memory to create radix node");
    exit(1);
  }

  radix_node->end = -1;
  for (i = 0; i < ASCII_SIZE; ++i) {
      radix_node->link[i] = NULL;
  }
  return radix_node;
}

radixPtr insertRadixTree (radixPtr *root, stats file_info) {
  int level, index;
  radixPtr q;
  
  if ( *root == NULL) 
    *root = createRadixNode();
  q = *root;

  for (level = 0; level < strlen(file_info.name); level++) {
    index = (int)file_info.name[level];

    if( q->link[index] == NULL )
      q->link[index] = createRadixNode();
    q = q->link[index];
  }

  q->end = level;
  q->permission = file_info.permission;
  q->type = file_info.type;
  return q;
}

void writeRadixTree (radixPtr root, char x[MAX_NAME], FILE *db_ptr) {
  int index = 0;
  radixPtr q = root;
  char y;

  if ( root->end != -1 ) 
    fprintf(db_ptr,"%s %d %o %d\n", x, root->type, root->permission, root->end);

  for (; index < ASCII_SIZE; ++index) {
    if ( q->link[index] != NULL ) {
      y = (char)index;
      strncat(x, &y, 1);
      writeRadixTree(q->link[index], x, db_ptr);
      x[strlen(x)-1] = '\0';
    }
  }
  return;
}

void setRadixFromDb (radixPtr *root, FILE *db_ptr) {
	char buff[1024], *delimeter = " ";
	stats file_info;
	int i;

	for(i = 0; fgets(buff, 1024, db_ptr) != NULL; i++) {
		file_info.name = strtok(buff, delimeter);
		file_info.type = (int)strtol(strtok(NULL,delimeter), NULL, 10);
		file_info.permission = strtol(strtok(NULL,delimeter), NULL, 8);
		insertRadixTree(root, file_info);
	}
  fclose(db_ptr);
}

void searchRadixTree(radixPtr root_radix, int i, int p, int only_directory, int only_file, int ignore_case, char* pattern) {

}
