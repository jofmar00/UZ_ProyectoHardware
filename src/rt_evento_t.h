#ifndef RT_EVENTO_T
#define RT_EVENTO_T

#include <stdint.h>

/*** DEFINICIONES DE TIPOS ***/

typedef uint64_t Tiempo_us_t;

//Tipos de eventos EVENTO_T
typedef enum {
	ev_VOID = 0,
	ev_T_PERIODICO = 1,
	ev_PULSAR_BOTON = 2,
	ev_INACTIVIDAD = 3,
	ev_BOTON_RETARDO = 4,
} EVENTO_T;

//Tipo EVENTO
typedef struct {
	EVENTO_T ID_EVENTO;
	uint32_t auxData;
	Tiempo_us_t TS;
} EVENTO;


/*** DEFINICIONES DE CONSTANTES ***/
#define EVENT_TYPES 4
#define ev_NUM_EV_USUARIO 1
#define ev_USUARIO {ev_PULSAR_BOTON}
#if ev_NUM_EV_USUARIO > 0
	static const EVENTO_T user_event_list[ev_NUM_EV_USUARIO] = ev_USUARIO;
#endif
#endif
