#include <stdbool.h>
#include <stdlib.h>
#include "timer.h"
#include "pcb_h.h"
#include "FIFO.h"

typedef int IO_p;

enum schedule_type {TIMER, TERMINATION, TRAP};

PCB_p runningProcess;
Timer_p timer;
FIFOq_p readyQueue;
FIFOq_p trap1WaitingQueue;
FIFOq_p trap2WaitingQueue;
FIFOq_p terminationQueue;

//Temp
IO_p io1;
IO_p io2;

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

void dispatcher() {
	runningProcess = FIFOq_dequeue(readyQueue);
	if (runningProcess != NULL) {
		PCB_set_state(runningProcess, running);
	}
}

//Add currently running proccess to ready queue and call dispatcher to dispatch next proccess.
void scheduler(enum schedule_type type) {
	if (type == TIMER) {
		PCB_set_state(runningProcess, ready);
		FIFOq_enqueue(readyQueue, runningProcess);
	} else if (type == TERMINATION) {
		PCB_set_state(runningProcess, terminated);
		FIFOq_enqueue(terminationQueue,runningProcess);
	}
	dispatcher();
}

// check if current process need to be terminated..
bool PCB_check_terminate () {
	if(runningProcess -> term_count < runningProcess->terminate) {
		return false;
	}
	else if(runningProcess->term_count >= runningProcess -> terminate) {
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
	// put running process to waiting queue
	PCB_set_state(runningProcess, waiting);
	FIFOq_enqueue(queue, runningProcess);
	scheduler(TRAP);
	// wait for a certain period of time and put the pcb to ready queue again
	// runningProcess = FIFOq_dequeue(NULL);
}

void initialize() {
	srand(time(0));

	// Randomize the IO timer values
	io1 = rand() % 100 + 300;
	io2 = rand() % 100 + 300;

	// initiate 2 pcbs as testing
	readyQueue = FIFOq_construct();
	terminationQueue = FIFOq_construct();
	trap1WaitingQueue = FIFOq_construct();
	trap2WaitingQueue = FIFOq_construct();
	int i;
	for (i=0;i<2;i++) {
		PCB_p pcb = PCB_construct();
		PCB_init(pcb);
		PCB_set_pid(pcb,i);
		printf("%s\n",PCB_toString(pcb));
		printf("%s","I/O_1 Traps Values ");
		int j;
		for (j=0;j<4;j++) {
			printf("%d ", pcb->io1_traps[j]);
		}
		printf("\n%s","I/O_2 Traps Values ");
		for (j=0;j<4;j++) {
			printf("%d ", pcb->io2_traps[j]);
		}
		FIFOq_enqueue(readyQueue,pcb);
		printf("\nReady Queue: %s\n",FIFOq_toString(readyQueue));
		printf("-----------------\n");
	}
	runningProcess = FIFOq_dequeue(readyQueue);
}


int main(int argc, char* argv[]) {
	initialize();
	timer = new_timer(300);
	int x = 10;
	while(runningProcess!=NULL) {
		(runningProcess->pc)++;
		if (runningProcess->pc >= runningProcess->max_pc) {
			runningProcess->pc = 0;
			(runningProcess->term_count)++;
			if (PCB_check_terminate()) {
				// if readyQueue is Empty
				scheduler(TERMINATION);
				if(FIFOq_is_empty(readyQueue) && runningProcess == NULL) {
					break;
				}
			}
		}
		int index;
		for (index = 0; index < 4; index++) {
			if (runningProcess->pc == runningProcess->io1_traps[index]) {
				//Handle I/O 1 trap
				//io_trap_handler(1);
			} else if (runningProcess->pc == runningProcess->io2_traps[index]) {
				//Handle I/O 2 trap
				//io_trap_handler(2);
			}
		}

		if (tick_timer(timer) == 1) {
			scheduler(TIMER);
		} else if (tick_IO(io1) == 1) {
			//Handle I/O 1 completion interrupt

			io1 = rand() % 100 + 300; // Reset random IO timer value
		} else if (tick_IO(io2) == 1) {
			//Handle I/O 2 completion interrupt

			io2 = rand() % 100 + 300; // Reset random IO timer value
		}
	}
	PCB_deconstruct(runningProcess);
	FIFOq_destruct(readyQueue);
	FIFOq_destruct(trap1WaitingQueue);
	FIFOq_destruct(trap2WaitingQueue);
	FIFOq_destruct(terminationQueue);
	printf("ENDING PROGRAM \n");
	return 0;
}
