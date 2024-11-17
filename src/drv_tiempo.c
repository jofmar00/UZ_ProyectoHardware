/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * suministra los servicios independientemente del hardware
 *
 * usa los servicos de hal_tiempo.h: 
 */
 
#include "drv_tiempo.h"
#include "hal_tiempo.h"
#include "drv_monitor.h"

#define US2MS 1000

/*** VARIABLES ESTATICAS ***/
static uint32_t TICKS2US;
static uint32_t parametro_callback;
static uint32_t id_monitor;
static void (*funcion_callback_drv)();

/*** FUNCIONES ***/

/**
 * Inicializa el reloj y empieza a contar
 */
void drv_tiempo_iniciar(void){
	TICKS2US = hal_tiempo_iniciar_tick();
}

/**
 * tiempo desde que se inicio el temporizador en microsegundos
 */
Tiempo_us_t drv_tiempo_actual_us(void){
	return hal_tiempo_actual_tick() / TICKS2US;
}

/**
 * tiempo desde que se inicio el temporizador en milisegundos
 */
Tiempo_ms_t drv_tiempo_actual_ms(void){
	return hal_tiempo_actual_tick() / (US2MS * TICKS2US);
}

/**
 * retardo: esperar un cierto tiempo en milisegundos
 */
void drv_tiempo_esperar_ms(Tiempo_ms_t ms){
	Tiempo_ms_t tiempo_inicial = drv_tiempo_actual_ms();
	while (drv_tiempo_actual_ms() < tiempo_inicial + ms) {
	}
}

/**
 * esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms){
	while(drv_tiempo_actual_ms() < ms) {
	}
	return drv_tiempo_actual_ms();
}

void drv_funcion_callback() {
	funcion_callback_drv(parametro_callback);
	drv_monitor_marcar(id_monitor);
}

void drv_tiempo_setMonitor(uint32_t id) {
	id_monitor = id;
}

void drv_tiempo_periodico_ms(Tiempo_ms_t ms, void (*funcion_callback)(), uint32_t parametro_f_callback){
	funcion_callback_drv = funcion_callback;
	parametro_callback = parametro_f_callback;
	hal_tiempo_reloj_periodico_tick(ms * US2MS * TICKS2US, drv_funcion_callback);
}
