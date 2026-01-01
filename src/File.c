#include "../lib/File.h"
#include "../lib/Process.h"
#include "../lib/Util.h"
#include "../lib/Handler.h"









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









// REQUIRED ARRAYS AND CONSTANTS//
char pathPtr[256];
int varCnt;
Directory *workingDirectory;
Variable* variables[64];









// UTIL FUNCTIONS FOR FILE SYSTEM //
void setDirectoryName(Directory *dir, char *name){
	int i = 0;
	while(name[i] != '\0'){
		dir->directoryName[i] = name[i];
		i++;
	}
	dir->directoryName[i] = '\0';
}
void setFileName(File *file, char *name){
	int i = 0;
	while(name[i] != '\0'){
		file->fileName[i] = name[i];
		i++;
	}
	file->fileName[i] = '\0';
}
void pathAppend(Directory *dir){
	int cnt = 0;
	while(pathPtr[cnt] != '\0')
		cnt++;
	if(cnt != 0 && pathPtr[cnt] != '\\'){
		pathPtr[cnt] = '\\';
		cnt++;
	}
	int anchor = cnt;
	while(dir->directoryName[cnt - anchor] != '\0'){
		pathPtr[cnt] = dir->directoryName[cnt - anchor];
		cnt++;
	}
}
void removeLast(){
	int cnt = 0;
	while(pathPtr[cnt] != '\0')
		cnt++;
	if(cnt == 0) return;
	for(int i = cnt; i>=0; i--){
		if(pathPtr[i] == '\\'){
			pathPtr[i] = '\0';
			break;
		}
		pathPtr[i] = '\0';
	}
}
void logContent(int idx, char *content){
	int n = strLen(content);
	if(n > 1024){
		printMessage("File content exceeds maximum character limit");
		n = 1024;
	}
	for(int i=0; i<1024; i++)
		workingDirectory->files[idx]->fileContent[i] = '\0';
	for(int i=0; i<n; i++)
		workingDirectory->files[idx]->fileContent[i] = content[i];
	workingDirectory->files[idx]->fileSize = strLen(content) * 8;
}
int findFileIndex(char *fileName){
	int flag = -1;
	for(int i=0; i<workingDirectory->fileNum; i++){
		if(compare(fileName, workingDirectory->files[i]->fileName)){
			flag = i;
			break;
		}
	}
	return flag;
}
int findVariable(char *name){
	for(int i=0; i<varCnt; i++){
		if(compare(variables[i]->variableName, name)){
			return variables[i]->value;
		}
	}
	printMessage("Variable not found\n");
	return 0;
}
int determineEligibility(){

}
char* getPath(){
	return pathPtr;
}
char* getContent(int fileIdx){
	return workingDirectory->files[fileIdx]->fileContent;
}










