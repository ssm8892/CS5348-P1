#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
extern FILE *stderr;

void processCommand(char*, size_t, bool*);
void processLine(char*, ssize_t, bool*);
void executeCommand(char* [], int, bool*);
void errorOccurred();
void addPaths(char* [], int);

int pathCount = 1;
char* paths = "/bin";

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
//		printf("arr[%d]: %s\n", count, word);
		count++; 
	}
	arr[count] = NULL;
	executeCommand(arr, wordCount, status);
}

void executeCommand(char* arr[], int wordCount, bool* status){
	if(strncmp(arr[0], "exit", 4) == 0){
		if(wordCount > 1 || wordCount < 1){
			errorOccurred();
			return;
		}
		*status = false;
		return;
	} else if(strncmp(arr[0], "path", 4) == 0){
		addPaths(arr, wordCount);	
	} else if(strncmp(arr[0], "cd", 2) == 0){
		if(wordCount > 2 || wordCount < 2){
			errorOccurred();
			return;
		}
		int status = chdir(arr[1]);
		if(status != 0){
			errorOccurred();
			return;
		}
	} else {
		char* tempPaths = malloc(sizeof(char) *strlen(paths));
		memcpy(tempPaths, paths, strlen(paths));
		while(tempPaths != NULL){
			char* tempPath = strtok_r(tempPaths, " ", &tempPaths);
			if(tempPath == NULL){
				break;
			} 
			char* path = malloc(sizeof(char) * (strlen(tempPath)+strlen(arr[0])+strlen("/"))); 
			memcpy(path, tempPath, strlen(tempPath));
			strcat(path, "/");
			strcat(path, arr[0]);
//			printf("%s\n", path);
			if(access(path, X_OK) == 1){
				continue;
			}
			pid_t pid;
			pid = fork();
			if(pid == 0){
				if(execv(path, arr) < 0){
					printf("ERROR IN EXECUTION LINE\n");
					errorOccurred();	
				}
				exit(0);
			} else if (pid < 0) {
				printf("ERROR IN CREATION A PROCESS\n");
				errorOccurred();
			} else {
				wait(NULL);
				return;
			}
		}
		errorOccurred();
		return;
	}
} 

void errorOccurred(){
	char error_message[30] = "An error occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

void addPaths(char* args[], int wordCount){
	size_t length = 0;
	char* tempPaths;
	int k;
	for(k = 1; k < wordCount; k++){
		length += sizeof(char) * strlen(args[k]);
	}
	//This accounts for the number of spaces and the null terminator
	//at the end of the string
	length += sizeof(char) * wordCount;
	tempPaths = malloc(length);
	for(k = 1; k < wordCount; k++){
		strcat(tempPaths, args[k]);
		strcat(tempPaths, " ");
	}
//	printf("NEWPATHS: %s\n", tempPaths);
	paths = tempPaths;
}
	
