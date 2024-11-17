#include "drv_monitor.h"
#include "hal_gpio.h"
#include "board.h"

#if MONITOR_NUMBER > 0
	static const uint8_t monitor_list[MONITOR_NUMBER] = MONITORS_LIST;
#endif

void drv_monitor_marcar(uint32_t id) {
	hal_gpio_escribir(monitor_list[id - 1], MONITOR_ACTIVE_STATE);
}

void drv_monitor_desmarcar(uint32_t id) {
	hal_gpio_escribir(monitor_list[id - 1], ~MONITOR_ACTIVE_STATE);
}
