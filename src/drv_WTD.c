#include "drv_WTD.h"
#include "hal_WTD.h"
#include "svc_SC.h"

void drv_WDT_iniciar(uint32_t sec) {
	hal_WDT_iniciar(sec);
}
void drv_WDT_alimentar(void) {
	svc_SC_entrar();
	hal_WDT_alimentar();
	svc_SC_salir();
}