// SHELL COMMAND EXECUTIONS //
void mkdir(char *dirName){
	if(workingDirectory->subDirectoriesNum >= 64)
		return;
	if(strLen(dirName) == 0 || strLen(dirName) > 16)
		return;
	if(contains(dirName, '\\') != -1){
		printMessage("Illegal Character in name: \\\n");
		return;
	}
	Directory *newDir = (Directory*)malloc(sizeof(Directory));
	setDirectoryName(newDir, dirName);
	newDir->parentDirectory = workingDirectory;
	newDir->subDirectoriesNum = 0;
	newDir->fileNum = 0;
	workingDirectory->subDirectories[workingDirectory->subDirectoriesNum] = newDir;
	workingDirectory->subDirectoriesNum++;
}
void cdBack(){
	if(workingDirectory->parentDirectory != NULL){
		workingDirectory = workingDirectory->parentDirectory;
		removeLast();
	}
}
void cdFront(char *newDir){
	int flag = -1;
	for(int i=0; i<workingDirectory->subDirectoriesNum; i++){
		if(compare(workingDirectory->subDirectories[i]->directoryName, newDir)){
			flag = i;
			break;
		}
	}
	if(flag != -1){
		workingDirectory = workingDirectory->subDirectories[flag];
		pathAppend(workingDirectory);
	}
}
void ls(){
	printMessage("Type\t\tSize\t\tName\n____________________________________________________\n");
	for(int i=0; i<workingDirectory->subDirectoriesNum; i++)
		printLs("Dir", "-", workingDirectory->subDirectories[i]->directoryName);
	for(int i=0; i<workingDirectory->fileNum; i++){
		char buffer[8];
		snprintf(buffer, sizeof(buffer), "%dB", workingDirectory->files[i]->fileSize);
		printLs("File", buffer, workingDirectory->files[i]->fileName);
	}
}
void touch(char *fileName){
	if(workingDirectory->fileNum >= 64)
		return;
	if(strLen(fileName) == 0 || strLen(fileName) > 16)
		return;
	if(contains(fileName, '\\') != -1){
		printMessage("Illegal Character in name: \\\n");
		return;
	}
	File *newFile = (File*)malloc(sizeof(File));
	setFileName(newFile, fileName);
	newFile->fileSize = 0;
	for(int i=0; i<1024; i++)
		newFile->fileContent[i] = '\0';
	workingDirectory->files[workingDirectory->fileNum] = newFile;
	workingDirectory->fileNum ++;
}
void del(char *fileName){
	int flag = findFileIndex(fileName);
	if(flag == -1){
		printMessage("Not Found");
		return;
	}
	for(int i=flag+1; i<workingDirectory->fileNum; i++)
		workingDirectory->files[i-1] = workingDirectory->files[i];
	workingDirectory->fileNum --;
	if(workingDirectory->fileNum == 0)
		workingDirectory->files[flag] = NULL;
}
void deldir(char *dirName){
	int flag = -1;
	for(int i=0; i<workingDirectory->subDirectoriesNum; i++){
		if(compare(dirName, workingDirectory->subDirectories[i]->directoryName)){
			flag = i;
			break;
		}
	}
	if(flag == -1){
		printMessage("Directory Not Found");
		return;
	}
	for(int i=flag+1; i<workingDirectory->subDirectoriesNum; i++)
		workingDirectory->subDirectories[i-1] = workingDirectory->subDirectories[i];
	workingDirectory->subDirectoriesNum --;
	if(workingDirectory->subDirectoriesNum == 0)
		workingDirectory->subDirectories[flag] = NULL;
}
void edit(char *fileName){
	int idx = findFileIndex(fileName);
	if(idx == -1){
		printMessage("Not Found");
		return;
	}
	cat(fileName);
	printMessage("\n$Enter New Content: ");
	fileEdit(idx);
}
void storeVar(char *var){
	if(contains(var, '\\') != -1){
		printMessage("Illegal Character in name: \\\n");
		return;
	}
	if(strLen(var) == 0 || strLen(var) > 16)
		return;
	Variable *newVar = (Variable*)malloc(sizeof(Variable));
	for(int i=0; i<strLen(var); i++)
		newVar->variableName[i] = var[i];
	newVar->value = 0;
	variables[varCnt++] = newVar;
	if(varCnt == 64)
		varCnt = 0;
}
char* cat(char *fileName){
	int idx = findFileIndex(fileName);
	if(idx == -1){
		printMessage("Not Found");
		return NULL;
	}
	char *content = workingDirectory->files[idx]->fileContent;
	if(content[0] == '\0'){
		content[0] = 'n';
		content[1] = 'i';
		content[2] = 'l';
	}
	return content;
}









// INITIALIZATION OF FILE SYSTEM //
void initFileSystem(){
	Directory *root = (Directory*)malloc(sizeof(Directory));
	setDirectoryName(root, "root");
	root->parentDirectory = NULL;
	root->subDirectoriesNum = 0;
	root->fileNum = 0;
	pathPtr[0] = '\0';
	pathAppend(root);
	workingDirectory = root;
	createProcess("filSys", fs_process);
	printMessage("\t\t\t\tFile System Boot Up\t\t---\t\t\tCompleted\n");
}

