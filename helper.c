#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helper.h"

char * arg_array[10];

/*	Prints an error message with an message. The message will contain
	information on where the error occured. The error message outputed
	may look like this: "cd.c:29: No such file or directory". The first part
	specifies which file the error occured in, the second the line of code
	which genereated the error and the third what type of error occured. */
void print_error(char * message){
	perror(message);
}

/*	Checks if the int err given from the parameter is -1, if so then it will
	print the error with the given message. */
void handle_error(int err, char * message){
	if(err == -1){
		print_error(message);
	}
}

/*	Returns an array containing the command and all of its arguments. */
char ** handle_args(char * cmd, char * args){
	int i;
	char * arg;
	
	/* Splits the arguments into an array of arguments */
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

/*	Returns the time difference in microseconds between the two timeval structs. */
long timevaldiff(struct timeval * start, struct timeval * end){
  long usec;

  usec = (end->tv_sec - start->tv_sec) * 1000000;
  usec += (end->tv_usec - start->tv_usec);

  return usec;
}
