#include "TEST_FIFO.h"
#include <stdio.h>

/* FUNCIONES AUXILIARES PARA TESTEAR */

static int test_fifo_encolar_extraer(void) {
    // Encolamos un evento pulsar boton y comprobamos que devuelve el mismo
    EVENTO_T evento_result;
    uint32_t auxData_result;
    Tiempo_us_t TS_result;
    printf("--- TEST FIFO ENCOLAR EXTRAER ---");
    printf("Encolando un evento PULSAR BOTON(2) con auxData 1");
    rt_FIFO_encolar(ev_PULSAR_BOTON, 1);
    rt_FIFO_extraer(&evento_result, &auxData_result, &TS_result);
    printf("Extraer nos ha devuelto un evento %d, con auxData %d en el timestamp %llu", evento_result, auxData_result, TS_result);

    rt_FIFO_vaciar();
    return (evento_result == 2) & (auxData_result == 1);
}

static void test_fifo_overflow(void){
    //printf("--- TEST FIFO OVERFLOW ---");
    for (int i = 0; i < FIFO_SIZE; i++) {
        //printf("Encolando evento %d", i);
        rt_FIFO_encolar(i % 3, i);
    }
    //printf("La cola debería estar llena, vamos a intentar encolar un elemento más");
    rt_FIFO_encolar(0,0);
    //printf("[!] NO SE DEBERÍA VER ESTE COMENTARIO, COLA DEBERIA OVERFLOWEAR");
}

static int test_fifo_estadisticas(void) {
    printf("--- TEST FIFO ESTADISTICAS ---");
    
    //ENCOLAMOS 2 EVENTOS DE TIPO VOID
    rt_FIFO_encolar(ev_VOID, 0);
    rt_FIFO_encolar(ev_VOID, 0);

    //ENCOLAMOS 5 EVENTOS DE TIPO PERIODICO1
    rt_FIFO_encolar(ev_T_PERIODICO, 0);
    rt_FIFO_encolar(ev_T_PERIODICO, 0);
    rt_FIFO_encolar(ev_T_PERIODICO, 0);
    rt_FIFO_encolar(ev_T_PERIODICO, 0);
    rt_FIFO_encolar(ev_T_PERIODICO, 0);

    //COMPROBAMOS LOS RESULTADOS
    int num_void = rt_FIFO_estadisticas(ev_VOID);
    int num_periodico = rt_FIFO_estadisticas(ev_T_PERIODICO);
    int num_boton = rt_FIFO_estadisticas(ev_PULSAR_BOTON);

    printf("NUM VOID (deben ser 2) -> %d", num_void);
    printf("NUM PERIODICO (deben ser 5) -> %d", num_periodico);
    printf("NUM_BOTON (deben ser 0) -> %d", num_boton);

    rt_FIFO_vaciar();
    return (num_void == 2) & (num_periodico == 5) & (num_boton == 0);
}


/* 
 * Funcion general de test de la cola fifo, devuelve 1 si falla algun test
*/
int test_fifo(void){
    rt_FIFO_inicializar(4);
    /*if (!test_fifo_encolar_extraer()) {
        printf("ERROR TEST ENCOLAR EXTRAER");
        return 1;
    }   
    if (!test_fifo_estadisticas()) {
        printf("ERROR TEST ESTADISTICAS");
        return 1;
    }*/
    test_fifo_overflow(); 
		return 1;
}
