#ifndef FILE_H
#define FILE_H

#include <stdio.h>

void initFileSystem();

int findFileIndex(char *fileName);
int variableExists(char *name);
int getVarValue(int idx);
void setVarValue(int idx, int val);
void logContent(int idx, char *content);
char* getContent(int fileIdx);
char* getPath();

void ls();
void lsVar();
void mkdir(char *dirName);
void cdBack();
void cdFront(char *newDir);
void touch(char *fileName);
void del(char *fileName);
void deldir(char *dirName);
void edit(char *fileName);
void storeVar(char *var);
char* cat(char *fileName);



#endif
