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
char const* paths[] = {"/bin"};

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
		*status = false;
		return;
	} else if(strncmp(arr[0], "path", 4) == 0){
		addPaths(arr, wordCount);	
	} else if(strncmp(arr[0], "cd", 2) == 0){
		//use chdir system call
	} else {
		if(access("/bin/ls", X_OK) == 1){
			printf("ERROR IN ACCESSING DIRECTORY\n");
			errorOccurred();
		}
		char* path = malloc(sizeof(char) * strlen(paths[0])); 
		memcpy(path, paths[0], strlen(paths[0]));
		strcat(path, "/");
		strcat(path, arr[0]);
//		printf("%s\n", path);
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
} 

void errorOccurred(){
	char error_message[30] = "An error occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

void addPaths(char* args[], int wordCount){
	//
}
	
