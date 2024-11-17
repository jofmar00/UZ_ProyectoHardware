/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los consumos
 * suministra los servicios independientemente del hardware
 */
#ifndef DRV_CONSUMO
#define DRV_CONSUMO

#include <stdint.h>

/*** DEFINICIONES DE TIPOS ***/
typedef uint64_t Tiempo_us_t;
typedef uint32_t Tiempo_ms_t;

/*** FUNCIONES ***/
void drv_consumo_iniciar(uint32_t monitor);
void drv_consumo_esperar(void);
void drv_consumo_dormir(void);

#endif
