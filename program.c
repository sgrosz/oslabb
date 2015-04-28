#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "cd.h"
#include "pipes.h"
#include "error.h"

#define INPUT_LENGTH 80
#define WRITE 1
#define READ 0

void handle_command(char * command);
void func();

char * home;
int end;
int status;
pid_t t;

int main(){
	char command[INPUT_LENGTH];
	char * success;
	end = 0;

	atexit(func);

	home = getenv("HOME");
	change_dir(home);
	
	while(1){
		print_current_directory();
		printf("> ");
		success = fgets(command, INPUT_LENGTH, stdin);

		/* A problem a first */
		command[strlen(command) - 1] = '\0';

		if(strcmp(success,"") == 0 || success == NULL){
			print_special_error("The line was not scanned properly");
			continue;
		}

		handle_command(command);

		if(end){
			exit(0);
		}
	}

	return 0;
}

/* Handles the command given in the prompt */
void handle_command(char * command){
	char * cmd;
	char * args;

    cmd = strtok(command," "); 
    args =  strtok(NULL, "\0");

    if(strcmp(cmd, "cd") == 0){
    	if(args == NULL){
    		change_dir(home);
    	} else {
    		change_dir(args);
    	}
    } else if(strcmp(cmd, "exit") == 0){
    	end = 1;
    } else if(strcmp(cmd, "checkEnv") == 0){
		run_checkenv(args);
    }
}

/* Handles termination of processes */
void func(){
	printf("Exiting\n");
}
