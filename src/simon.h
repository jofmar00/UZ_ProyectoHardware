#ifndef SIMON
#define SIMON


/*** DEFINICIONES DE TIPOS ***/
typedef enum {
	e_INIT = 0,
	e_SHOW_SEQUENCE = 1,
	e_WAIT_FOR_INPUT = 2,
	e_SUCCESS = 3,
	e_FAIL = 4,
} _GameState;

/*** DEFINICIONES DE CONSTANTES ***/
#define MAX_TURNOS_SIMON 32

#endif