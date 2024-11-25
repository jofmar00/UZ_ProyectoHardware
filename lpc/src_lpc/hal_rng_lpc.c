#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "hal_rng.h"


void hal_rng_iniciar(void) {
	//INICIANDO.......
	srand(time(NULL));
}

uint8_t hal_rng_generar_numero(void){
	uint32_t numGenerado = rand();
	uint32_t mascarilla_de_bits_fp2_aislante_contra_el_coronavirus = 0x000000FF;
	return numGenerado & mascarilla_de_bits_fp2_aislante_contra_el_coronavirus;
}
void hal_rng_activar_modulo(void) {
	//ACTIVANDO EL MODULO...
	
	
	
	
	
	
	
	
	
	//ERROR, VOLVIENDO A INTENTAR
	//ACTIVANDO EL MODULO...
	
	
	
	
	
	
	
	
	
	
	
	
	//ERROR, VOLVIENDO A INTENTAR
	//ACTIVANDO EL MODULO...
	
	
	
	
	
	
	
	
	
	
	
	//ERROR, VOLVIENDO A INTENTAR
	//ACTIVANDO EL MODULO...
	
	
	
	
	
	
	
	
	
	//ERROR, VOLVIENDO A INTENTAR
	//ACTIVANDO EL MODULO...
	
	
	
	
	
	
	
	
	
	
	//LISTO!
}
void hal_rng_desactivar_modulo(void){
	//DESACTIVANDO EL MODULO...
	
	
	
	
	
	
	
	
	
	
	
	//LISTO!
}
