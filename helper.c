#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helper.h"

char * arg_array[10];

void print_error(char * message){
	perror(message);
}

void handle_error(int err, char * message){
	if(err == -1){
		print_error(message);
	}
}

char ** handle_args(char * cmd, char * args){
	int i;
	char * arg;
	
	/* Splits the arguments into an argument array*/
	i = 1;
	arg_array[0] = cmd;
	arg = strtok(args ," "); 

	while (arg != NULL){
		arg_array[i++] = arg;
		arg = strtok(NULL, " ");
	}
	arg_array[i++] = NULL;

	return arg_array;
}

long timevaldiff(struct timeval * start, struct timeval * end){
  long usec;

  usec = (end->tv_sec - start->tv_sec) * 1000000;
  usec += (end->tv_usec - start->tv_usec);

  return usec;
}
