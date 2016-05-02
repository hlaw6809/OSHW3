
#include <pthread.h>
#include <unistd.h>
#include "timer.h"

void startTimer(int milliseconds) {
	while(1) {
		usleep(milliseconds * 1000);
		pthread_mutex_lock(timerMutex);
		//Signal CPU
	}
}