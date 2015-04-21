#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define INPUT_LENGTH 80
#define MAX_ARGS 4

void handle_command(char * command);
void print_current_directory();
void change_dir(char * path);

char * home;

int main(){
	char command[INPUT_LENGTH];
	char * success;

	home = getenv("HOME");
	change_dir(home);
	
	while(1){
		print_current_directory();
		printf("$ ");
		success = fgets(command, INPUT_LENGTH, stdin);
		command[strlen(command) - 1] = '\0';

		if(success == NULL){
			printf("The line was not scanned properly.\n");
		}

		handle_command(command);
	}
}

/* Handles the command given in the prompt */
void handle_command(char * command){
	char * cmd;
	char * args;

    cmd = strtok(command," "); 

    if(strcmp(cmd, "cd") == 0){
    	args = strtok(NULL, " ");

    	if(args == NULL){
    		change_dir(home);
    	} else {
    		change_dir(args);
    	}
    }
}

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
