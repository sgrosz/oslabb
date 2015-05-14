#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cd.h"
#include "signal-handler.h"
#include "helper.h"

void background_terminated(){
	pid_t p;
	int status;

	if((p = waitpid(-1, &status, WNOHANG)) > 0){
		fprintf(stderr, "Background process %d terminated\n", p);
	}
}

void setup_child_handler(){
	/* Establish handler. */
	struct sigaction child;
	child.sa_handler = &background_terminated;
	child.sa_flags = SA_RESTART;
	handle_error(sigemptyset(&child.sa_mask), "signal-handler.c:26");

	handle_error(sigaction(SIGCHLD, &child, 0), "signal-handler.c:28");
}

void setup_interrupt_handler(){
	/* Establish handler. */
	struct sigaction interrupt;
	interrupt.sa_handler = &interrupt_handler;
	interrupt.sa_flags = SA_RESTART;
	handle_error(sigemptyset(&interrupt.sa_mask), "signal-handler.c:36");

	handle_error(sigaction(SIGINT, &interrupt, 0), "signal-handler.c:38");
}

void interrupt_handler(int signum){
	/*DO NOTHING*/
	printf("\n");
	print_current_directory();
	printf("$ ");

	if(fflush(stdout) == EOF){
		print_error("signal-hander.c:47");
	}
}
