#include <LPC210x.H> 
#include "hal_ext_int.h"

//TODO
void hal_ext_int_iniciar(uint32_t pin);
void hal_ext_int_habilitar_int(uint32_t pin);
void hal_ext_int_deshabilitar_int(uint32_t pin);
void hal_ext_int_habilitar_despertar(uint32_t pin);
void hal_ext_int_deshabilitar_despertar(uint32_t pin);