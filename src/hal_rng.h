#ifndef HAL_RNG
#define HAL_RNG

#include <stdint.h>

/*** FUNCIONES ***/
void hal_rng_iniciar(void);
uint8_t hal_rng_generar_numero(void);
void hal_rng_desactivar_modulo(void);
void hal_rng_activar_modulo(void);

#endif