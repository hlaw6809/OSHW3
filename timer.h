
#include <pthread.h>

pthread_mutext_t timerMutext = PTHREAD_MUTEX_INITIALIZER;

void startTimer(int milliseconds);