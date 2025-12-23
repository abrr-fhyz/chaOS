#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
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

char outputBuffer[1000][1000];
int outputLoaded;
int outputPrinted;

#endif