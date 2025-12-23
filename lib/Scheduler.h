#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include "../lib/Process.h"

int timeSteps;
int target;
void schedulerInit();
void timeStep();
void scheduleProcesses(Process* processList[1000], int n);
void runCurrentProcess(Process* processList[1000]);

#endif

