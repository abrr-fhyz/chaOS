#include "../lib/Shell.h"
#include "../lib/Handler.h"
#include "../lib/Util.h"
#include "../lib/Process.h"
#include "../lib/File.h"
#include "../lib/Memory.h"









// UTIL FUNCTIONS //
void split(char *arg){
	for(int i=0; i<2; i++)
		for(int j=0; j<4096; j++)
			argv[i][j] = '\0';
	int cnt = 0, idx = 0, flag = 0, len = 0;
	while(cnt < 4095){
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
void showHelp(){
	printMessage("Recognised commands:\n\thelp\t\t\t- show recognised commands\n\techo TEXT\t\t- print TEXT as a string\n\tps\t\t\t- list processes\n\tclear\t\t\t- clear screen, also works as \"cls\"\n\tmkdir DIR\t\t- make new directory DIR\n\tls\t\t\t- list available directories and files\n\t\t\t\t-> ls var:\tlist available variables\n\tcd PATH\t\t\t- change current directory to PATH\n\tmem\t\t\t- show current memory status\n"
		"\tmake FILE\t\t- create an empty file with name FILE\n\tedit PATH\t\t- edit the file present at the end of PATH, press enter twice to save\n\tcat PATH\t\t- show the contents of the file at the end of PATH\n\tmv PATHA PATHB\t\t- move a given file from PATHA to PATHB\n\tcp PATHA PATHB\t\t- copy file from PATHA to PATHB\n"
		"\tvar VARIABLE\t\t- initialize new variable with name VARIABLE\n\tlabel LABEL\t\t- initialize new label with name LABEL\n\tcalc VARA OP VARB\t- perform arithmetic or logical operation OP on variables VARA and VARB\n\t\t\t\t- Available OP: +, -, *, /, %, &, |, ^, =, !\n\t\t\t\t- Answer stored in ANS Variable\n\tset VAR FILE\t\t- write the value of variable VAR into the file FILE\n\tload FILE VAR\t\t- write the value of file FILE into the variable VAR\n\tjump LABEL\t\t- unconditional jump to LABEL\n\t\t\t\t-> jump LABEL VAR:\tjump to the label LABEL if VAR is non-zero\n"
		"\tdel PATH\t\t- delete the file at the end of PATH\n\tdeldir PATH\t\t- delete the directory at the end of PATH\n\tread PATH i\t\t- loads the ASCII value of the i-th character in the file at the end of PATH\n\twrite PATH i\t\t- writes the value of the ANS variable as an ASCII character at the i-th place of file at PATH\n\t./FILE\t\t\t- executes console commands present in FILE in sequential order\n\texit\t\t\t- exit interface\n");
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
		for(int i=0; i<strLen(cmd); i++)
			fileName[i] = cmd[i];
		return;
	}
	for(int i=flag+1; i<strLen(cmd); i++)
		fileName[i - (flag+1)] = cmd[i];
	for(int i=flag; i<strLen(cmd); i++)
		cmd[i] = '\0';
	printMessage(" ");
	cd(cmd);
}
void restorePath(char *path){
	cd("..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\..");
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
char* callibrate(int idx, char *target){
	char parts[2][16] = {{0}};
	for(int i=0; i<16; i++)
		tempLabel[i] = '\0';
	int val = 0, cnt = 0;
	for(int i=0; i<strLen(intructions[idx]); i++){
		if(intructions[idx][i] == ' '){
			val = 1;
			cnt = 0;
			continue;
		}
		parts[val][cnt++] = intructions[idx][i];
	}
	if(compare(parts[0], target)){
		for(int i=0; i<strLen(parts[1]); i++)
			tempLabel[i] = parts[1][i];
		return tempLabel;
	}
	static char empty[1] = {0};
	return empty;
}
int convertToInt(char *str){
	int value = 0;
	if(strLen(str) > 11 || (strLen(str) == 11 && str[0] > 49)){
		//max value = 1,999,999,999
		printMessage("Desired value exceeds limit\n");
		return -1;
	}
	int tens = 1;
	for(int i=strLen(str)-2; i>=0; i--){
		if(str[i] >= 48 && str[i] < 58){
			value += ((int)str[i]-48) * tens;
			tens *= 10;
		} 
		else if(i == 0 && str[i] == '-'){
			value *= -1;
			break;
		}
		else {
			printMessage("Non-numeric value detected\n");
			return -1;
		}
	}
	return value;
}









// INTERMEDIARY SHELL FUNCTIONS //
void cp(){
	if(compare(compoundPaths[0], compoundPaths[1])){
		printMessage("Can not move file with same name within same directory.");
		return;
	}
	processDirectory(compoundPaths[0]);
	int fileidx = findFileIndex(fileName);
	char content[4096];
	for(int i=0; i<4096; i++)
		content[i] = '\0';
	if(fileidx == -1){
		printMessage("Target File Not Found");
		return;
	}
	char *ptr = getContent(fileidx);
	for(int i=0; i<strLen(ptr); i++)
		content[i] = ptr[i];
	restorePath(currentPath);
	wait(3);
	processDirectory(compoundPaths[1]);
	if(!determineEligibility(fileName, 0)){
		restorePath(currentPath);
		copySuccess = 0;
		return;
	}
	touch(fileName);
	int idx = findFileIndex(fileName);
	logContent(idx, content);
	restorePath(currentPath);
	copySuccess = 1;
	printMessage(" ");
}
void calc(char *arg){
	int answer = 0;
	int n = strLen(arg);
	char parts[3][16] = {{0}};
	int val = 0, cnt = 0;
	for(int i=0; i<n; i++){
		if(arg[i] == ' '){
			val ++;
			cnt = 0;
			continue;
		}
		parts[val][cnt++] = arg[i];
	}
	if(val != 2){
		raiseError(4, val+2);
		return;
	}
	int operandXIdx = variableExists(parts[0]);
	int operandYIdx = variableExists(parts[2]);
	if(operandYIdx == -1 || operandXIdx == -1){
		printMessage("One or both of the variables do not exist\n");
		return;
	}
	int operandX = getVarValue(operandXIdx);
	int operandY = getVarValue(operandYIdx);
	switch(parts[1][0]){
		case '+':
			answer = operandX + operandY;
			break;
		case '-':
			answer = operandX - operandY;
			break;
		case '*':
			answer = operandX * operandY;
			break;
		case '&':
			answer = operandX & operandY;
			break;
		case '|':
			answer = operandX | operandY;
			break;
		case '/':
			if (operandY != 0)
				answer = operandX / operandY;
			else
				printMessage("Can't divide by zero\n");
			break;
		case '%':
			answer = operandX % operandY;
			break;
		case '^':
			answer = operandX ^ operandY;
			break;
		case '=':
			answer = operandX;
			setVarValue(operandYIdx, operandX);
			break;
		case '!':
			answer = !operandY;
			setVarValue(operandXIdx, !operandX);
			setVarValue(operandYIdx, !operandY);
			break;
		default:
			printMessage("Unkown Operand\n");
			break;
	}
	setVarValue(0, answer);
}
void set(){
	int varIdx = variableExists(compoundPaths[0]);
	if(varIdx == -1){
		printMessage("Variable not found\n");
		return;
	}
	processDirectory(compoundPaths[1]);
	char* content = cat(fileName);	
	restorePath(currentPath);
	int newValue = convertToInt(content);
	setVarValue(varIdx, newValue);
	printMessage(" ");
}
void load(){
	int varIdx = variableExists(compoundPaths[1]);
	if(varIdx == -1){
		printMessage("Variable not found\n");
		return;
	}
	int value = getVarValue(varIdx);
	processDirectory(compoundPaths[0]);
	int fileIdx = findFileIndex(fileName);
	if(fileIdx == -1){
		printMessage("Variable not found\n");
		restorePath(currentPath);
		return;
	}
	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%d", value); 
	wait(5);
	logContent(fileIdx, buffer);
	wait(5);
	restorePath(currentPath);
	wait(5);
	printMessage(" ");
}







//RELATED TO SHELL-BASED COMPUTATIONS//
void exe(char *arg){
	labelCnt = 0;
	for(int i=0; i<64; i++)
		for(int j=0; j<16; j++)
			labels[i][j] = '\0';
	for(int i=0; i<64; i++)
		for(int j=0; j<64; j++)
			intructions[i][j] = '\0';

	int n = strLen(arg);
	for(int i=2; i<n; i++)
		arg[i-2] = arg[i];
	arg[n-2] = '\0';
	int idx = findFileIndex(arg);
	if(idx == -1){
		printMessage("Executable File Not Found");
		return;
	}
	char *ptr = getContent(idx);
	int instr = 0, cnt = 0;
	for(int i=0; i<strLen(ptr); i++){
		if(ptr[i] == '\n'){
			char *newLabel = callibrate(instr, "label");
			if(newLabel[0] != '\0'){
				for(int j=0; j<strLen(newLabel); j++)
					labels[labelCnt][j] = newLabel[j];
				labelLineNumber[labelCnt] = instr;
				labelCnt++;
				if(labelCnt == 64)
					labelCnt = 0;
			}
			instr ++;
			cnt = 0;
			continue;
		}
		intructions[instr][cnt++] = ptr[i];
	}
	for(int i=0; i<instr; i++){
		char *jumpInstr = callibrate(i, "jump");
		if(jumpInstr[0] != '\0'){
			int flag = -1;
			for(int j=0; j<labelCnt; j++){
				if(compare(jumpInstr, labels[j])){
					flag = j;
					break;
				}
			}
			if(flag != -1){
				i = labelLineNumber[flag] - 1;
				continue;
			}
			int val = 0, cnt = 0;
			char parts[2][16] = {{0}};
			for(int x=5; x<strLen(intructions[i]); x++){
				if(intructions[i][x] == ' '){
					val ++;
					if(val == 2){
						raiseError(3, 4);
						return;
					}
					cnt = 0;
					continue;
				}
				parts[val][cnt++] = intructions[i][x];
			}
			if(val == 0)
				return;
			flag = -1;
			for(int j=0; j<labelCnt; j++){
				if(compare(parts[0], labels[j])){
					flag = j;
					break;
				}
			}
			if(flag == -1)
				continue;
			int varIdx = variableExists(parts[1]);
			if(varIdx == -1){
				printMessage("Variable does not exist\n");
				return;
			}
			int condition = getVarValue(varIdx);
			if(condition){
				i = labelLineNumber[flag] - 1;
				continue;
			}
		}
		processArgument(intructions[i]);
		wait(64);
	}
}









// MAIN ARGUMENT PARSER //
void processArgument(char *arg){
	split(arg);
	if(compare(argv[0], "echo")){
		int idx = strLen(argv[1]);
		argv[1][idx] = '\n';
		printMessage(argv[1]);
	}
	else if(compare(argv[0], "mem")){
		mem();
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
		if(strLen(argv[1]) == 0)
			ls();
		else if(compare(argv[1], "var"))
			lsVar();
		else
			printMessage("Unknown ls command\n");
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
	else if(compare(argv[0], "make")){
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
			if(copySuccess)
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
	else if(compare(argv[0], "deldir")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		processDirectory(argv[1]);
		deldir(fileName);
		restorePath(currentPath);
	}
	else if(compare(argv[0], "var")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		storeVar(argv[1]);
	}
	else if(compare(argv[0], "label") || compare(argv[0], "jump")){
		if(strLen(argv[1]) == 0)
			raiseError(2, 1);
		if(contains(argv[1], ' ') != -1 && compare(argv[0], "label") == 1)
			printMessage("Illegal Character in name: ' '\n");
	}
	else if(compare(argv[0], "calc")){
		if(strLen(argv[1]) == 0)
			raiseError(4, 1);
		calc(argv[1]);

	}
	else if(compare(argv[0], "set")){
		if(strLen(argv[1]) == 0)
			raiseError(3, 1);
		processCompoundArgument(argv[1]);
		if(continueArgument)
			set();
	}
	else if(compare(argv[0], "load")){
		if(strLen(argv[1]) == 0)
			raiseError(3, 1);
		processCompoundArgument(argv[1]);
		if(continueArgument)
			load();
	}
	else if(compare(argv[0], "read")){
		if(strLen(argv[1]) == 0)
			raiseError(3, 1);
		processCompoundArgument(argv[1]);
		int n = strLen(compoundPaths[1]);
		compoundPaths[1][n] = '\n';
		processDirectory(compoundPaths[0]);
		int pos = convertToInt(compoundPaths[1]);
		if(pos != -1){
			char x = read(fileName, pos);
			setVarValue(0, (int)x);
		}
		restorePath(currentPath);
		printMessage(" ");
	}
	else if(compare(argv[0], "write")){
		if(strLen(argv[1]) == 0)
			raiseError(3, 1);
		processCompoundArgument(argv[1]);
		int n = strLen(compoundPaths[1]);
		compoundPaths[1][n] = '\n';
		processDirectory(compoundPaths[0]);
		int pos = convertToInt(compoundPaths[1]);
		if(pos != -1){
			char x = (char)getVarValue(0);
			write(fileName, pos, x);
		}
		restorePath(currentPath);
		printMessage(" ");
	}
	else if(argv[0][0] == '.' && argv[0][1] == '/'){
		exe(argv[0]);
	}
	else{
		printMessage("Unrecognised Command. Try 'help' for options\n");
	}
	wait(2);
}









// SHELL INITIALIZATION //
void initShell(){
	createProcess("shell", shell_process);
	printMessage("\t\t\t\tShell Boot Up\t\t\t---\t\t\tCompleted\n");
}