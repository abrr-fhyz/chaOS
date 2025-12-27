#include "../lib/Shell.h"
#include "../lib/Handler.h"
#include "../lib/Util.h"
#include "../lib/Process.h"
#include "../lib/File.h"

char argv[2][1024];
char cdArray[16][16];
char pathArray[16][16];
char compoundPaths[2][64];
char intructions[32][64];

char currentPath[64];
char fileName[16];

int continueArgument;


void split(char *arg){
	for(int i=0; i<2; i++){
		for(int j=0; j<1024; j++){
			argv[i][j] = '\0';
		}
	}
	int cnt = 0, idx = 0, flag = 0, len = 0;
	while(cnt < 1023){
		if(arg[cnt] == ' ' && flag == 0){
			idx ++;
			flag = 1;
			len = cnt + 1;
		} else {
			argv[idx][cnt - len] = arg[cnt];
		}
		cnt ++;
	}
}

void initShell(){
	createProcess("shell", shell_process);
	printMessage("\t\t\t\tShell Boot Up\t\t\t---\t\t\tCompleted\n");
}

void showHelp(){
	printMessage("Recognised commands:\n\thelp\t\t- show recognised commands\n\techo TEXT\t- print TEXT as a string\n\tps\t\t- list processes\n\tclear\t\t- clear screen\n\tmkdir DIR\t- make new directory DIR\n\tls\t\t- list available directories and files\n\tcd PATH\t\t- change current directory to PATH\n"
		"\ttouch FILE\t- create an empty file FILE\n\tedit PATH\t- edit the file present at the end of PATH, press enter twice to save\n\tcat PATH\t- show the contents of the file at the end of PATH\n\tmv PATHA PATHB\t- move a given file from PATHA to PATHB\n\tcp PATHA PATHB\t- copy file from PATHA to PATHB\n"
		"\tdel PATH\t- delete the file at the end of PATH\n\t./FILE\t\t- executes console commands present in FILE in sequential order\n\texit\t\t- exit interface\n");
}

void splitCmd(char *cmd){
	for(int i=0; i<16; i++)
		for(int j=0; j<16; j++)
			cdArray[i][j] = '\0';

	int cnt = 0, idx = 0;
	for(int i=0; i<strLen(cmd); i++){
		if(cmd[i] == '\\'){
			idx ++;
			cnt = 0;
			continue;
		}
		cdArray[idx][cnt] = cmd[i];
		cnt ++;
	}
}

void cd(char *cmd){
	splitCmd(cmd);
	for(int i=0; i<16; i++){
		if(cdArray[i][0] == '\0')
			break;
		if(compare(cdArray[i], "..")){
			cdBack();
		} else {
			cdFront(cdArray[i]);
		}
	}
}

void processDirectory(char *cmd){
	int flag = contains(cmd, '\\');
	char *ptr = getPath();
	for(int i=0; i<64; i++)
		currentPath[i] = '\0';
	for(int i=0; i<strLen(ptr); i++)
		currentPath[i] = ptr[i];
	for(int i=0; i<16; i++)
		fileName[i] = '\0';
	if(flag == -1){
		for(int i=0; i<strLen(cmd); i++){
			fileName[i] = cmd[i];
		}
		return;
	}
	for(int i=flag+1; i<strLen(cmd); i++){
		fileName[i - (flag+1)] = cmd[i];
	}
	for(int i=flag; i<strLen(cmd); i++){
		cmd[i] = '\0';
	}
	printMessage(" ");
	cd(cmd);
}

void restorePath(char *path){
	cd("..\\..\\..\\..\\..\\..\\..\\..\\..");
	cd(path);
}

void processCompoundArgument(char *cmd){
	for(int i=0; i<64; i++)
		for(int j=0; j<64; j++)
			compoundPaths[i][j] = '\0';
	int idx = 0, cnt = 0;
	continueArgument = 1;
	for(int i=0; i<strLen(cmd); i++){
		if(cmd[i] == ' '){
			idx ++;
			cnt = 0;
			if(idx == 2){
				raiseError(3, 4);
				continueArgument = 0;
				return;
			}
			continue;
		}
		compoundPaths[idx][cnt++] = cmd[i];
	}
	if(!idx){
		raiseError(3, 2);
		continueArgument = 0;
		return;
	}
}

void cp(){
	if(compare(compoundPaths[0], compoundPaths[1])){
		printMessage("Can not move file with same name within same directory.");
		return;
	}
	processDirectory(compoundPaths[0]);
	char *ptr = getContent(fileName);
	char content[1024];
	for(int i=0; i<1024; i++)
		content[i] = '\0';
	if(ptr == NULL){
		printMessage("Target File Not Found");
		return;
	}
	for(int i=0; i<strLen(ptr); i++)
		content[i] = ptr[i];
	restorePath(currentPath);
	wait(3);
	processDirectory(compoundPaths[1]);
	touch(fileName);
	int idx = findFileIndex(fileName);
	logContent(idx, content);
	restorePath(currentPath);
	printMessage(" ");
}

void exe(char *arg){
	int n = strLen(arg);
	for(int i=2; i<n; i++)
		arg[i-2] = arg[i];
	arg[n-2] = '\0';
	char *ptr = getContent(fileName);
	if(ptr == NULL){
		printMessage("Executable File Not Found");
		return;
	}
	int instr = 0, cnt = 0;
	for(int i=0; i<strLen(ptr); i++){
		if(ptr[i] == '\n'){
			instr ++;
			cnt = 0;
			continue;
		}
		intructions[instr][cnt++] = ptr[i];
	}
	for(int i=0; i<instr+1; i++){
		processArgument(intructions[i]);
	}
}

void processArgument(char *arg){
	split(arg);
	if(compare(argv[0], "echo")){
		int idx = strLen(argv[1]);
		argv[1][idx] = '\n';
		printMessage(argv[1]);
	}
	else if(compare(argv[0], "ps")){
		listProcesses();
	}
	else if(compare(argv[0], "clear") || compare(argv[0], "cls")){
		printStartUp(2);
	}
	else if(compare(argv[0], "help")){
		showHelp();
	}
	else if(compare(argv[0], "exit")){
		shutDown();
	}
	else if(compare(argv[0], "ls")){
		ls();
	}
	else if(compare(argv[0], "mkdir")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		mkdir(argv[1]);
	}
	else if(compare(argv[0], "cd")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		cd(argv[1]);
	}
	else if(compare(argv[0], "touch")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		touch(argv[1]);
	}
	else if(compare(argv[0], "cat")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		processDirectory(argv[1]);
		char* content = cat(fileName);
		if(content != NULL)
			printMessage(content);
		restorePath(currentPath);

	}
	else if(compare(argv[0], "edit")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		processDirectory(argv[1]);
		edit(fileName);
		restorePath(currentPath);
	}
	else if(compare(argv[0], "mv")){
		if(strLen(argv[1]) == 0)
			raiseError(3, 1);
		processCompoundArgument(argv[1]);
		if(continueArgument){
			cp();
			del(compoundPaths[0]);
		}
	}
	else if(compare(argv[0], "cp")){
		if(strLen(argv[1]) == 0)
			raiseError(3, 1);
		processCompoundArgument(argv[1]);
		if(continueArgument)
			cp();
	}
	else if(compare(argv[0], "del")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		processDirectory(argv[1]);
		del(fileName);
		restorePath(currentPath);
	}
	else if(argv[0][0] == '.' && argv[0][1] == '/'){
		exe(argv[0]);
	}
	else{
		printMessage("Unrecognised Command. Try 'help' for options.");
	}
	wait(2);
}