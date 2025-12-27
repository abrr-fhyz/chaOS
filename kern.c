#include<stdio.h>
#include "lib/Process.h"
#include "lib/Handler.h"
#include "lib/Scheduler.h"

void bootLoader(){
	initHandler();
	createProcess("idle", idle_process);
	createProcess("init", init_process);
}

void kernelLoop(){
	while(1){
		timeStep();
		schedule(1);
	}
}

int main(){
	bootLoader();
	kernelLoop();
	return 0;
}