#include "svc_SC.h"

static uint32_t n_in_critical_region = 0;

/*** FUNCIONES ***/
uint32_t svc_SC_entrar(void) {
	__disable_irq();
	return n_in_critical_region++;
}

void svc_SC_salir(void) {
	n_in_critical_region--;
	if(n_in_critical_region == 0) {
		__enable_irq();
	}
}
