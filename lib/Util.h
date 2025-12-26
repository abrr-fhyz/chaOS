#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include "../lib/Process.h"
#include "../lib/Handler.h"

void initOutput();
void initInput();
void printStartUp();
void printMessage(const char *msg);
void printProcess(const char* pName, int id, int step, const char* currentState);
void printLs(const char* type, const char* size, const char* name);
void printNext();
void printLast();
void waitForInput();
int compare(char *str1, char *str2);
int strLen(char *str1);
void raiseError(int x, int y);
void fileEdit(int fileIdx);

#endif