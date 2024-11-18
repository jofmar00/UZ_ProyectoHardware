#include "rt_GE.h"
#include "rt_fifo.h"
#include "drv_monitor.h"
#include "drv_consumo.h"
#include "drv_WTD.h"
#include "drv_leds.h"
#include "svc_alarma.h"

	
/*** VARIABLES ESTATICAS ***/

/*
 * Array de funciones de callback para los eventos que vayamos suscribiendo y tratando,
 * cuando un evento es tratado se quita de la cola FIFO de eventos, para revocarlo del array
 * podemos usar la funcion cancelar.
*/
static void (*eventos_suscritos[EVENT_TYPES][rt_GE_MAX_SUSCRITOS]) ();
static uint32_t monitor_overflow;

/*** FUNCIONES ***/

void rt_GE_iniciar(uint32_t monitor){
		//Guardamos el monitor del módulo
    monitor_overflow = monitor;

    //Inicializa el array de funciones a NULL
    for (int i = 0; i < EVENT_TYPES; i++) {
        for (int j = 0; j < rt_GE_MAX_SUSCRITOS; j++) {
            eventos_suscritos[i][j] = NULL;
        }
    }
		
		//Suscribimos los eventos gestionados por el módulo rt_GE_tratar
		svc_GE_suscribir(ev_VOID, rt_GE_tratar);
		svc_GE_suscribir(ev_PULSAR_BOTON, rt_GE_tratar);
		svc_GE_suscribir(ev_INACTIVIDAD, rt_GE_tratar);
		svc_GE_suscribir(ev_TIMEOUT_LED, rt_GE_tratar);
}

void rt_GE_lanzador() {
	//Activamos la alarma no periodica de inactividad 
	svc_alarma_activar(svc_alarma_codificar(0, TIMEOUT), ev_INACTIVIDAD, 0);
	while (1) {
		if (!rt_FIFO_vacia()) {
			//Si hay eventos pendientes los desencolamos
			EVENTO_T evento;
			uint32_t auxData;
			Tiempo_us_t TS;
			rt_FIFO_extraer(&evento, &auxData, &TS);
			
			//Alimentamos el watchdog porque sabemos que el sistema está corriendo satisfactoriamente
			drv_WDT_alimentar();
			
			//Ejecutamos todas las funciones suscritas al evento
			for (int i = 0; i < rt_GE_MAX_SUSCRITOS; i++) {
				if(eventos_suscritos[evento][i] != NULL){
						eventos_suscritos[evento][i](evento, auxData);
				}
			} 
		}
		else {
			//ESPERAR (NO SE COMO)
			drv_consumo_esperar();
		}
	}
}


void svc_GE_suscribir(EVENTO_T evento, void (*f_callback ) ()){
    for(int i = 0; i < rt_GE_MAX_SUSCRITOS; i++) {
        if (eventos_suscritos[evento][i] == NULL) {
            eventos_suscritos[evento][i] = f_callback;
            return;
        }
    }
    //Bucle infinito por overflow
    drv_monitor_marcar(monitor_overflow);
    while (1);
}

//!!!! PREGUNTAR HAY QUE COMPACTAR EL VECTOR CUANDO ELIMINAMOS UNA FUNCION????
void svc_GE_cancelar(EVENTO_T evento, void (*f_callback) ()) {
    for(int i = 0; i < rt_GE_MAX_SUSCRITOS; i++) {
        if (eventos_suscritos[evento][i] == f_callback) {
            eventos_suscritos[evento][i] = NULL;
        }
    }
}


void rt_GE_tratar(EVENTO_T evento, uint32_t auxData) {
	//ev_T_PERIODICO se trata en srv_alarma_tratar
	switch(evento){
		case ev_INACTIVIDAD:
			drv_consumo_dormir();
		break;
		case ev_PULSAR_BOTON:
			svc_alarma_activar(svc_alarma_codificar(0, 20*1000), ev_INACTIVIDAD, 0); //Reprograma la alarma
		break;
		case ev_VOID:
		break;
		case ev_TIMEOUT_LED:
			drv_led_apagar(auxData);
		break;
		default:
		break;
	} 
}
