#include "../lib/File.h"
#include "../lib/Process.h"
#include "../lib/Util.h"
#include "../lib/Handler.h"

typedef struct Directory Directory; 

typedef struct{
	char fileName[16];
	char fileContent[1024];
}File;

typedef struct Directory{
	char directoryName[16];
	int subDirectoriesNum;
	File* files[64];
	Directory* parentDirectory;
	Directory* subDirectories[64];
}Directory;

char pathPtr[256];
Directory *workingDirectory;

void setDirectoryName(Directory *dir, char *name){
	int i = 0;
	while(name[i] != '\0'){
		dir->directoryName[i] = name[i];
		i++;
	}
	dir->directoryName[i] = '\0';
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

void mkdir(char *dirName){
	if(workingDirectory->subDirectoriesNum >= 64){
		return;
	}
	if(strLen(dirName) == 0 || strLen(dirName) > 16){
		return;
	}
	Directory *newDir = (Directory*)malloc(sizeof(Directory));
	setDirectoryName(newDir, dirName);
	newDir->parentDirectory = workingDirectory;
	newDir->subDirectoriesNum = 0;
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
	for(int i=0; i<workingDirectory->subDirectoriesNum; i++){
		printLs("Dir", "-", workingDirectory->subDirectories[i]->directoryName);
	}
}

char* getPath(){
	return pathPtr;
}

void initFileSystem(){
	Directory *root = (Directory*)malloc(sizeof(Directory));
	setDirectoryName(root, "root");
	root->parentDirectory = NULL;
	root->subDirectoriesNum = 0;
	pathPtr[0] = '\0';
	pathAppend(root);
	workingDirectory = root;
	createProcess("fileSys", fs_process);
	printMessage("\t\t\t\tFile System Boot Up\t\t---\t\t\tCompleted\n");
}

