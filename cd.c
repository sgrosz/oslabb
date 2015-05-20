#include <stdio.h>
#include <unistd.h>

#include "helper.h"
#include "cd.h"

#define INPUT_LENGTH 80

void print_current_directory();
void change_dir(char * path);

/* Gets the current working directory and prints it */
void print_current_directory(){
	char * path_ptr;
	char path[INPUT_LENGTH];

	path_ptr = getcwd(path, INPUT_LENGTH);

	if(path_ptr == NULL){
        print_error("cd.c:17");
    }
    printf("%s", path_ptr);
}

/* Changes the current directory with the path param */
void change_dir(char * path){
	handle_error(chdir(path), "cd.c:27");
}
