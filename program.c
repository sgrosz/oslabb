#include <stdio.h>
#include <stdlib.h>
#include "cd.h"

void handle_command(char * command);

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

		/* A problem a first */
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
    	printf("%s", cmd);
    	args = strtok(NULL, " ");

    	if(args == NULL){
    		change_dir(home);
    	} else {
    		change_dir(args);
    	}
    } else if(strcmp(cmd, "exit") == 0){
    	printf("%s\n", cmd);
    }
}
