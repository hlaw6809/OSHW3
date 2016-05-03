// check if current process need to be terminated..
bool PCB_check_terminate (PCB_p current_pcb) {
	if(current_pcb->term_count < current_pcb->terminate) {
		return false;
	}
	else if (current_pcb->term_count == current_pcb->terminate) {
		if (current_pcb->pc < current_pcb->max_pc) {
			return false;
		}
		else return true;
	}
	else {
		return true;
	}
}