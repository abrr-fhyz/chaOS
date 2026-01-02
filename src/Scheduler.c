#include "../lib/Scheduler.h"
#include "../lib/Memory.h"

void schedulerInit(){
	timeSteps = 0;
	target = 0;
}

void timeStep(){
	timeSteps ++;
}

int getTime(){
	return timeSteps;
}

void scheduleProcesses(Process* processList[128], int n){
	Process *prevProcess = processList[target];
	if(getState(prevProcess) != TERMINATED)
		setState(prevProcess, READY);
	target = timeSteps % n;
	Process *newProcess = processList[target];
	setState(newProcess, RUNNING);
}

void runCurrentProcess(Process* processList[128]){
	Process *currentProcess = processList[target];
	executeProcess(currentProcess);
}