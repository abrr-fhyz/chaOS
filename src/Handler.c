#include "../lib/Handler.h"
#include "../lib/Util.h"
#include "../lib/Scheduler.h"

struct Handler{
    int cnt;
    Process* processList[1000];
};

void initHandler(){
    Handler *h = malloc(sizeof *h);
    processHandler = h;
    initCounter();
    schedulerInit();
}

void initCounter(){
    processHandler->cnt = 0;
}

void incrementCounter(){
    processHandler->cnt++;
}

int getCounter(){
    return processHandler->cnt;
}

Process* createProcess(const char *name, processFunc entry){
    Process *p = setUpProcess(name, entry);
    int pid = getCounter();
    setPID(p, pid);
    processHandler->processList[pid] = p;
    incrementCounter();
    return p;
}

void listProcesses(){
    int n = processHandler->cnt;
    printMessage("Active Processes:\n________________________________________________________________________________________________________________\n");
    for(int i=0; i<n; i++){
        Process *p = processHandler->processList[i];
        getProcessData(p);
    }
}

void schedule(){
    int n = processHandler->cnt;
    scheduleProcesses(processHandler->processList, n);
    runCurrentProcess(processHandler->processList);
}
