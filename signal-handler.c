#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cd.h"
#include "signal-handler.h"

void background_terminated(){
	pid_t p;
	int status;

	if((p = waitpid(-1, &status, WNOHANG)) > 0){
		printf("Background process %d terminated\n", p);
	}
}

void setup_child_handler(){
	/* Establish handler. */
	struct sigaction child;
	child.sa_handler = &background_terminated;
	child.sa_flags = SA_RESTART;
	sigemptyset(&child.sa_mask);

	sigaction(SIGCHLD, &child, 0);
}

void setup_interrupt_handler(){
	/* Establish handler. */
	struct sigaction interrupt;
	interrupt.sa_handler = &interrupt_handler;
	interrupt.sa_flags = SA_RESTART;
	sigemptyset(&interrupt.sa_mask);

	sigaction(SIGINT, &interrupt, 0);
}

void interrupt_handler(int signum){
	/*DO NOTHING*/

	fflush(stdin);

	printf("\n");
	print_current_directory();
	printf("> ");

	fflush(stdout);
}
