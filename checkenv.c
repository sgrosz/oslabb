#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "error.h"

#define WRITE 1
#define READ 0

void run_checkenv(char * args);
void run_pager();
void run_printenv();
void run_sort();
void run_grep(char * args);

int p1[2], p2[2], p3[2];
pid_t child;
int status;


void run_checkenv(char * args){
	if(pipe(p1) == -1){
		print_error();
		return;
	}

	if(pipe(p2) == -1){
		print_error();
		return;
	}

	if(pipe(p3) == -1){
		print_error();
		return;
	}

	run_printenv();
	run_grep(args);
	run_sort();
	run_pager();

}

void run_printenv(){
	child = fork();

	if(child == -1){
		print_error();
		return;
	}else if(child == 0){
		handle_error(close(p1[READ]));
		handle_error(dup2(p1[WRITE], WRITE));

		handle_error(execlp("printenv", "printenv", NULL));
	}

	handle_error(close(p1[WRITE]));
	handle_error(wait(&status));
}

void run_grep(char * args){
	int i;
	char * arg;
	char * arg_array[10];
	
	/* Splits the arguments into an argument array*/
	i = 1;
	arg_array[0] = "grep";
	arg = strtok(args ," "); 

	while (arg != NULL){
		arg_array[i++] = arg;
		arg = strtok (NULL, " ");
	}
	arg_array[i++] = NULL;

	child = fork();

	if(child == -1){
		print_error();
		return;
	}else if(child == 0){
		handle_error(dup2(p1[READ], READ));

		handle_error(close(p2[READ]));
		handle_error(dup2(p2[WRITE], WRITE));

		if(args == NULL){
			handle_error(execlp("cat", "cat", NULL));
		} else{
			handle_error(execvp("grep", arg_array));
		}
	}

	handle_error(close(p1[READ]));
	handle_error(close(p2[WRITE]));
	handle_error(wait(&status));

}

void run_sort(){
	child = fork();

	if(child == -1){
		print_error();
		return;
	}else if(child == 0){
		handle_error(dup2(p2[READ], READ));

		handle_error(close(p3[READ]));
		handle_error(dup2(p3[WRITE], WRITE));

		handle_error(execlp("sort", "sort", NULL));
	}
	
	handle_error(close(p2[READ]));
	handle_error(close(p3[WRITE]));
	handle_error(wait(&status));
}

void run_pager(){
	int err = 0;
	child = fork();

	if(child == -1){
		print_error();
		return;
	} else if(child == 0){
		char * pager_var = getenv("PAGER");
		if(pager_var == NULL){
			pager_var = "less";
		}

		handle_error(dup2(p3[READ], READ));

		/*This one ran outside the forloop -> in parent, not good*/
		err = execlp(pager_var, pager_var, NULL);

		/*If env-pager/less fails try more*/
		if(err == -1)
			handle_error(execlp("more", "more", NULL));
	}

	handle_error(close(p3[READ]));
	handle_error(wait(&status));
}
