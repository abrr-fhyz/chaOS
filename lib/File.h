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
void edit(char *fileName);
void logContent(int idx, char *content);
int findFileIndex(char *fileName);
char* cat(char *fileName);
char* getContent(char *fileName);
char* getPath();


#endif
