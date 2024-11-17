#ifndef DRV_BOTONES
#define DRV_BOTONES

#include "rt_evento_t.h"

/*** FUNCIONES ***/
void drv_botones_iniciar(void (*f_callback)(), EVENTO_T evento1, EVENTO_T evento2);
void drv_botones_tratar();

/*** DEFINICION DE CONSTANTES ***/
#define TRP 30
#define TEP 30
#define TRD 30
#endif