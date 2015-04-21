#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define INPUT_LENGTH 80
#define MAX_ARGS 4

void print_current_directory();

int main(int argc, char * argv[]){
	char line[INPUT_LENGTH];
	char * args[MAX_ARGS];
	char ** arg; 

	while(1){
		print_current_directory();
		printf("$ ");
		if(!fgets(line, INPUT_LENGTH, stdin)) break;

		arg = args;
        *arg++ = strtok(line," "); 
        while ((*arg++ = strtok(NULL, " ")));

        printf("%s\n", args[0]);
	}
	
	return 0;

}

void print_current_directory(){
	char * path;
	char pathBuff[INPUT_LENGTH];

	path = getcwd(pathBuff, INPUT_LENGTH);
	if(path == NULL){
        
    }

    printf("%s", path);
}
