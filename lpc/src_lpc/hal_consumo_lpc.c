#include "hal_consumo.h"
#include <LPC210x.H> 

#define DELAY_ms  500

uint32_t hal_consumo_iniciar(void) {
	return 0;
}

void hal_consumo_esperar(void){
	EXTWAKE = 1;
	PCON |= 0X01;
}

void hal_consumo_dormir(void){
	/*EXTWAKE = 1;
	PCON |= 0X02;
	switch_to_PLL();*/
}
