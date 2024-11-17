#include "svc_alarma.h"
#include "rt_GE.h"
#include "drv_tiempo.h"
#include "rt_fifo.h"
#include "drv_monitor.h"

/*** VARIABLES ESTATICAS ***/
static ALARMA alarm_list[svc_ALARMAS_MAX];
static uint32_t monitor;
static uint8_t num_alarmas_activas;


/*** FUNCIONES ***/


//Inicia el timer que nos despierta cada 'TIEMPO_ALARMA' ms, define un monitor
void svc_alarma_iniciar(uint32_t monitor_overflow, void (*f_callback)(), EVENTO_T ID_evento) {

    monitor = monitor_overflow;
		num_alarmas_activas = 0;
		//Inicializacion para llenar el vector de eventos VOID no activos
		for (int i = 0; i < svc_ALARMAS_MAX; i++) {
				alarm_list[i].ID_evento = ev_VOID;
				alarm_list[i].activa = 0;
		}
    
		//Hace que nos llame a 'rt_FIFO_encolar' cada TIEMPO_ALARMA ms con 'ev_T_PERIODICO'
		drv_tiempo_periodico_ms(TIEMPO_ALARMA, f_callback, ID_evento);
		
		//Define el comportamiento de las alarmas en el gestor de eventos 
		svc_GE_suscribir(ID_evento, svc_alarma_tratar);
}


void svc_alarma_activar(uint32_t retardo_ms, EVENTO_T ID_evento, uint32_t auxData){	
	//Desprogramar la alarma
	if (retardo_ms == 0) {
		for (int i = 0; i < svc_ALARMAS_MAX; i++) {
			if(alarm_list[i].ID_evento == ID_evento && alarm_list[i].auxData == auxData) {
				alarm_list[i].activa = 0;
				alarm_list[i].ID_evento = ev_VOID;
				return;
			}
		}
		return;
	}
	//OVERFLOW!
	if(num_alarmas_activas + 1 > svc_ALARMAS_MAX){
		drv_monitor_marcar(monitor);
	}
	
	//Caso normal
	uint32_t masc = 0x1 << 31;
	int periodica = (retardo_ms & masc);
	int hueco = -1;
	for (int i = 0; i < svc_ALARMAS_MAX; i++) {
		//Reprogramamos el evento
		if (alarm_list[i].ID_evento == ID_evento) {
			alarm_list[i].activa = 1;
			alarm_list[i].retardo_ms = retardo_ms & 0x7FFFFFF; //Mascara de un 0 seguido de 31 '1' para quitar el bit que indica si es periodica
			alarm_list[i].periodica = periodica;
			alarm_list[i].auxData = auxData;
			alarm_list[i].contador = retardo_ms & 0x7FFFFFF;
			return;
		}
		if (alarm_list[i].ID_evento == ev_VOID) {
			hueco = i;
		}
	}
	//Añadimos una nueva alarma al sistema
	if (hueco != -1) {
		alarm_list[hueco].ID_evento = ID_evento;
		alarm_list[hueco].activa = 1;
		alarm_list[hueco].retardo_ms = retardo_ms  & 0x7FFFFFF; //Mascara de un 0 seguido de 31 '1' para quitar el bit que indica si es periodica
		alarm_list[hueco].periodica = periodica;
		alarm_list[hueco].auxData = auxData;
		alarm_list[hueco].contador = retardo_ms  & 0x7FFFFFF;
	}
}

//Actualiza los contadores y trata alguna alarma si ha vencido su contador
void svc_alarma_tratar(EVENTO_T ID_evento, uint32_t auxData) {
		switch(ID_evento) {
			case(ev_T_PERIODICO):
					for (int i = 0; i < svc_ALARMAS_MAX; i++) {
						if(alarm_list[i].activa){
							alarm_list[i].contador-=TIEMPO_ALARMA;
							
							//Si la alarma salta
							if (alarm_list[i].contador <= 0 ) {
								rt_FIFO_encolar(alarm_list[i].ID_evento, alarm_list[i].auxData);
								//Si la alarma es periodica la reprogramamos
								if (alarm_list[i].periodica == 1) {
										alarm_list[i].contador = alarm_list[i].retardo_ms;
								}	
								//Si no es periodica la desactivamos
								else {
										alarm_list[i].activa = 0;
								}
							}
						}							
			break;
			default:
			break;
		}
	}
}

uint32_t svc_alarma_codificar(uint32_t periodica, uint32_t ms) {
		uint32_t bit_periodica = periodica << 31;
		return (bit_periodica | ms);
}