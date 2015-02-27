#ifndef FSM_H
#define FSM_H

/**
	Forteller tilstandsmaskinen hvor heisen befinner seg
	@param floor verdien til etasjesensoren (0, 1, 2, 3 for etasje, -1 for mellom etasje)
	@param previous_floor verdien til forrige etasje heisen befant seg i
*/
void fsm_evFloorSensor(int floor, int previous_floor);



/**
	Forteller tilstandsmaskinen at stoppknappen trykkes, med informasjon om heisens lokasjon
	@param floor verdien til etasjesensoren (0, 1, 2, 3 for etasje, -1 for mellom etasje)
	@param previous_floor verdien til forrige etasje heisen befant seg i
*/
void fsm_evStopSignalPressed(int floor, int previous_floor);



/**
	Forteller tilstandsmaskinen at stoppknappen ikke trykkes med informasjon om heisens lokasjon
	@param floor verdien til etasjesensoren (0, 1, 2, 3 for etasje, -1 for mellom etasje)
*/
void fsm_evStopSignalReleased(int floor);



/**
	Forteller tilstandsmaskinen at timeren har telt ned til 0
*/
void fsm_evTimeOut();


#endif
