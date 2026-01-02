#include "../lib/Process.h"
#include "../lib/Util.h"
#include "../lib/Shell.h"
#include "../lib/File.h"
#include "../lib/Memory.h"

void setState(Process *p, processState s){
    p->state = s;
}

processState getState(Process *p){
    return p->state;
}

void executeProcess(Process *p){
    p->entry(p);
}

void setPID(Process *p, int x){
    p->pid = x;
}

void setpName(Process *p, char *name){
    for(int i=0; i<strLen(name); i++)
        p->pName[i] = name[i];
    p->pName[strLen(name)] = '\0';
}

Process* setUpProcess(char *name, processFunc entry){
    Process *p = malloc(sizeof *p);
    setpName(p, name);
    setState(p, READY);
    p->entry = entry;
    p->step = 0;
    return p;
}

void getProcessData(Process *p){
    char *currentState;
    switch(getState(p)){
        case 0:
            currentState = "READY";
            break;
        case 1:
            currentState = "RUNNING";
            break;
        case 2:
            currentState = "BLOCKED";
            break;
        case 3:
            currentState = "TERMINATED";
            break;
        default:
            currentState = "UNKNOWN";
            break;
    }

    printProcess(p->pName, p->pid, p->step, currentState);
}

void init_process(Process *p) {
    switch (p->step) {
        case 0:
            initOutput();
            p->step++;
            return;

        case 1:
            initInput();
            p->step++;
            return;

        case 2:
            initShell();
            p->step++;
            return;

        case 3:
            initFileSystem();
            p->step++;
            return;

        case 4:
            printStartUp(1);
            p->step++;
            return;

        case 5:
            setState(p, TERMINATED);
            return;
    }
}

void idle_process(Process *p) {
    p->step++;
}

void output_process(Process *p) {
    printNext();
    p->step++;
}

void input_process(Process *p) {
    p->step++;
}

void shell_process(Process *p) {
    p->step++;
}

void fs_process(Process *p) {
    p->step++;
}

