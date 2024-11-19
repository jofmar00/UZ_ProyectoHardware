/* *****************************************************************************
  * P.H.2024: Driver/Manejador de los Leds
	*
	* blink practica 2 de proyecto hardware 2024
 */
 
#include <stdint.h>
#include <stdbool.h>
#include "board.h"

#include "TEST_FIFO.h"
#include "hal_gpio.h"
#include "rt_GE.h"
#include "drv_leds.h"
#include "drv_tiempo.h"
#include "drv_consumo.h"
#include "drv_botones.h"
#include "drv_WTD.h"
#include "svc_alarma.h"

#include <stdlib.h>


#define RETARDO_MS 500 		//retardo blink en milisegundos
	

void simon_jugar() {



}

/* *****************************************************************************
 * MAIN, Programa principal.
 * para la primera sesion se debe usar la funcion de blink_v1 sin temporizadores
 * para la entrega final se debe incocar a blink_v2
 */
int main(void){
	/* Iniciar Drivers */
	drv_leds_iniciar();
	drv_tiempo_iniciar();
	drv_consumo_iniciar(1);
	drv_WDT_iniciar(21); //TODO ver si el tiempo del watchdog está bien
	drv_botones_iniciar(rt_FIFO_encolar, ev_PULSAR_BOTON, ev_BOTON_RETARDO);
	
	/* Iniciar Servicios */
	rt_GE_iniciar(1);
	svc_alarma_iniciar(2, rt_FIFO_encolar, ev_T_PERIODICO);
	
	
}
