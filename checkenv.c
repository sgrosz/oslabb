#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "helper.h"
#include "checkenv.h"

/*	Define WRITE and READ for more readable code */
#define WRITE 1
#define READ 0

int p1[2], p2[2], p3[2];
pid_t child;
int status;

/*	First creates pipes and checks them for errors.
	Then systematically execute printenv, grep, sort and pager. */
void run_checkenv(char * args){
	if(pipe(p1) == -1){
		print_error("checkenv.c:21");
		return;
	}

	if(pipe(p2) == -1){
		print_error("checkenv.c:26");
		return;
	}

	if(pipe(p3) == -1){
		print_error("checkenv.c:31");
		return;
	}

	run_printenv();
	run_grep(args);
	run_sort();
	run_pager();
}

/*	Turns child into a forked process and checks if successful. 
	Closes the READ end of pipe 1  and redirects WRITE of child to pipe 1 before executing
	printenv. If fork is succesful, function always ends with closing WRITE of pipe 1 and
	waiting for child. */
void run_printenv(){
	child = fork();

	if(child == -1){
		print_error("checkenv.c:47");
		return;
	} else if(child == 0){
		handle_error(close(p1[READ]), "checkenv.c:53");
		handle_error(dup2(p1[WRITE], WRITE), "checkenv.c:54");

		handle_exec_error(execlp("printenv", "printenv", NULL), "checkenv.c:56");
	}

	handle_error(close(p1[WRITE]), "checkenv.c:59");
	handle_error(wait(&status), "checkenv.c:60");
}

/*	Turns the child into a forked process and checks if successful. If successful it
	redirects READ of child to pipe 1, closes READ of pipe 2 and redirects WRITE of child to 
	WRITE of pipe 2. If there are no arguments for grep, concatenate. If there are, 
	execute grep. If fork is succesful, function always ends with closing READ of pipe 1, 
	WRITE of pipe 2 and waiting for child. */
void run_grep(char * args){
	child = fork();

	if(child == -1){
		print_error("checkenv.c:69");
		return;
	}else if(child == 0){
		handle_error(dup2(p1[READ], READ), "checkenv.c:75");

		handle_error(close(p2[READ]), "checkenv.c:77");
		handle_error(dup2(p2[WRITE], WRITE), "checkenv.c:78");

		if(args == NULL){
			handle_exec_error(execlp("cat", "cat", NULL), "checkenv.c:81");
		} else{
			handle_exec_error(execvp("grep", handle_args("grep", args)), "checkenv.c:83");
		}
	}

	handle_error(close(p1[READ]), "checkenv.c:87");
	handle_error(close(p2[WRITE]), "checkenv.c:88");
	handle_error(wait(&status), "checkenv.c:89");

}

/*	Turns the child into a forked process and checks if successful. If successful, it
	redirects READ of child to READ of pipe 2, closes the READ of pipe 3, redirects
	WRITE of child to WRITE of pipe 3 and executes sort. If fork is successful, it
	ends with closing READ of pipe 2, WRITE of pipe 3 and waiting for child. */
void run_sort(){
	child = fork();

	if(child == -1){
		print_error("checkenv.c:98");
		return;
	}else if(child == 0){
		handle_error(dup2(p2[READ], READ), "checkenv.c:104");

		handle_error(close(p3[READ]), "checkenv.c:106");
		handle_error(dup2(p3[WRITE], WRITE), "checkenv.c:107");

		handle_exec_error(execlp("sort", "sort", NULL), "checkenv.c:109");
	}
	
	handle_error(close(p2[READ]), "checkenv.c:112");
	handle_error(close(p3[WRITE]), "checkenv.c:113");
	handle_error(wait(&status), "checkenv.c:114");
}

/*	Turns the child into a forked process and checks if successful. If successful we fetch
	the PAGER environment. If no PAGER variable is set, use "less". Redirect READ of child
	to READ of pipe 3. Try executing pager using pager_var, if unsuccessful, execute pager
	using "more". If fork was succesful, end by closing READ of pipe 3 and waiting for child. */
void run_pager(){
	int err = 0;
	child = fork();

	if(child == -1){
		print_error("checkenv.c:123");
		return;
	} else if(child == 0){
		char * pager_var = getenv("PAGER");
		if(pager_var == NULL){
			pager_var = "less";
		}

		handle_error(dup2(p3[READ], READ), "checkenv.c:134");

		/*This one ran outside the forloop -> in parent, not good*/
		err = execlp(pager_var, pager_var, NULL);

		/*If env-pager/less fails try more*/
		if(err == -1)
			handle_exec_error(execlp("more", "more", NULL), "checkenv.c:141");
	}

	handle_error(close(p3[READ]), "checkenv.c:144");
	handle_error(wait(&status), "checkenv.c:145");
}
