#ifndef HANDLER_H
#define HANDLER_H

#include<stdio.h>
#include<stdlib.h>
#include "Process.h"

typedef struct Handler Handler;
Handler* processHandler;

void initHandler();
void initCounter();
void incrementCounter();
int getCounter();
Process* createProcess(const char *name, processFunc entry);
void listProcesses();
void shutDown();

void schedule();

#endif

