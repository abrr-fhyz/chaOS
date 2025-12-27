#include "../lib/Util.h"
#include "../lib/Shell.h"
#include "../lib/File.h"

char outputBuffer[1024][1024];
int outputLoaded;
int outputPrinted;

void initOutput(){
   	createProcess("printf", output_process);
    outputLoaded = 0;
    outputPrinted = 0;
    printStartUp(0);
}

void raiseError(int x, int y){
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "Incorrect number of arguments: Expected %d, Found %d\n", x, y);
	printMessage(buffer);
}

int compare(char *str1, char *str2){
	int cnt = 0;
	while(str1[cnt] != '\0'){
		if(str1[cnt] != str2[cnt])
			return 0;
		cnt ++;
	}
	if(str2[cnt] != '\0')
		return 0;
	return 1;
}

int contains(char *str, char ch){
	int flag = -1;
	for(int i=strLen(str)-1; i>=0; i--){
		if(str[i] == ch){
			flag = i;
			break;
		}
	}
	return flag;
}

int strLen(char *str1){
	int cnt = 0;
	while(str1[cnt] != '\0'){
		cnt ++;
	}
	return cnt;
}

void initInput(){
   	createProcess("scanf", input_process);
   	printMessage("\t\t\t\tInput Boot Up\t\t\t---\t\t\tCompleted\n");
}

void printStartUp(int start){
	if(start == 0){
		system("cls");
		printMessage("\t\t\t\t\t\t\t   |||   CHA-OS   |||\n\n");
		printMessage("\t\t\t\t\t\t|||\t      Copyright 2025\t\t|||\n\n");
		printMessage("\t\t\t\t\t\tBooting chaOS Operating System........\n");
		printMessage("\t\t\t\tOutput Boot Up\t\t\t---\t\t\tCompleted\n");
	} else if(start == 1) {
		printMessage("\t\t\t\t\t\tOperating System Initialized........\n");
		printMessage("##############################################################################################################################\n\n\n");
	} else {
		system("cls");
		printMessage("\t\t\t\t\t\t\t   |||   CHA-OS   |||\n\n");
		printMessage("\t\t\t\t\t\t|||\t      Copyright 2025\t\t|||\n\n");
		printMessage("##############################################################################################################################\n\n\n");
	}
}

void printMessage(const char *msg){
	int i = 0;
	while(msg[i] != '\0'){
		outputBuffer[outputLoaded][i] = msg[i];
		i++;
	}
	outputBuffer[outputLoaded][i] = '\0';
	outputLoaded++;
	if(outputLoaded == 1024){
		outputLoaded = 0;
	}
}

void printLs(const char* type, const char* size, const char* name){
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "%s\t\t%s\t\t%s\n", type, size, name);
	printMessage(buffer);
}

void printProcess(const char* pName, int id, int step, const char* currentState){
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "Process: %s\t\t|| PID: %d\t\t|| Steps: %d\t\t|| %s\n", pName, id, step, currentState);
	printMessage(buffer);
}

void printNext(){
	if(outputPrinted >= outputLoaded && (outputPrinted != 0 || outputLoaded != 0)) {
		return;
	}
	printf("%s", outputBuffer[outputPrinted]);
	outputPrinted++;
	if(outputPrinted == 1024){
		outputPrinted = 0;
	}
}

void printLast(){
	while(outputLoaded > outputPrinted){
		printNext();
	}
}

void waitForInput(){
	char *path = getPath();
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "\n$%s> ", path);
	printMessage(buffer);
	printLast();
	char arg[1024] = "";
	int cnt = 0;
	char c;
	while(scanf("%c", &c) == 1 && c != '\n'){
		arg[cnt++] = c;
	}
	processArgument(arg);
}

void fileEdit(int fileIdx){
	printLast();
	char content[1024];
	int cnt = 0;
	char c, prev = '\0';
	while(scanf("%c", &c) == 1){
		if(prev != '\0' && prev == '\n' && c == '\n'){
			break;
		}
		content[cnt++] = c;
		prev = c;
	}
	logContent(fileIdx, content);
}