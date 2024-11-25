 #include <stdint.h>
 
 #include "board.h"
 #include "drv_leds.h"
 #include "drv_consumo.h"
 #include "drv_tiempo.h"
 #include "drv_rng.h"
 #include "rt_evento_t.h"
 #include "simon.h"
 #include "svc_alarma.h"
 #include "rt_GE.h"
 #include "rt_FIFO.h"
 
 
 static _GameState estado;
 static uint32_t tiempo_referencia_inicial = 1000;
 static uint32_t ronda;
 static uint32_t dificultad = 1;
 static uint32_t num_botones_pulsados;
 static uint32_t user_input[MAX_TURNOS_SIMON];
 static uint64_t secuencia_random;

 
 
 
 /*** FUNCIONES AUXILIARES ***/
void simon_leds_efecto_parpadeo(void) {
	static int contador_blink_success = 0;
	for(int i = 0; i < LEDS_NUMBER; i++) {
			drv_led_conmutar(i);
	}
	if (contador_blink_success < 3) {
			svc_alarma_activar(tiempo_referencia_inicial / dificultad, ev_SIMON_EFECTO_PARPADEO, 0);
	}
	contador_blink_success = (contador_blink_success + 1) % 4;
}


void simon_leds_efecto_caracol(void) {
	static int contador_blink_efecto_caracol = 0;
	static int orden_leds_efecto_caracol[] = {1, 3, 4, 2};
	
	drv_led_conmutar(orden_leds_efecto_caracol[contador_blink_efecto_caracol % LEDS_NUMBER]);
	if (contador_blink_efecto_caracol < LEDS_NUMBER*4) {
			svc_alarma_activar(300, ev_SIMON_EFECTO_CARACOL, 0);
			contador_blink_efecto_caracol++;
	}	
	else {
		contador_blink_efecto_caracol = 0;
	}
}


/* Funcion que suscribiremos al evento ev_PULSAR_BOTON para 
 * ir guardando las pulsaciones del usuario en memoria */
void simon_tratar_botones( EVENTO_T evento, uint32_t auxData) {
	if (evento != ev_BOTON_RETARDO) return;
	
	
}

/* Obtiene el par de bits en la posicion 'posicion' * 2*/
uint32_t simon_obtener_num_actual(uint64_t secuencia, uint32_t posicion) {
			uint32_t secuencia_shifted = secuencia >> (posicion * 2);
			return secuencia_shifted & 0b11;
}

void simon_mostrar_secuencia_actual() {
	static uint32_t contador_mostrar_secuencia = 0;
	static uint32_t num_actual = 0;
	static uint32_t num_siguiente = 0;
	
	num_actual = simon_obtener_num_actual(secuencia_random, contador_mostrar_secuencia);
	
	if(contador_mostrar_secuencia < ronda) {
		num_siguiente = simon_obtener_num_actual(secuencia_random, contador_mostrar_secuencia);
		drv_led_conmutar(num_actual);
		drv_led_conmutar(num_siguiente);
		svc_alarma_activar(tiempo_referencia_inicial/dificultad, ev_MOSTRAR_SECUENCIA, 0);
	}
	else {
		drv_led_apagar(num_actual);
	}
 }
 
 uint64_t simon_generar_secuencia(){
		drv_rng_iniciar();
		return drv_rng_generar_numero();
 }
	 
 
 /*** ESTADOS DE SIMON ***/
 
 /* Inicia el juego del simon */
 void simon_estado_iniciar() {
	dificultad = 1;
	ronda = 0;
	secuencia_random = simon_generar_secuencia();
 }
 
 /* Muestra la secuencia hasta la ronda 'ronda', va mostrando 
  * la secuencia de numeros cada vez más rapido */
 void simon_estado_show_sequence(uint32_t ronda) {
	 simon_mostrar_secuencia_actual();
	 rt_FIFO_encolar(e_WAIT_FOR_INPUT,0);	
 }
 
 /* Comprueba la entrada del usuario y devuelve
	* 1 si la secuencia es correcta y 0 si es incorrecta*/
 uint32_t simon_estado_wait_for_input() {
		//TODO
 }
 
 /* Muestra un doble parpadeo de todos los led como
	  muestra de que la secuencia introducida es correcta */
 void simon_estado_success() {
		svc_alarma_activar(300, ev_SIMON_SUCCESS, 0);
 }
 
 /* Muestra un encendido como en espiral para mostrar que la secuencia 
  * introducida es incorrecta y se queda dormido */
 void simon_estado_fail() {
		for (int i = 0; i < (LEDS_NUMBER * 2); i++) {
			drv_led_encender(i % LEDS_NUMBER);
			//TODO AVERIGUAR WAIT 0.3
			drv_led_apagar(i % LEDS_NUMBER);
			//TODO AVERIGUAR WAIT 0.3
		}
		drv_consumo_dormir();
 }
 
 /*** SIMON TRATAR ***/
 void simon_tratar (EVENTO_T evento, uint32_t auxData) {
		switch(estado) {
			case 	e_INIT:
				simon_estado_iniciar();
				estado = e_SHOW_SEQUENCE;
			break;
			case e_SHOW_SEQUENCE:
				simon_estado_show_sequence(auxData);
				estado = e_WAIT_FOR_INPUT;
			break;
			case e_WAIT_FOR_INPUT:{
				uint32_t error = simon_estado_wait_for_input();
				if (error) {
					estado = e_FAIL;
				}
				else {
					estado = e_SUCCESS;
				}
			}
			break;
			case e_SUCCESS:
				simon_estado_success();
				estado = e_SHOW_SEQUENCE;
			break;
			case e_FAIL:
				simon_estado_fail();
			break;
		}
 }
 
 /*** FUNCION PRINCIPAL ***/
  void simon_iniciar() {
		
		svc_GE_suscribir(ev_SIMON_SUCCESS, simon_leds_conmutar_todos);
		svc_GE_suscribir(ev_SIMON_FAIL, simon_leds_conmutar_uno)
	}
	