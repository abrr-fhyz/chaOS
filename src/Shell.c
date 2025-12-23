#include "../lib/Shell.h"
#include "../lib/Handler.h"
#include "../lib/Util.h"
#include "../lib/Process.h"

Process* shellHandler;
char argv[2][1024];

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

void initShell(){
	shellHandler = createProcess("shell", shell_process);
	printMessage("\t\t\t\tShell Boot Up\t\t\t---\t\t\tCompleted\n");
}

void showHelp(){
	printMessage("Recognised commands:\n\thelp\t\t- show recognised commands\n\techo TEXT\t- print TEXT\n\tps\t\t- list processes\n\tclear\t\t- clear screen\n\texit\t\t- exist interface\n");
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
	else{
		printMessage("Unrecognised Command. Try 'help' for options.");
	}

}