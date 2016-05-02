// assignment 3
#include <stdio.h>
#include <stdlib.h>
#include "pcb_h.h"
#define NO_OBJECT_ERROR 1
#define NO_ERROR 0

PCB_p PCB_construct (void) {
	PCB_p raw_pcb = (PCB_p) malloc(sizeof(PCB));
	return raw_pcb;
}

void PCB_deconstruct(PCB_p raw_pcb) {
	if(!raw_pcb) {
		free(raw_pcb);
	}
}
//initialize the pcb...
int PCB_init (PCB_p raw_pcb) {
	unsigned short r = (rand() % 16);
	raw_pcb->state = ready;
	raw_pcb->priority = r;
	raw_pcb->pc = 0;
	return 0;
}

int PCB_set_pid (PCB_p raw_pcb, unsigned long pid) {
	if(!raw_pcb) {
		return NO_OBJECT_ERROR;
	}
	raw_pcb -> pid = pid;
	return NO_ERROR;
}

unsigned long PCB_get_pid (PCB_p raw_pcb) {
	if(!raw_pcb) {
		return NO_OBJECT_ERROR;
	}
	return raw_pcb -> pid;
}

char * PCB_toString (PCB_p raw_pcb) {
	char *c = (char*) malloc(sizeof(char)*50);
	if(!raw_pcb) {
		sprintf(c,"NO_OBJECT_ERROR");
		return c;
	}
	else {
		// print out the contents of pcb
		sprintf(c,"PID: 0x%lu, Priority: 0x%u, state: %d, PC: 0x%ld",raw_pcb->pid,raw_pcb->priority,raw_pcb->state,raw_pcb->pc);
		return c;
	}
}

int PCB_set_state (PCB_p raw_pcb, )
