#include "../lib/Handler.h"
#include "../lib/Util.h"
#include "../lib/Scheduler.h"

struct Handler{
    int cnt;
    Process* processList[1024];
};

void initCounter(){
    processHandler->cnt = 0;
}

void initHandler(){
    Handler *h = malloc(sizeof *h);
    processHandler = h;
    initCounter();
    schedulerInit();
}

void incrementCounter(){
    processHandler->cnt++;
}

int getCounter(){
    return processHandler->cnt;
}

void createProcess(const char *name, processFunc entry){
    Process *p = setUpProcess(name, entry);
    int pid = getCounter();
    setPID(p, pid);
    processHandler->processList[pid] = p;
    incrementCounter();
}

void listProcesses(){
    int n = processHandler->cnt;
    printMessage("Active Processes:\n________________________________________________________________________________________________________________\n");
    for(int i=0; i<n; i++){
        Process *p = processHandler->processList[i];
        getProcessData(p);
    }
}

void schedule(int flag){
    int t = getTime();
    if(!(t % 64) && flag)
        waitForInput();
    int n = processHandler->cnt;
    scheduleProcesses(processHandler->processList, n);
    runCurrentProcess(processHandler->processList);
}

void wait(int x){
    while(x--){
        timeStep();
        schedule(0);
    }
}

void shutDown(){
    printMessage("\n\nChaOS Shutting Down......");
    printLast();
    exit(0);
}
