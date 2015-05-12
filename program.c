#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include "cd.h"
#include "checkenv.h"
#include "helper.h"

#define INPUT_LENGTH 80
#define WRITE 1
#define READ 0

void handle_command(char * command);
void func();
void sig_handler(int signo);
void exec_foreground(char * cmd, char ** arguments);
void exec_background(char * cmd, char ** arguments);
void exec(char * cmd, char * args);
void terminated_polling();

char * home;
int status;

int main(){
	char command[INPUT_LENGTH];
	char * success;

	atexit(func);

	home = getenv("HOME");
	change_dir(home);
	
	while(1){
		terminated_polling();

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
    	exec(cmd, args);
    }
}

void exec(char * cmd, char * args){
	char ** arguments;
	int arg_number = 0;

	arguments = handle_args(cmd, args);

	while(arguments[arg_number] != NULL){
		arg_number++;
	}

	if(strcmp("&", arguments[arg_number - 1]) == 0){
		arguments[arg_number - 1] = NULL;
		exec_background(cmd, arguments);
	}else{
		exec_foreground(cmd, arguments);
	}
}

void exec_foreground(char * cmd, char ** arguments){
	struct timeval start, end;
	pid_t p;

	gettimeofday(&start, NULL);
	p = fork();

	if(p == -1){
		print_error();
	} else if(p == 0){
		handle_error(execvp(cmd, arguments));
	}

	printf("%d: Foreground process were started.\n", p);
	
	handle_error(waitpid(p, &status, 0));
	handle_error(gettimeofday(&end, NULL));
	printf("Foreground process %d terminated. Time elapsed: %ld μs\n", p, timevaldiff(&start, &end));
}

void exec_background(char * cmd, char ** arguments){
	pid_t p;
	p = fork();
	
	if(p == -1){
		print_error();
	} else if(p == 0){
		handle_error(execvp(cmd, arguments));
	}

	printf("%d: Background process were started.\n", p);
}

void terminated_polling(){
	pid_t p;
	int status;

	if((p = waitpid(-1, &status, WNOHANG)) > 0){
		printf("Background process %d terminated\n", p);
	}
}

/* Handles termination of processes */
void func(){
	printf("Exiting\n");
}
