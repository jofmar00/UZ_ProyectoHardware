/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los consumos
 * suministra los servicios independientemente del hardware
 *
 * usa los servicos de hal_consumo.h: 
 */
#include "drv_consumo.h"
#include "hal_consumo.h"
#include "drv_monitor.h"
#include "drv_tiempo.h"

static uint32_t id_monitor;

void drv_consumo_iniciar(uint32_t monitor){
	//TODO monitor dormir
	id_monitor = monitor;
	drv_tiempo_setMonitor(id_monitor);
	drv_monitor_marcar(id_monitor);
}

void drv_consumo_esperar(void) {
	drv_monitor_desmarcar(id_monitor);
	hal_consumo_esperar();
}

void drv_consumo_dormir(void) {
	drv_monitor_desmarcar(id_monitor);
	hal_consumo_dormir();
}
