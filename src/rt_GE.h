/* *****************************************************************************
 * P.H.2024: rt_GE.h
 */
 
#ifndef RT_GE
#define RT_GE

#include <stdint.h>
#include "rt_evento_t.h"

/*** FUNCIONES ***/
void rt_GE_iniciar(uint32_t monitor);
void rt_GE_lanzador();
void svc_GE_suscribir(EVENTO_T evento, void (*f_callback ) ());
void svc_GE_cancelar(EVENTO_T evento, void (*f_callback) ());
void rt_GE_tratar(EVENTO_T evento, uint32_t auxiliar);

/*** DEFINICIONES DE CONSTANTES ***/
#define rt_GE_MAX_SUSCRITOS 4
#define TIMEOUT 12000 //12 Segundos
#define NULL ((void*)0)
	
#endif