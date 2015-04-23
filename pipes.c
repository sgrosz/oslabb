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
	char *nargv[5] = {"printenv", NULL};
	char *margv[5] = {"more", NULL};

	if(pipe(p1) == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}

	printenv = fork();

	if(printenv == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}else if(printenv == 0){
		close(p1[READ]);
		dup2(p1[WRITE], WRITE);
		close(p1[WRITE]);

		execvp("printenv", nargv);
	} else {
		pager = fork();

		if(pager == -1){
			fprintf(stderr, "Error: %s\n", strerror(errno));
		} else if(pager == 0){
			close (p1[WRITE]);
			dup2 (p1[READ], READ);
			close (p1[READ]);
		}

		execvp("more", margv);
	}

	/* This is the parent */
    close(p1[READ]);
    close(p1[WRITE]);

    waitpid (pager, &status, 0);
    printf("Done waiting for more.\n");
}
