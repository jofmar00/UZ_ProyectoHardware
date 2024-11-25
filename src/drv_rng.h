#ifndef DRV_RNG
#define DRV_RNG
#include <stdint.h>

void drv_rng_iniciar();
uint64_t drv_rng_generar_numero();
#endif