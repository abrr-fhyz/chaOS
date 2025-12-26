#ifndef FILE_H
#define FILE_H

#include <stdio.h>

void initFileSystem();
void ls();
void mkdir(char *dirName);
void cdBack();
void cdFront(char *newDir);
void touch(char *fileName);
void del(char *fileName);
void cat(char *fileName);
void edit(char *fileName);
void logContent(int idx, char *content);
char* getPath();

#endif
