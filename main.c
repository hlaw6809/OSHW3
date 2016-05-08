#include <stdbool.h>
#include <stdlib.h>
#include "pcb.c"
#include "FIFO.c"
#include "timer.c"
#include "timer.h"
#include "pcb_h.h"
#include "FIFO.h"

typedef int IO_p;

PCB_p runningProcess;
Timer_p timer;
FIFOq_p readyQueue;
FIFOq_p trap1WaitingQueue;
FIFOq_p trap2WaitingQueue;
FIFOq_p terminationQueue;

//Temp
IO_p io1 = 100;
IO_p io2 = 150;

int tick_IO(IO_p io) {
	io--;
	int interrupted = 0;
	if (io == 0) {
		interrupted = 1;
		io = 100;
	}
	return interrupted;
}
//End Temp

// check if current process need to be terminated..
bool PCB_check_terminate () {
	if(runningProcess -> term_count < runningProcess->terminate) {
		return false;
	}
	else if(runningProcess->term_count == runningProcess -> terminate) {
		if (runningProcess -> pc < runningProcess -> max_pc) {
			return false;
		}
		else return true;
	}
	else {
		return true;
	}
}

// trap handler passing the trap service number.
// work very similarly to the timer ISR in terms of taking the running process
// out of the state and put it into the waiting queue 
void io_trap_handler(int trapNum) {
	FIFOq_p queue;
	switch(trapNum) {
		case 1: queue = trap1WaitingQueue;
				break;
		case 2: queue = trap2WaitingQueue;
				break;
		default: return;
	}
	PCB_set_state(runningProcess, waiting);
	FIFOq_enqueue(queue, runningProcess);
	runningProcess = FIFOq_dequeue(NULL);
}

void initialize() {
	// initiate 2 pcbs as testing
	for (int i=0;i<2;i++) {
		PCB_p pcb = PCB_construct();
		PCB_init(pcb);
		PCB_set_pid(pcb,i);
		printf("%s\n",PCB_toString(pcb));
		printf("%s","I/O_1 Traps Values ");
		for (int j=0;j<4;j++) {
			printf("%d ", pcb->io1_traps[j]);
		}
		printf("%s","I/O_2 Traps Values ");
		for (int j=0;j<4;j++) {
			printf("%d ", pcb->io2_traps[j]);
		}
		printf("\n-----------------\n");
		FIFOq_enqueue(readyQueue,pcb);
	}
	runningProcess = FIFOq_dequeue(readyQueue);
}



int main(int argc,char* argv[]) {
	initialize();
	timer = new_timer(300);
	while(runningProcess!=NULL) {
		runningProcess->pc++;
		if (runningProcess->pc >= runningProcess->max_pc) {
			runningProcess->pc = 0;
			runningProcess->term_count++;
		}
		if (PCB_check_terminate()) {
			//handle termination
			FIFOq_enqueue(terminationQueue,runningProcess);
			runningProcess = FIFOq_dequeue(readyQueue);
		}
		
		int index;
		for (index = 0; index < 4; index++) {
			if (runningProcess->pc == runningProcess->io1_traps[index]) {
				//Handle I/O 1 trap
			} else if (runningProcess->pc == runningProcess->io2_traps[index]) {
				//Handle I/O 2 trap
			}
		}
			
		if (tick_timer(timer) == 1) {
			//Handle Timer Interrupt
		} else if (tick_IO(io1) == 1) {
			//Handle I/O 1 completion interrupt
		} else if (tick_IO(io2) == 1) {
			//Handle I/O 2 completion interrupt
		}
	}
	
	return 0;
}
