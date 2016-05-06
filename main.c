#include "timer.h"
#include "pcb_h.h"
#include "FIFO.h"
#include <stdbool.h>

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
	if (runningProcess->terminate != 0 && runningProcess->term_count >= runningProcess->terminate) {
		return true;
	}
	else {
		return false;
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
	PCB_set_state(currentProcess, waiting);
	FIFOq_enqueue(queue, currentProcess);
	runningProcess = FIFOq_dequeue(NULL);
	return 0;
}

void initialize() {
	
}



int main(char * args) {
	timer = new_timer(300);
	while(1) {
		runningProcess->pc++;
		if (runningProcess->pc >= runningProcess->max_pc) {
			runningProcess->pc = 0;
			runningProcess->term_count++;
		}
		if (PCB_check_terminate()) {
			//handle termination
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