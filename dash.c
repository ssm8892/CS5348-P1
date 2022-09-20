#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processCommand(char*, bool*);

int main() {
	bool status = true;
	do {
		printf("dash> ");
		char* line = NULL;
		size_t len = 0;
		ssize_t linesize = 0;
		linesize = getline(&line, &len, stdin);
		char* tokens = strtok(line, "&");
		processCommand(tokens, &status);
		free(line);
		if(status == false){
			exit(0);
		}
		printf("\n");
	} while (status);
	return 0;
}

int processCommand(char* tokens, bool* status){
	printf("Number of words in commands: %zu", strlen(tokens));
	int i = 0;
	for(i; i < strlen(tokens); i++){

	}
	if(strcmp(tokens, "exit")==0){
		*status = false;
	}
	return 0;
}