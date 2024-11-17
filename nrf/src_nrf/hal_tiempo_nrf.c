/* *****************************************************************************
 * P.H.2024: TODO
 * implementacion para cumplir el hal_tiempo.h
 */
 
 #include "hal_tiempo.h"
 #include <nrf.h>
 
 #define MAX_COUNTER_VALUE 0xFFFFFFFE
 #define HAL_TICKS2US 16 // funcionamos PCLK a 16 MHz
 static volatile uint32_t timer0_int_count = 0;
 
 
 //Rutina de servicio de interrupciones para cuando las IRQ de timer0
 void TIMER0_IRQHandler(void) {
		if (NRF_TIMER0->EVENTS_COMPARE[0]) {
        NRF_TIMER0->EVENTS_COMPARE[0] = 0;   // Limpiar la bandera de interrupción
    }
    timer0_int_count++;                  // Incrementar el contador 
 }
 /**
 * configura e inicializa la cuenta de tiempo en ticks del hardware y
 * devuelve la constante hal_ticks2us,
 * hal_ticks2us permite pasar de los ticks del hardware a microsegundos
 * (tip, el driver lo necesitara para trabajar en us y ms de la app y hacer la conversion a ticks del hardware)
 */
uint32_t hal_tiempo_iniciar_tick(){

	//CONFIGURACION INICIAL DEL TIMER0
	timer0_int_count = 0;
	
	//NRF_TIMER0->TASKS_STOP = 1; //Paramos el timer 
	
	NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos; //Ponemos BITMODE -> 16b, el contador del timer tendrá 16b
	
	NRF_TIMER0->PRESCALER = 0x0 << TIMER_PRESCALER_PRESCALER_Pos;//Configuramos el PRESCALER a 0, esto significa que frecuencia del timer será 16MHz

	NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer; //MODE -> timer
	
	NRF_TIMER0->CC[0] = MAX_COUNTER_VALUE; // Comparador -> máximo numero que el registro BITMODE puede alcanzar
	
	//TODO: PREGUNTAR EN EL LABORATORIO SI ES NECESARIO
	//Aun que no se si es necesario, cuando se llega al valor máximo y se activa COMPARE0, activar CLEAR.
	NRF_TIMER0->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
	
	NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos ;//Activar interrupciones timer cuando compare0 esté activo
	
	//TODO: No se si está bien
	NVIC_EnableIRQ(TIMER0_IRQn);

  //Empezar el timer
	NRF_TIMER0->TASKS_START = 1; 
	
	return HAL_TICKS2US;
}

/**
 * nos devuelve el numero total de ticks desde que se inicio la cuenta
 */
uint64_t hal_tiempo_actual_tick() {
	NRF_TIMER0->TASKS_CAPTURE[1] = 0x01;
	uint64_t time;
  time = (MAX_COUNTER_VALUE+1)*timer0_int_count + (uint64_t)NRF_TIMER0->CC[1]; 
	return time;
}


					/* -------------------- TIMER1 -------------------- */

/* Puntero a funcion de callback que se llamará cuando salte una IRQ*/
static void (*f_callback)();

void TIMER1_IRQHandler (void) {
	f_callback();
	if (NRF_TIMER1->EVENTS_COMPARE[0]) {
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;   // Limpiar la bandera de interrupción
  }
}

//TODO COMPROBAR QUE FUNCIONA
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)()) {
	
	f_callback = funcion_callback;
	if( periodo_en_tick != 0 ) {

		NRF_TIMER1->TASKS_CLEAR = 0x1;
		NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
		NRF_TIMER1->PRESCALER = 0x0 << TIMER_PRESCALER_PRESCALER_Pos;
		NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer; //Modo timer
		NRF_TIMER1->CC[0] = periodo_en_tick; //Pone en el registro de comparacion el valor de periodo_en_tick 
		NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos; //Cuando counter esté al max se resetea
		NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos ; //Hace saltar IRQ cuando el counter está al max
		NVIC_EnableIRQ(TIMER1_IRQn); 	//Habilitas la rutina de servicio de interrupción
		NRF_TIMER1->TASKS_START = 1; //Inicia el contador

	}
	else {
	// Detiene el temporizador
		NRF_TIMER1->TASKS_STOP = 1; 
	}
}
 