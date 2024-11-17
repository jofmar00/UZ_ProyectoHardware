/* *****************************************************************************
  * P.H.2024: Driver/Manejador de los Leds
	*
	* blink practica 2 de proyecto hardware 2024
 */
 
#include <stdint.h>
#include <stdbool.h>

#include "TEST_FIFO.h"
#include "hal_gpio.h"
#include "drv_leds.h"
#include "drv_tiempo.h"
#include "drv_consumo.h"
#include "drv_botones.h"

#define RETARDO_MS 500 		//retardo blink en milisegundos


void blink_v3_bis(uint32_t id) {
	  rt_FIFO_inicializar(1);
		drv_consumo_iniciar(3);
		drv_tiempo_periodico_ms(500, drv_led_conmutar, id);
		drv_botones_iniciar(rt_FIFO_encolar, ev_PULSAR_BOTON, ev_BOTON_RETARDO);
		//10 CONMUTACIONES
		for(int i = 0; i < 10; i++) {
			drv_consumo_esperar();
		}
		drv_tiempo_periodico_ms(0, drv_led_conmutar, id);
		drv_led_apagar(id);
		
		drv_consumo_dormir();
		drv_led_encender(id);
}

void bit_counter_strike(){
	int id_led = 0;
	drv_consumo_iniciar(1);
	drv_botones_iniciar(rt_FIFO_encolar, ev_PULSAR_BOTON, ev_BOTON_RETARDO);
	while(1) {
		drv_led_encender(id_led);
	}
}

/* *****************************************************************************
 * MAIN, Programa principal.
 * para la primera sesion se debe usar la funcion de blink_v1 sin temporizadores
 * para la entrega final se debe incocar a blink_v2
 */
int main(void){
	hal_gpio_iniciar();	// llamamos a iniciar gpio antesde que lo hagan los drivers
	
	/* Configure LED */
	uint32_t Num_Leds = drv_leds_iniciar();
	
	/* Configure Timer */
	drv_tiempo_iniciar();
	
	if (Num_Leds > 0){
		blink_v3_bis(3);
	}
}
