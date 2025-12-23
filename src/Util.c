#include "../lib/Util.h"

void initOutput(){
   	outputHandler = createProcess("printf", output_process);
    outputLoaded = 0;
    outputPrinted = 0;
    printStartUp(0);
}

void initInput(){
   	inputHandler = createProcess("scanf", input_process);
   	printMessage("\t\t\t\tInput Boot Up\t\t\t---\t\t\tCompleted\n");
}

void printStartUp(int start){
	if(start == 0){
		printMessage("\t\t\t\t\t\t\t|||   CHA-OS   |||\n\n");
		printMessage("\t\t\t\t\t\tBooting chaOS Operating System........\n");
		printMessage("\t\t\t\tOutput Boot Up\t\t\t---\t\t\tCompleted\n");
	} else {
		printMessage("\t\t\t\t\t\tOperating System Initialized........\n");
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
	if(outputLoaded == 1000){
		outputLoaded = 0;
	}
}

void printProcess(const char* pName, int id, int step, const char* currentState){
	char buffer[1000];
	snprintf(buffer, sizeof(buffer), "Process: %s\t\t|| PID: %d\t\t|| Steps: %d\t\t|| %s\n", pName, id, step, currentState);
	printMessage(buffer);
}

void printNext(){
	if(outputPrinted >= outputLoaded && (outputPrinted != 0 || outputLoaded != 0)) {
		return;
	}
	printf("%s", outputBuffer[outputPrinted]);
	outputPrinted++;
	if(outputPrinted == 1000){
		outputPrinted = 0;
	}
}