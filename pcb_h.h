#ifndef PCB_H
#define PCB_H

enum state_type {new_state, ready, running, interrupted, waiting, halted};

typedef struct pcb {
	unsigned long pid; // process PID #, a unique number
	enum state_type state;
	unsigned short priority; // priorities 0 = highest, 15 = lowest
	unsigned long pc; // holds the current pc value when preempted
} PCB;

typedef PCB * PCB_p;

PCB_p PCB_construct (void);
void PCB_deconstruct (PCB_p); // deallocates pcb from the heap
int PCB_init (PCB_p); //sets default values for member data
int PCB_set_pid (PCB_p, unsigned long pid);
unsigned long PCB_get_pid (PCB_p); // returns pid of the process
char * PCB_toString (PCB_p); // returns a string reprersenting the contents of the pcb
// declare pointer pcb_p to class pcb

#endif
