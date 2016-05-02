
typedef int IO_p;

PCB_p runningProcess;
Timer_p timer;

//Temp
IO_p io1 = 100;
IO_p io2 = 150;

int tick_IO(IO_p) {
	IO_p--;
	int interrupted = 0;
	if (IP_p == 0) {
		interrupted = 1;
		IP_p = 100;
	}
	return interrupted;
}
//End Temp

int main(char * args) {
	
	while(1) {
		(runningProcess->pc)++;
		int index;
		for (index = 0; index < 4; index++) {
			if (runningProcess->pc == runningProcess->I/O_1_Traps[index]) {
				//Handle I/O 1 trap
			} else if (runningProcess->pc == runningProcess->I/O_2_Traps[index]) {
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