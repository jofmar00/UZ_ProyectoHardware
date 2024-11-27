#include <LPC210x.H> 
#include "hal_ext_int.h"
#include "hal_gpio.h"


/*** VARIABLES ESTATICAS ***/
static void (*callback)();

//Boton pin 16 - Boton 1
void eint0_ISR (void) __irq {
	callback(2, 16); //boton 16, EV_PULSAR_BOTON = 2
	hal_ext_int_deshabilitar_int(16);
	VICVectAddr = 0;        // Acknowledge Interrupt
}

//Boton pin 14 - Boton 2
void eint1_ISR (void) __irq {
	callback(2, 14); //boton 14, EV_PULSAR_BOTON = 2
	hal_ext_int_deshabilitar_int(14);
	VICVectAddr = 0;        // Acknowledge Interrupt
}

//Boton pin 15 - Boton 3
void eint2_ISR (void) __irq {
	callback(2, 15); //boton 15, EV_PULSAR_BOTON = 2
	hal_ext_int_deshabilitar_int(15);
	VICVectAddr = 0;        // Acknowledge Interrupt
}

//TODO
void hal_ext_int_iniciar(uint32_t pin, void (*f_callback)()) {
	callback = f_callback;
	//Clear interrupt flags
	EXTINT |= 0x7; //111, 1s en bits de EINT0, EINT1 y EINT2
	//VICVectAddr0 y 1 usados por timer
	VICVectAddr2 = (unsigned long)eint0_ISR;
	VICVectAddr3 = (unsigned long)eint1_ISR;
	VICVectAddr4 = (unsigned long)eint2_ISR;

	//ENABLE EXTINT0 INTERRUPT
	PINSEL1 &= 0xfffffffC;
	PINSEL1 |= 1;
	VICVectCntl2 = 0x20 | 14;
	VICIntEnable |= 1 << 14;
	
	//ENABLE EXTINT1 INTERRUPT
	PINSEL0 &= 0x0fffffff;
	PINSEL0 |= 0x20000000;
	VICVectCntl3 = 0x20 | 15;
	VICIntEnable |= 1 << 15;
	
	//ENABLE EXTINT2 INTERRUPT
	PINSEL0 |= 0x80000000;
	VICVectCntl4 = 0x20 | 16;
	VICIntEnable |= 1 << 16;
}

void hal_ext_int_habilitar_int(uint32_t pin) {
	switch(pin) {
		case 14: //EINT1
			EXTINT |= 2; //010
			PINSEL0 &= 0xCfffffff;
			PINSEL0 |= 0x20000000;
			VICIntEnable |= 1 << 15;		
		break;
		case 15: //EINT2
			EXTINT |= 4; //100
			PINSEL0 &= 0x3fffffff;
			PINSEL0 |= 0x80000000;
			VICIntEnable |= 1 << 16;
		break;
		case 16: //EINT0
			EXTINT |= 1; //001
			PINSEL1 &= 0xfffffffC;
			PINSEL1 |= 1;
			VICIntEnable |= 1 << 14;
		break;
	}
}
void hal_ext_int_deshabilitar_int(uint32_t pin) {
	switch(pin) {
		case 14: //EINT1
			VICIntEnClr |= 1 << 15;
			PINSEL0 &= 0xCfffffff;
		break;
		case 15: //EINT2
			VICIntEnClr |= 1 << 16;
			PINSEL0 &= 0x3fffffff;
		break;
		case 16: //EINT0
			VICIntEnClr |= 1 << 14;
			PINSEL1 &= 0xfffffffC;
		break;
	}
}
void hal_ext_int_habilitar_despertar(uint32_t pin) {
	switch(pin) {
		case 14: //EINT1
			EXTWAKE |= 2; //010
		break;
		case 15: //EINT2
			EXTWAKE |= 4; //100
		break;
		case 16: //EINT0
			EXTWAKE |= 1; //001
		break;
	}
}
void hal_ext_int_deshabilitar_despertar(uint32_t pin) {
	switch(pin) {
		case 14: //EINT1
			EXTWAKE &= 5; //101
		break;
		case 15: //EINT2
			EXTWAKE &= 3; //011
		break;
		case 16: //EINT0
			EXTWAKE &= 6; //110
		break;
	}
}
