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

#define RETARDO_MS 500 		//retardo blink en milisegundos
static int id_led = 0;
static int n_leds_juego = (BUTTONS_NUMBER < LEDS_NUMBER) ? BUTTONS_NUMBER : LEDS_NUMBER; //menor entre numero de botones y de leds

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

void boton_pulsar_counter_strike(EVENTO_T evento, uint32_t auxData){
	//CODIFICACION AUXDATA: 16b pin_led - 16b pin_boton
	uint32_t masc_led = 0xffff0000;
	uint32_t pin_led = (auxData & masc_led) >> 16;
	if (pin_led == id_led){
		drv_led_apagar(id_led + 1);
		//Enciende el nuevo led
		id_led = (id_led + 1) % n_leds_juego;
		//Reprograma la alarma
		svc_alarma_activar(svc_alarma_codificar(1, 300), ev_TIMEOUT_LED, id_led);
	}
}

void apagarLed() {
	
}

void siguienteLed() {
	drv_led_apagar(id_led + 1);
	id_led = (id_led + 1) % n_leds_juego;
	drv_led_encender(id_led + 1);
}


void bit_counter_strike(){
	svc_GE_suscribir(ev_PULSAR_BOTON, boton_pulsar_counter_strike);
	svc_GE_suscribir(ev_TIMEOUT_LED, apagarLed);
	svc_GE_suscribir(ev_TIMEOUT_LED, siguienteLed);
	svc_alarma_activar(svc_alarma_codificar(1, 300), ev_TIMEOUT_LED, 0); //Alarma periodica
	rt_GE_lanzador();
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
	
	/* Iniciar Drivers */
	rt_GE_iniciar(3);
	drv_tiempo_iniciar();
	drv_WDT_iniciar(21); //TODO ver si el tiempo del watchdog está bien
	drv_consumo_iniciar(1);
	drv_botones_iniciar(rt_FIFO_encolar, ev_PULSAR_BOTON, ev_BOTON_RETARDO);
	svc_alarma_iniciar(2, rt_FIFO_encolar, ev_T_PERIODICO);
	
	/*if (Num_Leds > 0){
		blink_v3_bis(3);
	}*/
	bit_counter_strike();
}
