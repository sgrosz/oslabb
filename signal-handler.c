#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cd.h"
#include "signal-handler.h"
#include "helper.h"

/*	Checks if a child process have terminated, if so then the shell will output
	that a background process have terminated and the PID of that process. This
	will only be checked once since WNOHANG is set in the waitpid. */
void background_terminated(){
	pid_t p;
	int status;

	if((p = waitpid(-1, &status, WNOHANG)) > 0){
		fprintf(stderr, "Background process %d terminated\n", p);
	}
}

/*	Setups the signal handler for the signal SIGCHLD that are sent from child
	process to its parent when a status has changed for the child. Defines with
	SA_RESTART the exection of the child process will restart. If there a SIGCHLD
	signal has been sent the function background_terminated() will run. */
void setup_child_handler(){
	/* Establish handler. */
	struct sigaction child;
	child.sa_handler = &background_terminated;
	child.sa_flags = SA_RESTART;
	handle_error(sigemptyset(&child.sa_mask), "signal-handler.c:26");

	handle_error(sigaction(SIGCHLD, &child, 0), "signal-handler.c:28");
}

/*	Setups the signal handler for the signal SIGINT that are sent from the
	process where it is setup whenever Ctrl-C is used. The signal handler
	is set with SA_RESTART to make sure that the process restart if a system
	call is interrupted. If a SIGINT is sent from where it is setup then the
	function interrupt_handler() will run. */
void setup_interrupt_handler(){
	/* Establish handler. */
	struct sigaction interrupt;
	interrupt.sa_handler = &interrupt_handler;
	interrupt.sa_flags = SA_RESTART;
	handle_error(sigemptyset(&interrupt.sa_mask), "signal-handler.c:36");

	handle_error(sigaction(SIGINT, &interrupt, 0), "signal-handler.c:38");
}

/*	If there have been an interrupt then the shell will output the prompt again. */
void interrupt_handler(int signum){
	printf("\n");
	print_current_directory();
	printf("$ ");

	if(fflush(stdout) == EOF){
		print_error("signal-hander.c:47");
	}
}
