#include "../lib/File.h"
#include "../lib/Process.h"
#include "../lib/Util.h"
#include "../lib/Handler.h"
#include "../lib/Memory.h"









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
	updateFileSize(workingDirectory->files[idx]->fileSize, strLen(content) * 8);
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
int variableExists(char *name){
	int flag = -1;
	for(int i=0; i<varCnt; i++){
		if(compare(variables[i]->variableName, name)){
			flag = i;
			break;
		}
	}
	return flag;
}
int getVarValue(int idx){
	return variables[idx]->value;
}
void setVarValue(int idx, int val){
	variables[idx]->value = val;
}
int determineEligibility(char *name, int type){
	// 1 means elligible, 0 means not
	// type = 0 is files, type = 1 is directories
	if(type == 0){
		int idx = findFileIndex(name);
		if(idx == -1)
			return 1;
		else 
			return 0;
	} else {
		int flag = -1;
		for(int i=0; i<workingDirectory->subDirectoriesNum; i++){
			if(compare(workingDirectory->subDirectories[i]->directoryName, name)){
				flag = 1;
				break;
			}
		}
		if(flag == -1)
			return 1;
		else
			return 0;
	}
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
	if(!determineEligibility(dirName, 1)){
		printMessage("Directory already exists\n");
		return;
	}
	Directory *newDir = generateDirectory();
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
void lsVar(){
	printMessage("Type\t\tName\t\tValue\n____________________________________________________\n");
	for(int i=0; i<varCnt; i++){
		char buffer[8];
		snprintf(buffer, sizeof(buffer), "%d", variables[i]->value);
		printLs("Var", variables[i]->variableName, buffer);
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
	if(!determineEligibility(fileName, 0)){
		printMessage("File already exists\n");
		return;
	}
	File *newFile = generateFile();
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
	removeFileSize(workingDirectory->files[flag]->fileSize);
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
	removeDirectorySize(workingDirectory->subDirectories[flag]);
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
	printMessage("\n$Enter New Content: ");
	fileEdit(idx);
}
void storeVar(char *var){
	if(strLen(var) == 0 || strLen(var) > 16)
		return;
	if(contains(var, '\\') != -1){
		printMessage("Illegal Character in name: \\\n");
		return;
	}
	if(strLen(var) == 0 || strLen(var) > 16)
		return;
	Variable *newVar = generateVariable();
	for(int i=0; i<strLen(var); i++)
		newVar->variableName[i] = var[i];
	newVar->variableName[strLen(var)] = '\0';
	newVar->value = 0;
	variables[varCnt++] = newVar;
	if(varCnt == 64)
		varCnt = 1;
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
char read(char *fileName, int pos){
	int idx = findFileIndex(fileName);
	if(idx == -1){
		printMessage("Not Found");
		return '#';
	}
	char *content = getContent(idx);
	if(pos >= strLen(content)){
		printMessage("nil");
		return '#';
	}
	return content[pos];
}
void write(char *fileName, int pos, char ch){
	int idx = findFileIndex(fileName);
	if(idx == -1){
		printMessage("Not Found");
		return;
	}
	workingDirectory->files[idx]->fileContent[pos] = ch;
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
	storeVar("ANS");
	printMessage("\t\t\t\tFile System Boot Up\t\t---\t\t\tCompleted\n");
}

