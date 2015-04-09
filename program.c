#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define INPUT_LENGTH 80

int main(int argc, char * argv[]){
	char line[INPUT_LENGTH];
	
	pid_t pid = fork();

	if(pid>0){
	
		while(1){
			printf("$ ");
			if(!fgets(line, INPUT_LENGTH, stdin)) break;
			printf("%s", line);
		}
		return 0;
	}
	
	if(pid==0){
		sleep(10);
	}
	
	return 0;

}
