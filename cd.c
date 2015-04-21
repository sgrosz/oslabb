#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void print_current_directory();
void change_dir(char * path);

/* Gets the current working directory and prints it */
void print_current_directory(){
	char * path_ptr;
	char path[INPUT_LENGTH];

	path_ptr = getcwd(path, INPUT_LENGTH);

	if(path_ptr == NULL){
        fprintf(stderr, "Error: %s\n", strerror(errno));
    }

    printf("%s", path_ptr);
}

/* Changes the current directory with the path param */
void change_dir(char * path){
	int ok;
	ok = chdir(path);

	if(ok != 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}
}
