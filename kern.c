#include<stdio.h>
#include<stdlib.h>
#include "lib/Process.h"
#include "lib/Handler.h"
#include "lib/Util.h"
#include "lib/Scheduler.h"

void bootLoader(){
	system("cls");
	initHandler();
	Process *idle = createProcess("idle", idle_process);
	Process *init = createProcess("init", init_process);
}

void kernelLoop(){
	int t = 50;
	while(t--){
		timeStep();
		schedule();
	}
	listProcesses();
	t = 100;
	while(t--){
		timeStep();
		schedule();
	}
	listProcesses();
	t = 100;
	while(t--){
		timeStep();
		schedule();
	}
}

int main(){
	bootLoader();
	kernelLoop();
	return 0;
}