#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void print_error();
void print_special_error(char * arg);
void handle_error();
char ** handle_args(char * cmd, char * args);
long timevaldiff(struct timeval * start, struct timeval * end);

char * arg_array[10];

void print_error(){
	fprintf(stderr, "Error: %s\n", strerror(errno));
}

void print_special_error(char * arg){
	fprintf(stderr, "Error: %s\n", arg);
}

void handle_error(int err){
	if(err == -1){
		print_error();
		exit(EXIT_FAILURE);
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
		arg = strtok (NULL, " ");
	}
	arg_array[i++] = NULL;

	return arg_array;
}

long timevaldiff(struct timeval * start, struct timeval * end){
  long usec;

  usec=(end->tv_sec - start->tv_sec) * 1000000;
  usec+=(end->tv_usec - start->tv_usec);

  return usec;
}
