#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cd.h"
#include "checkenv.h"
#include "helper.h"
#include "signal-handler.h"
#include "program.h"

#ifdef SIGDET
#define SIGHANDLER SIGDET
#endif

#ifndef SIGDET
#define SIGHANDLER 0
#endif

#define INPUT_LENGTH 80

char * home;
int status;

int main(){
	char command[INPUT_LENGTH];
	char * success;

	home = getenv("HOME");
	change_dir(home);

	setup_interrupt_handler();

	if(SIGHANDLER){
		setup_child_handler();
	}
	
	while(1){

		if(!SIGHANDLER){
			background_terminated();
		}

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
	/*sigset_t block, empty;*/
	struct timeval start, end;
	pid_t p;

	gettimeofday(&start, NULL);

	/* Defers the signals from children processes until sigrelse */
	sighold(SIGCHLD);

	p = fork();

	if(p == -1){
		print_error();
	} else if(p == 0){
		handle_error(execvp(cmd, arguments));
	}

	printf("Foreground process %d were started.\n", p);
	
	handle_error(waitpid(p, &status, 0));
	handle_error(gettimeofday(&end, NULL));
	printf("Foreground process %d terminated. Time elapsed: %ld μs\n", p, timevaldiff(&start, &end));

	sigrelse(SIGCHLD);
}

void exec_background(char * cmd, char ** arguments){
	pid_t p;
	p = fork();
	
	if(p == -1){
		print_error();
	} else if(p == 0){
		handle_error(execvp(cmd, arguments));
	}

	printf("Background process %d were started.\n", p);
}
