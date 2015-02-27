#include "timer.h"
#include <time.h>
#include <stdlib.h>

time_t start;
int reset = 1;

void timer_start(){
	reset = 0;
	start = time(NULL);
}

void timer_reset(){
	reset = 1;
}

int timer_isTimeOut(){
	time_t now = time(NULL);
	if (reset) { return 0; }	
	return (difftime(now, start) >= 3.0);
}
