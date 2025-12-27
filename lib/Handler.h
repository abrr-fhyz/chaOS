#ifndef HANDLER_H
#define HANDLER_H

#include<stdio.h>
#include<stdlib.h>
#include "Process.h"

typedef struct Handler Handler;
Handler* processHandler;

void initHandler();
void createProcess(const char *name, processFunc entry);
void listProcesses();
void shutDown();
void schedule(int flag);
void wait(int x);

#endif

