#include <stdint.h>
#include <LPC210x.H>
#include "hal_WTD.h"

/*** FUNCIONES ***/
//Configura un WatchDog que salta cada 'ms' segundos.
void hal_WDT_iniciar(uint32_t sec) {
	WDTC = sec * (15000000 / 4); // time-out WatchDog.
	WDMOD = 0x03; // Habilito y configuro reinicio.
	hal_WDT_alimentar();
}
void hal_WDT_alimentar(void) {
	WDFEED = 0xAA; // Alimento el WatchDog
	WDFEED = 0x55;
}