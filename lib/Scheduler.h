#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include "../lib/Process.h"

int timeSteps;
int target;
void schedulerInit();
void timeStep();
int getTime();
void scheduleProcesses(Process* processList[1024], int n);
void runCurrentProcess(Process* processList[1024]);

#endif

