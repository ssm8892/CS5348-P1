#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processCommand(char*, size_t, bool*);
void processLine(char*, ssize_t, bool*);
void executeCommand(char* [], int, bool*);

int main() {
	bool status = true;
	do {
		printf("dash> ");
		char* line = NULL;
		size_t len = 0;
		ssize_t linesize = 0;
		linesize = getline(&line, &len, stdin);
		line[strlen(line)-1]='\0';
		linesize--;
		processLine(line, linesize, &status);
		free(line);
		if(status == false){
			exit(0);
		}
		printf("\n");
	} while(status);
	return 0;
}

void processLine(char* line, ssize_t linesize, bool* status){
	char* cmdline = malloc(sizeof(char) * linesize);
	memcpy(cmdline, line, sizeof(char) * linesize);
	int commandCount = 0;
	while(cmdline != NULL){
		char* token = strtok_r(cmdline, "&", &cmdline);
		if(token == NULL){
			break;
		}
		commandCount++;
		processCommand(token,strlen(token), status);
//		printf("TOKEN: %s\n", token);
//		printf("LINE: %s\n", line);
	}
//	printf("TOKEN COUNT: %d\n", commandCount);
}

void processCommand(char* token, size_t tokensize, bool* status){
	char* tempToken = malloc(sizeof(char) * tokensize);
	memcpy(tempToken, token, sizeof(char) * tokensize);
	int wordCount = 0;
//	printf("TEMP TOKEN: %s\n", tempToken);
	while(tempToken != NULL){
		char* word = strtok_r(tempToken, " ", &tempToken);
		if(word == NULL){
			break;
		}
		wordCount++;
//		printf("WORD: %s\n", word); 
	}
//	printf("WORD COUNT: %d\n", wordCount);
	tempToken = malloc(sizeof(char) * tokensize);
	memcpy(tempToken, token, sizeof(char) * tokensize);
	char* arr[wordCount+1];
	int count = 0;
	while(tempToken != NULL){
		char* word = strtok_r(tempToken, " ", &tempToken);
		if(word == NULL){
			break;
		}	
		arr[count] = word;
		printf("arr[%d]: %s\n", count, word);
		count++; 
	}
	executeCommand(arr, wordCount, status);
}

void executeCommand(char* arr[], int wordCount, bool* status){
	if(strncmp(arr[0], "exit", 4) == 0){
		*status = false;
		return;
	}
} 
