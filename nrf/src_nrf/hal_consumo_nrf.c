#include "hal_consumo.h"
#include <nrf.h>

#define DELAY_ms 500

static int timer1_int_count;

uint32_t hal_consumo_iniciar(void) {
	
}

void hal_consumo_esperar(void) {
	__WFI();
}

void hal_consumo_dormir(void) {
	NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
	while(1){
		__WFE();
	}
}
