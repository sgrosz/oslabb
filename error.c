#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void print_error();
void print_special_error(char * arg);
void handle_error();

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
