#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "helper.h"
#include "checkenv.h"

#define WRITE 1
#define READ 0

int p1[2], p2[2], p3[2];
pid_t child;
int status;


void run_checkenv(char * args){
	if(pipe(p1) == -1){
		print_error("checkenv.c:19");
		return;
	}

	if(pipe(p2) == -1){
		print_error("checkenv.c:24");
		return;
	}

	if(pipe(p3) == -1){
		print_error("checkenv.c:29");
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
		print_error("checkenv.c:44");
		return;
	}else if(child == 0){
		handle_error(close(p1[READ]), "checkenv.c:48");
		handle_error(dup2(p1[WRITE], WRITE), "checkenv.c:49");

		handle_error(execlp("printenv", "printenv", NULL), "checkenv.c:51");
	}

	handle_error(close(p1[WRITE]), "checkenv.c:54");
	handle_error(wait(&status), "checkenv.c:55");
}

void run_grep(char * args){
	child = fork();

	if(child == -1){
		print_error("checkenv.c:59");
		return;
	}else if(child == 0){
		handle_error(dup2(p1[READ], READ), "checkenv.c:65");

		handle_error(close(p2[READ]), "checkenv.c:67");
		handle_error(dup2(p2[WRITE], WRITE), "checkenv.c:68");

		if(args == NULL){
			handle_error(execlp("cat", "cat", NULL), "checkenv.c:71");
		} else{
			handle_error(execvp("grep", handle_args("grep", args)), "checkenv.c:73");
		}
	}

	handle_error(close(p1[READ]), "checkenv.c:77");
	handle_error(close(p2[WRITE]), "checkenv.c:78");
	handle_error(wait(&status), "checkenv.c:79");

}

void run_sort(){
	child = fork();

	if(child == -1){
		print_error("checkenv.c:84");
		return;
	}else if(child == 0){
		handle_error(dup2(p2[READ], READ), "checkenv.c:90");

		handle_error(close(p3[READ]), "checkenv.c:92");
		handle_error(dup2(p3[WRITE], WRITE), "checkenv.c:93");

		handle_error(execlp("sort", "sort", NULL), "checkenv.c:95");
	}
	
	handle_error(close(p2[READ]), "checkenv.c:98");
	handle_error(close(p3[WRITE]), "checkenv.c:99");
	handle_error(wait(&status), "checkenv.c:100");
}

void run_pager(){
	int err = 0;
	child = fork();

	if(child == -1){
		print_error("checkenv.c:105");
		return;
	} else if(child == 0){
		char * pager_var = getenv("PAGER");
		if(pager_var == NULL){
			pager_var = "less";
		}

		handle_error(dup2(p3[READ], READ), "checkenv.c:116");

		/*This one ran outside the forloop -> in parent, not good*/
		err = execlp(pager_var, pager_var, NULL);

		/*If env-pager/less fails try more*/
		if(err == -1)
			handle_error(execlp("more", "more", NULL), "checkenv.c:123");
	}

	handle_error(close(p3[READ]), "checkenv.c:126");
	handle_error(wait(&status), "checkenv.c:127");
}
