#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED
} processState;

typedef struct Process Process;
typedef void (*processFunc)(Process *);
void setPID(Process *p, int x);
Process* setUpProcess(const char *name, processFunc entry);
void getProcessData(Process *p);

void init_process(Process *p);
void idle_process(Process *p);
void output_process(Process *p);
void input_process(Process *p);
void shell_process(Process *p);
void fs_process(Process *p);

void setState(Process *p, processState s);
void executeProcess(Process *p);

#endif

