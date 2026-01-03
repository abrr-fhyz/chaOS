#include "../lib/Memory.h"

int reserved = 79808;
int totalUse = 20;
int mb = (1024*1024);

void clearBuffer(){
	for(int i=0; i<4096; i++)
		buffer[i] = '\0';
}

int calculateAvailable(){
	return totalUse - reserved - memoryUsed;
}

void mem(){
	printMessage(
		"##########################################\n"
		"#\t\tMEMORY STATUS\t\t #\n"
		"##########################################\n\n");
	clearBuffer();
	int available = calculateAvailable();
	float mb1 = (float) reserved / (float) mb;
	float mb2 = (float) memoryUsed / (float) mb;
	float mb3 = (float) available / (float) mb;
	snprintf(buffer, sizeof(buffer), "- RESERVED: %d B\n\t(%f MB)\n- USED: %d B\n\t(%f MB)\n- AVAILABLE: %d B\t(%f MB)\n", reserved, mb1, memoryUsed, mb2, available, mb3);
	printMessage(buffer);

}

void initMem(){
	memoryUsed = 0;
	reserved *= 8;
	totalUse *= (1024*1024);
}

void updateFileSize(int before, int after){
	memoryUsed += after;
	memoryUsed -= before;
}

Directory* generateDirectory(){
	if(calculateAvailable() < 4096){
		return NULL;
	}
	Directory *newDir = (Directory*)malloc(sizeof(Directory));
	memoryUsed += (19*8);
	return newDir;
}

File* generateFile(){
	if(calculateAvailable() < 4096){
		return NULL;
	}
	File *newFile = (File*)malloc(sizeof(File));
	memoryUsed += (17*8);
	return newFile;
}

Variable* generateVariable(){
	if(calculateAvailable() < (32 + 16*8)){
		return NULL;
	}
	Variable *newVar = (Variable*)malloc(sizeof(Variable));
	memoryUsed += (32 + 16*8);
	return newVar;
}

void removeDirectorySize(Directory *dir){
	memoryUsed -= (19*8);
	for(int i=0; i<dir->subDirectoriesNum; i++)
		removeDirectorySize(dir->subDirectories[i]);
	for(int i=0; i<dir->fileNum; i++){
		memoryUsed -= (17*8);
		memoryUsed -= dir->files[i]->fileSize;
	}
}

void removeFileSize(int size){
	memoryUsed -= (17*8);
	memoryUsed -= size;
}