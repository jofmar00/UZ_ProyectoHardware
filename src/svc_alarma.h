/* *****************************************************************************
 * P.H.2024: rt_GE.h
 */
 
#ifndef SVC_ALARMAS
#define SVC_ALARMAS

#include <stdint.h>
#include "rt_evento_t.h"



/*** DEFINICIONES DE TIPOS ***/
typedef struct {
		uint8_t activa;
    uint32_t retardo_ms;
    uint8_t periodica;
    EVENTO_T ID_evento;
    uint32_t auxData;
		uint32_t contador;
} ALARMA;

/*** FUNCIONES ***/
void svc_alarma_iniciar(uint32_t monitor_overflow, void (*f_callback)(), EVENTO_T ID_evento);
void svc_alarma_activar(uint32_t retardo_ms, EVENTO_T ID_evento, uint32_t auxData);
void svc_alarma_tratar(EVENTO_T ID_evento, uint32_t auxData);
uint32_t svc_alarma_codificar(uint32_t periodica, uint32_t retardo);

/*** DEFINICIONES DE CONSTANTES ***/
#define svc_ALARMAS_MAX 4 
#define TIEMPO_ALARMA 50
#endif
