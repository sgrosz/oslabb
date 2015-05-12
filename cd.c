#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "helper.h"

#define INPUT_LENGTH 80

void print_current_directory();
void change_dir(char * path);

/* Gets the current working directory and prints it */
void print_current_directory(){
	char * path_ptr;
	char path[INPUT_LENGTH];

	path_ptr = getcwd(path, INPUT_LENGTH);

	if(path_ptr == NULL){
        print_error();
    }
    printf("%s", path_ptr);
}

/* Changes the current directory with the path param */
void change_dir(char * path){
	int ok;
	ok = chdir(path);

	if(ok != 0){
		print_error();
	}
}
