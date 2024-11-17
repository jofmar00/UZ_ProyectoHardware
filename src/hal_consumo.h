/* *****************************************************************************
 * P.H.2024: hal_consumo, interface que nos independiza del hardware concreto
 */
#ifndef HAL_CONSUMO
#define HAL_CONSUMO

#include <stdint.h>

/*** FUNCIONES ***/
uint32_t hal_consumo_iniciar(void);
void hal_consumo_esperar(void);
void hal_consumo_dormir(void);

#endif
