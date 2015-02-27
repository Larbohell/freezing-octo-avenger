#include "elev.h"

#ifndef QUEUE_H
#define QUEUE_H

/*
	Initialiserer kølista
*/
void queue_init();


/**
	Printer ut kølista
*/
void queue_printQueue();



/*
	Oppdaterer heisens sist registrerte etasje
	@param floor heisens sist registrerte etasje
*/
void queue_set_previous_floor(int floor);


/*
	Returnerer første etasje i bestillingskøen
	@return bestillingsetasje
*/
int queue_getFirst();


/*
	Fjerner første element i bestillingskøen
*/
void queue_removeFirst();


/*
	Fjerner alle etasjene i bestillingskøen
*/
void queue_removeAll();


/*
	Sjekker om bestillingskøen er tom
	@return 1 hvis tom, 0 hvis ikke
*/
int queue_isEmpty();


/*
	Legger til bestilling i køen, tar seg av all prioritering av bestillingene
	@param elevator_floor 
*/
void queue_addToQueue(int elevator_floor, int destination_floor, elev_button_type_t button);

#endif
