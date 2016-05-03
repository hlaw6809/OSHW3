#include "pcb.c"
#include "pcb_h.h"
#include <stdio.h>
int main (int argc, char* argv[]) {
	// generate four pcbs
	for (int i=0;i<10;i++) {
		PCB_p pcb = PCB_construct();
		PCB_init(pcb);
		PCB_set_pid(pcb,i);
		printf("%s",PCB_toString(pcb));
		printf("%s","\n");
		printf("%s","I/O_1 Traps Values ");
		for (int j=0;j<4;j++) {
			printf("%d ", pcb->io1_traps[j]);
		}
		printf("\n");
		printf("%s","I/O_2 Traps Values ");
		for (int j=0;j<4;j++) {
			printf("%d ", pcb->io2_traps[j]);
		}
		printf("\n-----------------\n");
	}
	return 0;
}