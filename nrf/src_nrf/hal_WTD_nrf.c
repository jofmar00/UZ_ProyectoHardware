
#include <stdint.h>
#include <nrf.h>
#include "hal_WTD.h"

/*** FUNCIONES ***/
//Configura un WatchDog que salta cada 'ms' segundos.
void hal_WDT_iniciar(uint32_t sec) {
	//Configuracion general del WatchDog.
  //Cuando esté durmiendo el procesador no sigue corriendo y si está halted (debug mode)tampoco
	NRF_WDT->CONFIG = (WDT_CONFIG_SLEEP_Pause << WDT_CONFIG_SLEEP_Pos) | 
										(WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos);
	
	//Configuracion del counter reload value
	//1 segundo = {CRV + 1} / 32768
	NRF_WDT->CRV = (32768 * sec) - 1; 
	
	//Configuracion del reload request register. Usaremos el 0
	NRF_WDT->RREN = (WDT_RREN_RR0_Enabled << WDT_RREN_RR0_Pos);
	
	//Empezar el WatchDog
	NRF_WDT->TASKS_START = WDT_TASKS_START_TASKS_START_Trigger;
}
void hal_WDT_alimentar(void) {
	NRF_WDT->RR[0] = 0x6E524635;
}
