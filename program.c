#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define INPUT_LENGTH 80
#define MAX_ARGS 4

void print_current_directory(char * path);
void change_directory(char * path);

char line[INPUT_LENGTH];
char * home;

int main(int argc, char * argv[]){
	char * path;
	char * arg;
	home = getenv("HOME");
	path = home;
	chdir(path);

	while(1){
		print_current_directory(path);
		printf("$ ");
		if(!fgets(line, INPUT_LENGTH, stdin)) break;

        if(strncmp("cd", line, strlen("cd")) == 0){
        	change_directory(&line[3]);
        }

	}
	
	return 0;

}

void print_current_directory(char * path){
	char pathBuff[INPUT_LENGTH];

	path = getcwd(pathBuff, INPUT_LENGTH);

	if(path == NULL){
        fprintf(stderr, "%s\n", strerror(errno));
    }

    printf("%s", path);
}

void change_directory(char * path){
	int ret;
	printf("%s", path);
	ret = chdir(path);

	if(ret!=0){
      perror("Error:");
    }
}
