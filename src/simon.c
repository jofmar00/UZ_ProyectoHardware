 #include <stdint.h>
 
 #include "board.h"
 #include "drv_leds.h"
 #include "drv_consumo.h"
 #include "drv_tiempo.h"
 #include "rt_evento_t.h"
 #include "simon.h"
 #include "svc_alarma.h"
 #include "rt_GE.h"
 
 
 static _GameState estado;
 static uint32_t ronda;
 static uint32_t dificultad;
 static uint32_t num_botones_pulsados;
 static uint32_t user_input[MAX_TURNOS_SIMON];
 static uint64_t secuencia_random;

 
 
 
 /*** FUNCIONES AUXILIARES ***/
void simon_leds_conmutar_todos(void) {
	static int contador_blink_success = 0;
	for(int i = 0; i < LEDS_NUMBER; i++) {
			drv_led_conmutar(i);
	}
	if (contador_blink_success < 3) {
			svc_GE_suscribir(300, simon_leds_conmutar_todos);
	}
	contador_blink_success = (contador_blink_success + 1) % 4;
}

void simon_leds_conmutar_uno(void) {
	static int contador_blink_fail = 0;
	static int orden_leds_efecto_giro[] = {1, 3, 4, 2}
	
	drv_led_conmutar(orden_leds_efecto_giro[i]);
	if (contador_blink_fail < 7) {
			svc_GE_suscribir(300, simon_leds_conmutar_todos);
	}
	contador_blink_fail = (contador_blink_fail + 1) % 4;
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

 void simon_mostrar_secuencia_actual(uint64_t secuencia, uint32_t ronda) {
	uint32_t num_actual = 0;
	 for (int i = 0; i < ronda; i++) {
			num_actual = simon_obtener_num_actual(secuencia, i);
			drv_led_encender(num_actual);
			//TODO PROGRAMAR UNA ALARMA QUE NOS DESPIERTE CADA TIEMPO/DIFICULTAD SEGUNDOS
			drv_consumo_esperar();
		}		
 }
 
 uint64_t simon_generar_secuencia(){
		//TODO IMPLEMENTAR MODULO RANDOM
		return 0;
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
	 for(int i = 0; i < ronda; i++) {
			int led_actual = simon_obtener_num_actual(secuencia_random, i);
			drv_led_encender(led_actual);
			//TODO COMPROBAR ESPERA 
			drv_led_apagar(led_actual);
	 }
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
	