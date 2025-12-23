#include<stdio.h>
#include<stdlib.h>
#include "lib/Process.h"
#include "lib/Handler.h"
#include "lib/Util.h"
#include "lib/Scheduler.h"

void bootLoader(){
	initHandler();
	Process *idle = createProcess("idle", idle_process);
	Process *init = createProcess("init", init_process);
}

void kernelLoop(){
	while(1){
		timeStep();
		schedule();
	}
}

int main(){
	bootLoader();
	kernelLoop();
	return 0;
}