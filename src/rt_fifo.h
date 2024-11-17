#ifndef RT_FIFO
#define RT_FIFO

#include <stdint.h>
#include "rt_evento_t.h"

/*** FUNCIONES ***/
uint32_t rt_FIFO_llena();
uint32_t rt_FIFO_vacia();
void rt_FIFO_inicializar(uint32_t monitor_overflow);
void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData);
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData, Tiempo_us_t *TS);
uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento);
void rt_FIFO_vaciar(void);

/*** DEFINICIONES DE CONSTANTES ***/
#define FIFO_SIZE 64
#endif
