#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/Process.h"
#include "../lib/Handler.h"

Process *outputHandler;
Process *inputHandler;

void initOutput();
void initInput();
void printStartUp();
void printMessage(const char *msg);
void printProcess(const char* pName, int id, int step, const char* currentState);
void printNext();
void printLast();
void waitForInput();

#endif