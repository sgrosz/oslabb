#include <stdio.h>

#define INPUT_LENGTH 80

int main(int argc, char * argv[]){
	char line[INPUT_LENGTH];

	while(1){
		printf("$ ");
		if(!fgets(line, INPUT_LENGTH, stdin)) break;
		printf("%s", line);
	}
	return 0;
}
