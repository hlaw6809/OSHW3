// check if current process need to be terminated..
bool PCB_check_terminate (PCB_p current_pcb) {
	if(cpu->current_pcb->term_count < cpu->current_pcb->terminate) {
		return false;
	}
	else if (cpu->current_pcb->term_count == cpu->current_pcb->terminate) {
		if (cpu->current_pcb->pc < cpu->current_pcb->max_pc) {
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

int io_trap_handler(PCB_p current_pcb) {
	PCB_set_state(cpu->current_pcb, waiting);
	FIFOq_enqueue(cpu->waiting_q,current_pcb);
	cpu->current_pcb = FIFOq_dequeue(cpu->running_q);
	return SUCCESS;
}