#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define WRITE 1
#define READ 0

void run_checkenv();
void run_pager();
void run_printenv();
void run_sort();
void run_grep();

int p1[2], p2[2], p3[2];
pid_t printenv, pager, sort, grep;
int status;

void run_checkenv(){
	if(pipe(p1) == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}

	if(pipe(p2) == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}

	if(pipe(p3) == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}

	run_printenv();
	run_grep();
	run_sort();
	run_pager();
}

void run_printenv(){
	printenv = fork();

	if(printenv == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}else if(printenv == 0){
		close(p1[READ]);
		dup2(p1[WRITE], WRITE);

		execlp("printenv", "printenv", 0);
	}

	close(p1[WRITE]);
	wait(&status);
}

void run_grep(){
	grep = fork();

	if(grep == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}else if(grep == 0){
		close(p1[WRITE]);
		dup2(p1[READ], READ);

		close(p2[READ]);
		dup2(p2[WRITE], WRITE);

		execlp("cat", "cat", 0);
	}
	close(p1[READ]);
	close(p2[WRITE]);
	wait(&status);

}

void run_sort(){
	sort = fork();

	if(sort == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}else if(sort == 0){

		close(p2[WRITE]);
		dup2(p2[READ], READ);

		close(p3[READ]);
		dup2(p3[WRITE], WRITE);

		execlp("sort", "sort", 0);
	}

	close(p2[READ]);
	close(p3[WRITE]);
	wait(&status);
}

void run_pager(){
	pager = fork();

	if(pager == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
	} else if(pager == 0){
		char * pager_var = getenv("PAGER");
		if(pager_var == NULL){
			pager_var = "less";
		}

		close(p3[WRITE]);
		dup2(p3[READ], READ);

		/*This one ran outside the forloop -> in parent, not good*/
		execlp(pager_var, pager_var, 0);

		/*If env-pager/less fails try more*/
		execlp("more", "more", 0);
	}

	close(p3[READ]);
	wait(&status);
}
