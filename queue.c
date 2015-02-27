#include "queue.h"
#include <stdio.h>
#include <assert.h>

static const int NUMBER_OF_FLOORS = 4;
static const int SIZE = NUMBER_OF_FLOORS + 1; //Lagrer heisens sist registrerte etasje som første element i kølista, trenger derfor antall_etasjer + 1 elementer

static int queue_list[SIZE] = {0};

// Testfunksjoner:
void queue_printQueue(){
	printf("(");
	for (int i = 0; i<SIZE-1; i++){
		printf("%d, ", queue_list[i]);
	}
	printf("%d)\n", queue_list[SIZE-1]);
}

// Hjelpefunksjoner:
static int getDir(int start, int destination);
static void putInFront(int last, int floor);
static void removeFromList(int floor);

// Definisjoner:
void queue_init(){
	for (int i = 0; i < SIZE; i++){
		queue_list[i] = -2;
	}
}

void queue_set_previous_floor(int floor){
	queue_list[0] = floor;
}

int queue_getFirst(){
	return queue_list[1];
}

void queue_addToQueue(int elevator_floor, int destination_floor, elev_button_type_t button) {

	// Åpner døren dersom heisen står stille i en etasje, og en bestillingsknapp trykkes i den etasjen	
	if (queue_list[1] == -2 && destination_floor == elevator_floor) { queue_list[1] = destination_floor; }
	
	// Sjekk om heisen står i samme etasje som bestillingen, da skal ingenting lagres
	else if (destination_floor == queue_list[0] && queue_list[0] == elevator_floor) { return; } // 2a
	
	
	//Intervallsjekk:
		//Går gjennom to og to etterfølgende elementer/bestilte etasjer i lista og sjekker om bestilt etasje ligger i intervallet mellom de to
		//Heisens retning i forhold til den bestilte retningen avgjør hvordan bestillingen skal prioriteres

	for (int first = 0; first < SIZE - 1; first++){	
		
		int last = first + 1;
       
		if (destination_floor == queue_list[last]) { return; } //Bestilt etasje finnes i listen fra før, skal ikke legges til på nytt
		
		else if (queue_list[last] == -2) { //Enden av lista er nådd, bestilt etasje skal legges til bakerst i listen
			queue_list[last] = destination_floor;
			return;			
		}
		
		//Intervallsjekken
		else if ((destination_floor > queue_list[first] && destination_floor < queue_list[last]) || (destination_floor < queue_list[first] && destination_floor > queue_list[last])) { 
			if (button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN){ 
				if (getDir(queue_list[first], queue_list[last]) != button) { //Retningssjekk (heisens retning i intervallet vs bestilt retning)
					continue; //Feil retning: Sjekk neste intervall
				}
			}

			removeFromList(destination_floor); //Fjerner alle eventuelle bestillinger av samme etasje som måtte ligge i lista, duplikater er ikke lov. 
			putInFront(last, destination_floor);		
			return;	
		}
	}
}

void queue_removeFirst(){
	for (int i = 1; i < SIZE - 1; i++) {		
		queue_list[i] = queue_list[i+1];
	}
	queue_list[SIZE-1] = -2;
}

void queue_removeAll(){
	for (int i = 1; i < SIZE; i++){
		queue_list[i] = -2;
	}
}

int queue_isEmpty(){
    if (queue_list[1] == -2) {
		return 1;
	}
	return 0;
}

// Hjelpefunksjoner:

static void removeFromList(int floor){
	for (int i = 1; i < SIZE; i++){
		if (queue_list[i] == floor){
			for (int j = i; j < SIZE - 1; j++){
				queue_list[j] = queue_list[j+1];
			}
			queue_list[SIZE - 1] = -2;
			i--;
		}
	}
}

static void putInFront(int last, int floor) {
	// Går bakover i lista fra nest siste element (listen vil aldri være full hvis denne funksjonen kalles)
	
	for (int i = SIZE - 2; i > 0; i--) {
		queue_list[i+1] = queue_list[i];		
		if (i == last) {
			queue_list[i] = floor;
		}
	}
}

static int getDir(int start, int destination){
	int dir = destination - start;

	assert(dir != 0);
	if (dir > 0) { return 0; } // Opp
	else if (dir < 0) { return 1;} // Ned

}
