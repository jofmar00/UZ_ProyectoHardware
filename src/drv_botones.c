#include "drv_botones.h"
#include "hal_ext_int.h"
#include "rt_GE.h"
#include "svc_alarma.h"
#include "hal_gpio.h"
#include "board.h"

/*** VARIABLES ESTATICAS ***/
#if BUTTONS_NUMBER > 0
	static const uint8_t button_list[BUTTONS_NUMBER] = BUTTONS_LIST;
#endif

static enum estados {REPOSO=0,ENTRANDO=1,ESPERANDO=2,SOLTANDO=3}estado_boton[BUTTONS_NUMBER];

//ESTO HAY QUE PASARLO A SIMON.C DE ALGUNA FORMA
static uint8_t n_pulsados = 0;
static uint8_t pulsados[BUTTONS_NUMBER] = {};

/*** FUNCIONES ***/

//Habilita las interrupciones de los botones y suscribe los eventos.
void drv_botones_iniciar(void (*f_callback)(), EVENTO_T evento_pulsar_boton, EVENTO_T evento_boton_retardo) {
	for (int i = 0; i < BUTTONS_NUMBER; i++) {
		hal_ext_int_iniciar(button_list[i], f_callback);
		hal_ext_int_habilitar_int(button_list[i]);
		hal_ext_int_habilitar_despertar(button_list[i]);
	}
	svc_GE_suscribir(evento_pulsar_boton, drv_botones_tratar);
	svc_GE_suscribir(evento_boton_retardo, drv_botones_tratar);
}

//Nos tienen que llamar a esta funcion con auxData siendo el gpio del boton que quieren gestionar.
void drv_botones_tratar(EVENTO_T evento, uint32_t auxData) {
	int n_boton = -1;
	for (int i = 0; i < BUTTONS_NUMBER; i++) {
		if (auxData == button_list[i]) {
			n_boton = i;
		}
	}
	if (n_boton == -1) {
		return;
	}
	switch(estado_boton[n_boton]) {
		case REPOSO:
			n_pulsados++;
			pulsados[n_boton] = 1;
			svc_alarma_activar(TRP, ev_BOTON_RETARDO, auxData); //AuxData es el numero del boton
			estado_boton[n_boton] = ENTRANDO;
		break;
		case ENTRANDO:
			svc_alarma_activar(TRP, ev_BOTON_RETARDO, auxData); //AuxData es el numero del boton
			estado_boton[n_boton] = ESPERANDO;
		break;
		case ESPERANDO:
			if( hal_gpio_leer(auxData) == BUTTONS_ACTIVE_STATE) {
					svc_alarma_activar(TEP, ev_BOTON_RETARDO, auxData);
			}
			else {
					estado_boton[n_boton] = SOLTANDO;
					svc_alarma_activar(TRD, ev_BOTON_RETARDO, auxData);
			}
		break;
		case SOLTANDO:
			n_pulsados--;
			pulsados[n_boton] = 0;
			hal_ext_int_habilitar_int(auxData);
			estado_boton[n_boton] = REPOSO;
		break;
		return;
	}
}