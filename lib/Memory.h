#ifndef MEMORY_H
#define MEMORY_H

#include<stdio.h>

#include "../lib/File.h"
#include "../lib/Process.h"
#include "../lib/Util.h"
#include "../lib/Handler.h"
#include "../lib/Shell.h"

// REQUIRED STRUCT DEFINITIONS //
typedef struct Directory Directory; 
typedef struct{
	char fileName[16];
	char fileContent[1024];
	int fileSize;
}File;
typedef struct Directory{
	char directoryName[16];
	int subDirectoriesNum;
	int fileNum;
	File* files[64];
	Directory* parentDirectory;
	Directory* subDirectories[64];
}Directory;
typedef struct{
	char variableName[16];
	int value;
}Variable;
struct Process{
    int pid;
    char pName[8];
    processFunc entry;
    processState state;
    int step;
};

// SHELL MEMORY ARRAYS //
char argv[2][4096];
char cdArray[16][16];
char pathArray[16][16];
char compoundPaths[2][64];
char intructions[64][64];
char currentPath[64];
char fileName[16];
char tempLabel[16];
char labels[64][16];
int labelLineNumber[64];
int continueArgument;
int copySuccess;
int labelCnt;

// REQUIRED FILE SYSTEM CONSTANTS//
char pathPtr[256];
int varCnt;
Directory *workingDirectory;
Variable* variables[64];

// UTIL BUFFERS //
char outputBuffer[64][4096];
int outputLoaded;
int outputPrinted;
char buffer[4096];

int memoryUsed;
void mem();
void initMem();
void clearBuffer();
Variable* generateVariable();
File* generateFile();
Directory* generateDirectory();
void updateFileSize(int before, int after);
void removeDirectorySize(Directory *dir);
void removeFileSize(int size);



#endif