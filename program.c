#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define INPUT_LENGTH 80
#define MAX_ARGS 4

int main(int argc, char * argv[]){
	char line[INPUT_LENGTH];
	char * args[MAX_ARGS];
	char ** arg; 
	
	pid_t pid = fork();

	if(pid>0){
	
		while(1){
			printf("$ ");
			if(!fgets(line, INPUT_LENGTH, stdin)) break;
			printf("%s", line);

			arg = args;
	        *arg++ = strtok(line," "); 
	        while ((*arg++ = strtok(NULL, " ")));

	        printf("%s\n", args[0]);
		}
		return 0;
	}
	
	if(pid==0){
		sleep(10);
	}
	
	return 0;

}
