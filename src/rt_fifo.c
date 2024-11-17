#include "rt_fifo.h"
#include "drv_monitor.h"
#include "drv_tiempo.h"

static uint32_t id_monitor_overflow; //Pin para la gestion del overflow de la cola de eventos

//Variables que gestionan la cola de eventos FIFO
static EVENTO fifo_array[FIFO_SIZE];
static uint32_t ultimo_encolado; //Usaremos los �ndices en -1 si la cola de eventos est� vac�a
static uint32_t por_desencolar;

static uint32_t counter[EVENT_TYPES];



/***	FUNCIONES ***/

uint32_t rt_FIFO_llena() {
	return ((ultimo_encolado + 1) % FIFO_SIZE) == por_desencolar;
}

uint32_t rt_FIFO_vacia() {
	return por_desencolar == -1;
}

/*
 * Inicializa el sistema de cola de eventos con 0 eventos en la cola
 */
void rt_FIFO_inicializar(uint32_t monitor_overflow) {
	id_monitor_overflow = monitor_overflow;
	ultimo_encolado = -1;
	por_desencolar = -1;
	for (int i = 0; i < EVENT_TYPES; i++) {
		counter[i] = 0;
	}
}

/*
 * Encola el evento con los parametros que se pasan como parametros,
 * si la cola hace overflow, marca en el monitor correspondiente y 
 * bloquea el programa.
 */
void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData) {
	
	if(rt_FIFO_llena()) {
		//OVERFLOW
		drv_monitor_marcar(id_monitor_overflow);
		while(1);
		return;
	}
	if(rt_FIFO_vacia()) {
		//Si la cola est� vac�a actualiza por_desencolar
		por_desencolar = 0;
	}
	ultimo_encolado = ((ultimo_encolado + 1) % FIFO_SIZE);
	EVENTO newEvent = {ID_evento, auxData, drv_tiempo_actual_us()};
	fifo_array[ultimo_encolado] = newEvent;
	counter[ID_evento]++;
}

/*
 * Asigna a ID_evento, auxData y TS los valores del siguiente evento a procesar
 * y devuelve la cantidad de eventos que había en la cola de eventos antes de la
 * llamada
*/ 
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData, Tiempo_us_t *TS) {
	if(rt_FIFO_vacia()) return 0;
	
	
	//Asingnacion de resultados
	*ID_evento = fifo_array[por_desencolar].ID_EVENTO;
	*auxData = fifo_array[por_desencolar].auxData;
	*TS = fifo_array[por_desencolar].TS;
	//Si vaciamos la cola
	if(por_desencolar == ultimo_encolado) {
		por_desencolar = -1;
		ultimo_encolado = -1;
		return 1;
	}
	
	por_desencolar = ((por_desencolar + 1) % FIFO_SIZE);

	//Numero de eventos que quedan por procesar
	return (por_desencolar < ultimo_encolado) ? 
					(ultimo_encolado - por_desencolar + 1) : 
					(por_desencolar - ultimo_encolado + 1);
}

/*
 * Devuelve la cantidad de veces que ID_evento se ha
 * añadido a la cola de eventos
*/
uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento) {
	if(ID_evento != ev_VOID){
		return counter[ID_evento];
	}
	uint32_t event_count = 0;
	for (int i = 0; i < EVENT_TYPES; i++) {
		event_count += counter[i];
	}
	return event_count;
}

void rt_FIFO_vaciar(void) {
	ultimo_encolado = -1;
	por_desencolar = -1;
}
