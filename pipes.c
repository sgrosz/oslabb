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

int p1[2], p2[2];
pid_t printenv, pager;
int status;

void run_checkenv(){
	if(pipe(p1) == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}

	printenv = fork();

	if(printenv == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return;
	}else if(printenv == 0){
		close(p1[READ]);
		dup2(p1[WRITE], WRITE);

		execlp("printenv", "printenv", 0);
	} else {
		pager = fork();

		if(pager == -1){
			fprintf(stderr, "Error: %s\n", strerror(errno));
		} else if(pager == 0){
			close (p1[WRITE]);
			dup2 (p1[READ], READ);

			/*This one ran outside the forloop -> in parent, not good*/
			execlp("more", "more", 0);
		}
	}

	/* This is the parent */
    close(p1[READ]);
    close(p1[WRITE]);

    waitpid(printenv, &status, 0);
    waitpid(pager, &status, 0);

}
