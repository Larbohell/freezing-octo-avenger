#include "elev.h"
#include "fsm.h"
#include "timer.h"

#include <stdio.h>

void em_closeDoor(){
	elev_set_door_open_lamp(0);
}

int main(){	


	// Oppstart
	if (!elev_init()) {
        	printf("Unable to initialize elevator hardware!\n");
        	return 1;
    	}
	
	//state_INIT
	if (elev_get_floor_sensor_signal() == -1) {	
		elev_set_motor_direction(DIRN_UP);
	}
	
	queue_init();	

	int previous_floor;

	// Kjøring
	while(1){

		//previous_floor: Nyttig info om forrige etasje (som aldri vil bli -1)
		int floor_sensor = elev_get_floor_sensor_signal();
		if (floor_sensor != -1) {
			previous_floor = floor_sensor;
		}

	
		//Stoppsignal-event
		if (elev_get_stop_signal() ){
			fsm_evStopSignalPressed(elev_get_floor_sensor_signal(), previous_floor);
		} 
		else { 					//Stopsignalet slippes
			fsm_evStopSignalReleased(elev_get_floor_sensor_signal());
		}

		//Etasjesensor-event
		fsm_evFloorSensor(elev_get_floor_sensor_signal(), previous_floor);


		//Timout-event
		if (timer_isTimeOut()){
			fsm_evTimeOut(1);
			timer_reset();
		} else {
			fsm_evTimeOut(0);
		}
		
		//Etasjeindikatorlys settes
		elev_set_floor_indicator(previous_floor);
		
		//Nye bestillinger:

		queue_set_previous_floor(previous_floor);

		//Sjekker alle knapper og gir beskjed til køen om nye bestillinger (når en knapp blir trykket)
		for (int floor = 0; floor < 4; floor++) {
			for (int button = 0; button <= 2; button++) {
				if ((floor == 0 && button == BUTTON_CALL_DOWN) || (floor == 3 && button == BUTTON_CALL_UP)) {
					continue;
				}

				if (elev_get_button_signal(button, floor)) {

					queue_addToQueue(elev_get_floor_sensor_signal(), floor, button);
					elev_set_button_lamp(button, floor, 1);
				}
			}
		}
		
		
		//queue_printQueue(); //print kø

	}
	
	return 0; 
}
