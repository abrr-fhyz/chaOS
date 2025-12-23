#include "../lib/Scheduler.h"

void schedulerInit(){
	timeSteps = 0;
	target = 0;
}

void timeStep(){
	timeSteps ++;
}

void scheduleProcesses(Process* processList[1000], int n){
	Process *prevProcess = processList[target];
	setState(prevProcess, READY);
	target = timeSteps % n;
	Process *newProcess = processList[target];
	setState(newProcess, RUNNING);
}

void runCurrentProcess(Process* processList[1000]){
	Process *currentProcess = processList[target];
	executeProcess(currentProcess);
}