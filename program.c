#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

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

/*	Runs the shell. First it setups the current directory to the HOME directory.
	Then it setup the interrupt handler for Ctrl-C and for children processes (
	only if SIGDET is set to 1 during compilation. The shell is then setup to
	get input from the user and makes sure the input is handled before prompting
	the user again for intput. */
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
		printf("$ ");
		success = fgets(command, INPUT_LENGTH, stdin);

		if(success == NULL){
			print_error("program.c:51");
			continue;
		}

		command[strlen(command) - 1] = '\0';
		handle_command(command);
	}

	return 0;
}

/*	Parses the command and makes sure the correct function handles
	the given command. */
void handle_command(char * command){
	char * cmd;
	char * args;

    cmd = strtok(command," "); 
    args =  strtok(NULL, "\0");

    if(cmd == NULL){
    	return;
    }

    if(strcmp(cmd, "cd") == 0){
    	if(args == NULL){
    		change_dir(home);
    	} else {
    		change_dir(args);
    	}
    } else if(strcmp(cmd, "exit") == 0){
    	exit_shell();
    } else if(strcmp(cmd, "checkEnv") == 0){
		run_checkenv(args);
    } else{
    	exec(cmd, args);
    }
}

/*	Parses the commands arguments and then makes sure that the commands
	that are not cd, exit or checkEnv are executed either as a foreground
	process or background process. */
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

/*	Executes the command in a new process that will execute in the foreground.
	The main process, the shell, will get the current time, output that a
	foreground process has started and then wait until it finished. Then it will
	get the time of termination of the foreground process and then calculate the
	time elapsed. The shell then outputs that a foreground process has terminated
	and the time it took to execute. */
void exec_foreground(char * cmd, char ** arguments){
	struct timeval start, end;
	pid_t p;

	handle_error(gettimeofday(&start, NULL), "program.c:117");

	/* Postpones the signals from children processes until sigrelse */
	handle_error(sighold(SIGCHLD), "program.c:120");

	p = fork();

	if(p == -1){
		print_error("program.c:122");
		return;
	} else if(p == 0){
		handle_error(execvp(cmd, arguments), "program.c:127");
	}

	fprintf(stderr, "Foreground process %d were started.\n", p);
	
	handle_error(waitpid(p, &status, 0), "program.c:132");
	handle_error(gettimeofday(&end, NULL), "program.c:133");
	fprintf(stderr, "Foreground process %d terminated. Time elapsed: %ld μs\n", p, timevaldiff(&start, &end));

	handle_error(sigrelse(SIGCHLD), "program.c:136");
}

/*	Executes the command in a new process that will execute in the background.
	The main process, the shell, will output that a background process have been
	started and which PID the process has. */
void exec_background(char * cmd, char ** arguments){
	pid_t p;
	p = fork();
	
	if(p == -1){
		print_error("program.c:141");
		return;
	} else if(p == 0){
		handle_error(execvp(cmd, arguments), "program.c:146");
	}

	fprintf(stderr, "Background process %d were started.\n", p);
}

/*	Exits the shell by using the command kill. This way all process that share the
	same group PID as the current process will be signaled to terminated with the
	signal SIGTERM. */
void exit_shell(){
	pid_t parent;
	parent = getpid();

	handle_error(kill(-parent, SIGTERM), "program.c:156");
}
