#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define WRITE 1
#define READ 0

void run_checkenv(char * args);
int run_pager();
int run_printenv();
int run_sort();
int run_grep(char * args);

int p1[2], p2[2], p3[2];
pid_t printenv, pager, sort, grep;
int status;

void run_checkenv(char * args){
	int err;
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

	err = run_printenv();
	if(err == -1){
		return;
	}
	err = run_grep(args);
	if(err == -1){
		return;
	}
	err = run_sort();
	if(err == -1){
		return;
	}
	err = run_pager();
	if(err == -1){
		return;
	}
}

int run_printenv(){
	int err = 0;
	printenv = fork();

	if(printenv == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}else if(printenv == 0){
		close(p1[READ]);
		dup2(p1[WRITE], WRITE);

		err = execlp("printenv", "printenv", 0);
		fprintf(stderr, "%d\n", err);
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}

	close(p1[WRITE]);
	wait(&status);
	return err;
}

int run_grep(char * args){
	int i;
	char * arg;
	char * arg_array[10];

	int err = 0;
	
	/* Splits the arguments into an argument array*/
	i = 1;
	arg_array[0] = "grep";
	arg = strtok(args ," "); 

	while (arg != NULL){
		arg_array[i++] = arg;
		arg = strtok (NULL, " ");
	}
	arg_array[i++] = NULL;

	for (i=0;i<3; ++i) 
    	printf("%s\n", arg_array[i]);

	grep = fork();

	if(grep == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}else if(grep == 0){
		close(p1[WRITE]);
		dup2(p1[READ], READ);

		close(p2[READ]);
		dup2(p2[WRITE], WRITE);

		if(args == NULL){
			err = execlp("cat", "cat", 0);
		} else{
			err = execvp("grep", arg_array);
		}
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}

	close(p1[READ]);
	close(p2[WRITE]);
	wait(&status);
	return err;

}

int run_sort(){
	int err = 0;
	sort = fork();

	if(sort == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}else if(sort == 0){

		close(p2[WRITE]);
		dup2(p2[READ], READ);

		close(p3[READ]);
		dup2(p3[WRITE], WRITE);

		execlp("sort", "sort", 0);
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}

	close(p2[READ]);
	close(p3[WRITE]);
	wait(&status);
	return err;
}

int run_pager(){
	int err = 0;
	pager = fork();

	if(pager == -1){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
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
		err = execlp("more", "more", 0);

		fprintf(stderr, "Error: %s\n", strerror(errno));
	}

	close(p3[READ]);
	wait(&status);
	return err;
}
