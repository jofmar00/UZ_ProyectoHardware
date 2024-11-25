#ifndef RT_EVENTO_T
#define RT_EVENTO_T

#include <stdint.h>

/*** DEFINICIONES DE TIPOS ***/

typedef uint64_t Tiempo_us_t;

//Tipos de eventos EVENTO_T
typedef enum {
	ev_VOID = 0,
	ev_T_PERIODICO = 1, 				// Encola otros eventos
	ev_PULSAR_BOTON = 2,				// Actualiza seq. introducida por el user
	ev_INACTIVIDAD = 3,					// Duerme a la CPU, cada vez será mas frecuente para dar menos tiempo al user a pulsar botones
	ev_BOTON_RETARDO = 4,				// Gestiona los rebotes de los botones
	ev_SIMON_SUCCESS = 5,
	ev_SIMON_FAIL = 6,
	ev_SIMON_TRATAR_ESTADO = 7,
	ev_SIMON_EFECTO_PARPADEO = 8,
	ev_SIMON_EFECTO_CARACOL = 9,
	ev_MOSTRAR_SECUENCIA = 10,
} EVENTO_T;

//Tipo EVENTO
typedef struct {
	EVENTO_T ID_EVENTO;
	uint32_t auxData;
	Tiempo_us_t TS;
} EVENTO;


/*** DEFINICIONES DE CONSTANTES ***/
#define EVENT_TYPES 9
#define ev_NUM_EV_USUARIO 1
#define ev_USUARIO {ev_PULSAR_BOTON}
#if ev_NUM_EV_USUARIO > 0
	static const EVENTO_T user_event_list[ev_NUM_EV_USUARIO] = ev_USUARIO;
#endif
#endif
