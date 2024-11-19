#ifndef DRV_WTD
#define DRV_WTD
#include <stdint.h>
/*** FUNCIONES ***/
void drv_WDT_iniciar(uint32_t sec);
void drv_WDT_alimentar(void);
#endif
