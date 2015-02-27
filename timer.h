#ifndef TIMER_H
#define TIMER_H

/**
	Starter timer
*/
void timer_start();



/**
	Sier fra når tiden er ute
	@return 1 hvis tiden siden siste kall av timer_start() er >= 3 sek
*/
int timer_isTimeOut();


/**
	Skrur av timeren, timer ikke aktiv før timer_start() kalles igjen
*/
void timer_reset();

#endif

