#include "hal_consumo.h"
#include <LPC210x.H> 

#define DELAY_ms  500

extern void switch_to_PLL(void);

uint32_t hal_consumo_iniciar(void) {
	return 0;
}

void hal_consumo_esperar(void){
	PCON |= 0X01;
}

void hal_consumo_dormir(void){
	PCON |= 0X02;
	switch_to_PLL(); //PLL aranca a 12Mhz cuando volvemos de power down
}
