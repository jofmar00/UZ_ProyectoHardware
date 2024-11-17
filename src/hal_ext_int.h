/* *****************************************************************************
 * Abstrae las funciones necesarias de acceso al hardware para la
 * gestión de interrupciones externas y de preparar el hardware para el dormir
 * profundo y despertar si se pulsa un botón.
 */
 
#ifndef HAL_EXT_INT
#define HAL_EXT_INT

#include <stdint.h>

/*** FUNCIONES ***/

void hal_ext_int_iniciar(uint32_t pin, void (*f_callback)());
void hal_ext_int_habilitar_int(uint32_t pin);
void hal_ext_int_deshabilitar_int(uint32_t pin);
void hal_ext_int_habilitar_despertar(uint32_t pin);
void hal_ext_int_deshabilitar_despertar(uint32_t pin);
				
#endif