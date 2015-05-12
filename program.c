#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include "cd.h"
#include "checkenv.h"
#include "helper.h"

#define INPUT_LENGTH 80
#define WRITE 1
#define READ 0

void handle_command(char * command);
void func();
void sig_handler(int signo);
void exec_foreground(char * cmd, char * args);

char * home;
int status;
pid_t child;

int main(){
	char command[INPUT_LENGTH];
	char * success;

	atexit(func);

	home = getenv("HOME");
	change_dir(home);
	
	while(1){
		print_current_directory();
		printf("> ");
		success = fgets(command, INPUT_LENGTH, stdin);

		/* A problem a first */
		command[strlen(command) - 1] = '\0';

		if(success == NULL){
			print_error("The line was not scanned properly");
			continue;
		}

		handle_command(command);
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
    	exit(EXIT_SUCCESS);
    } else if(strcmp(cmd, "checkEnv") == 0){
		run_checkenv(args);
    } else{
    	exec_foreground(cmd, args);
    }
}

void exec_foreground(char * cmd, char * args){
	child = fork();
	if(child == -1){
		print_error();
	} else if(child == 0){
		handle_error(execvp(cmd, handle_args(cmd, args)));
	}

	wait(&status);

}

/* Handles termination of processes */
void func(){
	printf("Exiting\n");
}
