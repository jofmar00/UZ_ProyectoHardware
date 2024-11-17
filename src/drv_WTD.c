#include "drv_WTD.h"
#include "hal_WTD.h"

void drv_WDT_iniciar(uint32_t sec) {
	hal_WDT_iniciar(sec);
}
void drv_WDT_alimentar(void) {
	hal_WDT_alimentar();
}