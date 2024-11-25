#include <stdint.h>
#include "nrf.h"
#include "hal_rng.h"

static uint8_t ultimo_generado = 0;

void hal_rng_iniciar(void) {
	//Asegura que la distribucion que siguen los numeros generados es uniforme
	NRF_RNG->CONFIG = (RNG_CONFIG_DERCEN_Enabled << RNG_CONFIG_DERCEN_Pos);
	NVIC_EnableIRQ(RNG_IRQn);
}

uint8_t hal_rng_generar_numero(void) {
	//Llamamos para que genere otro numero para el siguiente que lo necesite :)
	NRF_RNG->INTENSET = 0x1;
	return ultimo_generado;
}

void hal_rng_activar_modulo(void) {
	NRF_RNG->INTENSET = 0x1;
	NRF_RNG->TASKS_START = (RNG_TASKS_START_TASKS_START_Trigger << RNG_TASKS_START_TASKS_START_Pos);
}
void hal_rng_desactivar_modulo(void) {
	NRF_RNG->INTENCLR = (RNG_INTENCLR_VALRDY_Clear << RNG_INTENCLR_VALRDY_Pos);
	NRF_RNG->TASKS_STOP = (RNG_TASKS_STOP_TASKS_STOP_Trigger << RNG_TASKS_STOP_TASKS_STOP_Pos);
}

void RNG_IRQHandler(void) {
	ultimo_generado = NRF_RNG->VALUE;
}