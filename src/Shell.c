#include "../lib/Shell.h"
#include "../lib/Handler.h"
#include "../lib/Util.h"
#include "../lib/Process.h"
#include "../lib/File.h"

char argv[2][1024];
char cdArray[16][16];

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
	printMessage("Recognised commands:\n\thelp\t\t- show recognised commands\n\techo\t- print next string\n\tps\t\t- list processes\n\tclear\t\t- clear screen\n\tmkdir\t\t- make new directory\n\tcd\t\t- change current directory\n\texit\t\t- exit interface\n");
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

void processArgument(char *arg){
	split(arg);
	if(compare(argv[0], "echo")){
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
		mkdir(argv[1]);
	}
	else if(compare(argv[0], "cd")){
		cd(argv[1]);
	}
	else{
		printMessage("Unrecognised Command. Try 'help' for options.");
	}

}