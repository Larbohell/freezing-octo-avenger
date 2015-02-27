#include "elev.h"
#include "timer.h"
#include "fsm.h"
#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//Hjelpefunksjoner
static void turn_off_all_lights();
static int getDir(int floor); //returnerer retningen heisen beveger seg i

int isTimeOut = 0;
int dirWhenStopped = 0;

typedef enum{ //tilstandene
	state_INIT,
	state_IDLE,
	state_MOVE,
	state_OPENDOOR,
	state_STOP
} TState;

static TState current_state = state_INIT;


// Entry-conditions til en state utføres som exit-conditions i forrige state

//Events: Hva skal skje ved ulike hendelser avhengig av hvilken tilstand man er i
void fsm_evFloorSensor(int floor, int previous_floor){

	switch(current_state){
		case state_INIT: {
			if (floor != -1){
				elev_set_motor_direction(DIRN_STOP);
				current_state = state_IDLE;
			}
			queue_removeAll();
			turn_off_all_lights();
			break;			
		}

		case state_STOP: {
			turn_off_all_lights();

			if (floor != -1){
				queue_removeAll();
				elev_set_door_open_lamp(1);
				current_state = state_OPENDOOR;
				timer_start();
			}
			break;
		}

		case state_IDLE: {
			if (queue_getFirst() == floor){
				current_state = state_OPENDOOR;
				elev_set_door_open_lamp(1);
				timer_start();
			}
			
			else if (!queue_isEmpty()) {
				if (floor == -1 && previous_floor == queue_getFirst()){ //Kommer til IDLE fra STOP og skal tilbake til forrige etasje den var i
					current_state = state_MOVE;
					elev_set_motor_direction(dirWhenStopped*(-1)); //Kjør i motsatt retning i forhold til før stoppknappen ble trykket
				}
				else if (floor == -1){ //Til IDLE fra STOP, men skal ikke tilbake til forrige etasje den var i
					current_state = state_MOVE;
					elev_set_motor_direction(getDir(previous_floor)); //Retning baseres da på forrige passerte etasje
				}
				else { //Vanlig IDLE (ikke fra STOP)
					current_state = state_MOVE;
					elev_set_motor_direction(getDir(floor));
				}
			}

			break;
		}

		case state_MOVE: {
			if (queue_getFirst() == floor && floor != -1){
				current_state = state_IDLE;
				elev_set_motor_direction(DIRN_STOP);
			}
			
			break;
		}
	}
}

void fsm_evStopSignalPressed(int floor, int previous_floor){
	elev_set_stop_lamp(1);	

	switch(current_state){
		case state_IDLE: {
			current_state = state_STOP;
			break;
		}
		case state_MOVE: {
			elev_set_motor_direction(DIRN_STOP);
			dirWhenStopped = getDir(previous_floor);		
			current_state = state_STOP;
			break;
		}
		case state_OPENDOOR: { 
			// BUTTON_CALL_UP finnes ikke i øverste etasje, og tilsvarende for ned-knapp i nederste etasje, må sjekke for dette
			if (floor < N_FLOORS - 1 && floor >= 0) {elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);} //Skrur av lysende i gjeldende etasje
			if (floor > 0) { elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0); }
			if (floor != -1) { elev_set_button_lamp(BUTTON_COMMAND, floor, 0); }
			

			if (queue_getFirst() == floor){
				queue_removeFirst();
			}
			
			current_state = state_STOP;
			break;
		}
	}
}

void fsm_evStopSignalReleased(int floor){
	elev_set_stop_lamp(0);

	switch(current_state){
		case state_STOP: {
			queue_removeAll();

			if (isTimeOut){
				elev_set_door_open_lamp(0);	
			}
			
			turn_off_all_lights();
			current_state = state_IDLE;
			break;
		}
		case state_OPENDOOR: {
			
			if (isTimeOut){

				if (floor < N_FLOORS - 1 && floor >= 0) {elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);}
				if (floor > 0) { elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0); }
				if (floor != -1) { elev_set_button_lamp(BUTTON_COMMAND, floor, 0); }

				if (queue_getFirst() == floor){
					queue_removeFirst();
				}
				elev_set_door_open_lamp(0);
				current_state = state_IDLE;
			}
			break;
		}
	}
}

void fsm_evTimeOut(int val){
	isTimeOut = val;
}

//Hjelpefunksjoner
static int getDir(int floor){
	int dir = queue_getFirst() - floor;
	assert(dir != 0); 
	if (dir > 0) {return 1; }  // Opp
	else {return -1;}  // Ned
}

static void turn_off_all_lights(){
	for (int floor = 0; floor < 4; floor++) {
		for (int button = 0; button <= 2; button++) {
			if ((floor == 0 && button == BUTTON_CALL_DOWN) || (floor == 3 && button == BUTTON_CALL_UP)) {
				continue;
			}			
			elev_set_button_lamp(button, floor, 0);
		}
	}
}
