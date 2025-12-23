#include "../lib/Process.h"
#include "../lib/Util.h"
#include "../lib/Shell.h"

struct Process{
    int pid;
    char pName[10];
    processFunc entry;
    processState state;
    int step;
};

void setState(Process *p, processState s){
    p->state = s;
}

void executeProcess(Process *p){
    p->entry(p);
}

void setPID(Process *p, int x){
    p->pid = x;
}

void setpName(Process *p, const char *name){
    strncpy(p->pName, name, sizeof(p->pName) - 1);
    p->pName[sizeof(p->pName) - 1] = '\0';
}

Process* setUpProcess(const char *name, processFunc entry){
    Process *p = malloc(sizeof *p);
    setpName(p, name);
    p->state = READY;
    p->entry = entry;
    p->step = 0;
    return p;
}

void getProcessData(Process *p){
    char *currentState;
    switch(p->state){
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
            printStartUp(1);
            p->step++;
            return;

        case 4:
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

