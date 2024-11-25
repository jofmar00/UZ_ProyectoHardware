#include "drv_rng.h"
#include "hal_rng.h"

void drv_rng_iniciar(void) {
	hal_rng_iniciar();
}
uint64_t drv_rng_generar_numero(void) {
	hal_rng_activar_modulo();
	uint64_t resultado = 0;
	
	//4
	uint8_t resul1 = hal_rng_generar_numero();
	uint8_t resul2 = hal_rng_generar_numero();
	uint8_t resul3 = hal_rng_generar_numero();
	uint8_t resul4 = hal_rng_generar_numero();
	
	resultado = (resul1) | (resul2 << 8) | (resul3 << 16) | (resul4 << 24);
	hal_rng_desactivar_modulo();
	
	return resultado;
}
