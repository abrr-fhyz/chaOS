#ifndef FILE_H
#define FILE_H

#include <stdio.h>

void initFileSystem();
void ls();
void mkdir(char *dirName);
void cdBack();
void cdFront(char *newDir);
char* getPath();

#endif
