#ifndef HAL_WTD
#define HAL_WTD
#include <stdint.h>

/*** FUNCIONES ***/
void hal_WDT_iniciar(uint32_t sec);
void hal_WDT_alimentar(void);

#endif