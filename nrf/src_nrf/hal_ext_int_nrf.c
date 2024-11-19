/* *****************************************************************************
 * P.H.2024
 */
 
#include "rt_evento_t.h"
#include "hal_ext_int.h"
#include <nrf.h>

/*** VARIABLES ESTATICAS ***/
static void (*callback)();
//Aqui guardamos en indice i, el numero del pin del GPIOTE que está relacionado con el GPIO[i]
static int gpio_to_gpiote[32];
//Variable para ir asignando gpiotes en orden de llegada
static int gpiote_iterator = 0;

/*** FUNCIONES ***/

/* Inicia un pin como input */
void hal_ext_int_iniciar(uint32_t pin, void (*f_callback)()) {
	callback = f_callback;
	NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
															(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
															(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
															(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
															(GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
	
	//Relaciona el pin que nos pasan como parametro con un evento que saltará, manejado por el GPIOTE correspondiente
	NRF_GPIOTE->CONFIG[gpiote_iterator] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
																				(GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos ) |
																				(pin << GPIOTE_CONFIG_PSEL_Pos);
	
	//Guardamos que el gpio pin esta relacionado con el gpiote 'gpiote_iterator'
	gpio_to_gpiote[pin] = gpiote_iterator;
	gpiote_iterator++;
	//NVIC_EnableIRQ(GPIOTE_IRQn);
}
//Habilita las interrupciones de los botones
void hal_ext_int_habilitar_int(uint32_t pin){
		NRF_GPIOTE->INTENSET = 1 << gpio_to_gpiote[pin];
}
//Deshabilita las interrupciones de los botones
void hal_ext_int_deshabilitar_int(uint32_t pin){
		NRF_GPIOTE->INTENCLR = 1 << gpio_to_gpiote[pin];
}

//Habilita que el pin 'pin' pueda hacer despertar al sistema 
void hal_ext_int_habilitar_despertar(uint32_t pin) {
	NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
															(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
															(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
															(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
															(GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);
}
//Deshabilita que el pin 'pin' pueda hacer despertar al sistema
void hal_ext_int_deshabilitar_despertar(uint32_t pin) {
	NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
															(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
															(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
															(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
															(GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
}


void GPIOTE_IRQHandler() {
		int button_gpiote;
		for (int i = 0; i < 8; i++) {
			if (NRF_GPIOTE->EVENTS_IN[i] == 1) {
				NRF_GPIOTE->EVENTS_IN[i] = 0;
				button_gpiote = i;
			}  
		}
		int button_gpio;
		for (int i = 0; i < 32; i++) {
			if (gpio_to_gpiote[i] ==  button_gpiote) {
					button_gpio = i;
					break;
			}
		}
		callback(ev_PULSAR_BOTON, button_gpio);
}
