/*
Proyecto: MISE Micros 16bits 
Fichero: Tiempos.c

Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
v0.0	Fecha: 2017-12-5

Descripción:
 * Programa un Timer para que actualice una variable y/o 
 * genere una interrupción cada cierto tiempo
 * Hay que darle la frecuencia de reloj y el Nº de timer
 *
 * Entrada: Frecuencia, Timer
 * Salida: int microsegundos

 */

#include "p24HJ256GP610A.h"
#include "Tiempos.h"
#include "Main_MISE_G0_v0.h"


unsigned int milisegundos;
unsigned int milisegundosCPU;
unsigned int milisegundos2;
unsigned int milisegundos32;
//====================================================================================================


/*
 * TIMER de tiempo en milisegundos
 */
void Inic_Tiempo_Miliseg ()
{
    milisegundos = 0;
    milisegundos2 = 0;
	TMR_Timer_mili = 0;		// Borra valor del timer
    PR_Timer_mili = (Fmult * 4000)-1;	// Define periodo
	TCON_Timer_mili = 0x8000;	// Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_mili = 0b1000000000000000;	// Equivalente a la linea anterior
	
// Inicializa interrupción Timer 2
//  _Timer_mili_IP = 4;  // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_mili_IF = 0;  // Borra flag de interrupción
    _Timer_mili_IE = 1;  // Habilita Interrupción del Timer 2
}

/*
 * TIMER para Inicializar el timer CPU
 */
void Inic_Tiempo_CPU (void)
{
    milisegundosCPU = 0;
	TMR_Timer4_CPU = 0;		// Borra valor del timer
    PR_Timer4_CPU = 10 * ((Fmult * 4000)-1);	// Define periodo 10 milisegundos
	TCON_Timer4_CPU = 0x8000;	// Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_mili = 0b1000000000000000;	// Equivalente a la linea anterior
	
// Inicializa interrupción Timer 2
//  _Timer_mili_IP = 4;  // Prioridad por defecto (1 minima - 6 máxima)
    _Timer4_CPU_IF = 0;  // Borra flag de interrupción
    _Timer4_CPU_IE = 0;  // Habilita Interrupción del Timer 2
}

/*
 * TIMER de  32 bits
 */
 void Inic_Tiempo_32bits_Miliseg ()
{   
    TMR_Timer2_mili = 0;		// Borra valor del timer
    TMR_Timer3_mili = 0;
    PR_Timer2_mili = 5*(Fmult * 4000)-1;	// Define periodo
    PR_Timer3_mili = 1;
	TCON_Timer2_mili = 0x8008;	// Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_mili = 0b1000000000000000;	// Equivalente a la linea anterior
	
// Inicializa interrupción Timer 2
//  _Timer_mili_IP = 4;  // Prioridad por defecto (1 minima - 6 máxima)
    _Timer3_mili_IF = 0;  // Borra flag de interrupción
    _Timer3_mili_IE = 1;  // Habilita Interrupción del Timer 2
    Nop();
    Nop();
}

//========================================================
 /*
  * Rutina de interrupción para el timer en milisegundos
  */
void _ISR_NO_PSV _Timer_mili_Interrupt( void)
{
    milisegundos++ ; 
     milisegundos2++ ;    // se incrementa cada milisegundo
    _Timer_mili_IF = 0;
} // Fin T1Interrupt

//========================================================
/*
 * Rutina de interrupción del TIMER3
 */
void _ISR_NO_PSV _Timer3_mili_Interrupt( void)
{
        // se incrementa cada milisegundo
     //milisegundos32++;
     Reprogramar();
    _Timer3_mili_IF = 0;	// Borra flag de interrpción
    _Timer2_mili_IF = 0;
} // Fin T1Interrupt

//========================================================

/*
 * Rutina de interrupción del TIMER4
 */
void _ISR_NO_PSV _Timer4_CPU_Interrupt( void)
{
    Nop();
    Nop();
    Nop();
    //milisegundosCPU = 0;
    //_Timer4_CPU_IF = 0;
} // Fin T1Interrupt

