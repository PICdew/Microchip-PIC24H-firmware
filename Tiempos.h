/*
Proyecto: MISE Micros 16bits 2017
Fichero: Tiempos.h

Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
v0.0	Fecha: 2017-12-5
 */

#include "Main_MISE_G0_v0.h"

#define Timer_mili 8 // Define el nº Timer a usar
#define Timer2_mili 2
#define Timer3_mili 3
#define Timer4_CPU 4
//#define Flag_32bits 0

#if (Timer4_CPU == 4)
#define TMR_Timer4_CPU  TMR4
#define PR_Timer4_CPU   PR4
#define TCON_Timer4_CPU T4CON

#define _Timer4_CPU_Interrupt _T4Interrupt
#define _Timer4_CPU_IF _T4IF
#define _Timer4_CPU_IP _T4IP
#define _Timer4_CPU_IE _T4IE
#endif

#if(Timer2_mili==2)
#define TMR_Timer2_mili  TMR2
#define PR_Timer2_mili   PR2
#define TCON_Timer2_mili T2CON

#define _Timer2_mili_Interrupt _T2Interrupt
#define _Timer2_mili_IF _T2IF
#define _Timer2_mili_IP _T2IP
#define _Timer2_mili_IE _T2IE
#endif

#if (Timer3_mili == 3)
#define TMR_Timer3_mili  TMR3
#define PR_Timer3_mili   PR3
#define TCON_Timer3_mili T3CON

#define _Timer3_mili_Interrupt _T3Interrupt
#define _Timer3_mili_IF _T3IF
#define _Timer3_mili_IP _T3IP
#define _Timer3_mili_IE _T3IE
#endif

#if (Timer_mili == 1)
#define TMR_Timer_mili  TMR1
#define PR_Timer_mili   PR1
#define TCON_Timer_mili T1CON

#define _Timer_mili_Interrupt _T1Interrupt
#define _Timer_mili_IF _T1IF
#define _Timer_mili_IP _T1IP
#define _Timer_mili_IE _T1IE


#elif(Timer_mili==2)
#define TMR_Timer_mili  TMR2
#define PR_Timer_mili   PR2
#define TCON_Timer_mili T2CON

#define _Timer_mili_Interrupt _T2Interrupt
#define _Timer_mili_IF _T2IF
#define _Timer_mili_IP _T2IP
#define _Timer_mili_IE _T2IE

#elif (Timer_mili == 3)
#define TMR_Timer_mili  TMR3
#define PR_Timer_mili   PR3
#define TCON_Timer_mili T3CON

#define _Timer_mili_Interrupt _T3Interrupt
#define _Timer_mili_IF _T3IF
#define _Timer_mili_IP _T3IP
#define _Timer_mili_IE _T3IE

#elif (Timer_mili == 4)
#define TMR_Timer_mili  TMR4
#define PR_Timer_mili   PR4
#define TCON_Timer_mili T4CON

#define _Timer_mili_Interrupt _T4Interrupt
#define _Timer_mili_IF _T4IF
#define _Timer_mili_IP _T4IP
#define _Timer_mili_IE _T4IE

#elif (Timer_mili == 5)
#define TMR_Timer_mili  TMR5
#define PR_Timer_mili   PR5
#define TCON_Timer_mili T5CON

#define _Timer_mili_Interrupt _T5Interrupt
#define _Timer_mili_IF _T5IF
#define _Timer_mili_IP _T5IP
#define _Timer_mili_IE _T5IE

#elif (Timer_mili == 6)
#define TMR_Timer_mili  TMR6
#define PR_Timer_mili   PR6
#define TCON_Timer_mili T6CON

#define _Timer_mili_Interrupt _T6Interrupt
#define _Timer_mili_IF _T6IF
#define _Timer_mili_IP _T6IP
#define _Timer_mili_IE _T6IE

#elif (Timer_mili == 7)
#define TMR_Timer_mili  TMR7
#define PR_Timer_mili   PR7
#define TCON_Timer_mili T7CON

#define _Timer_mili_Interrupt _T7Interrupt
#define _Timer_mili_IF _T7IF
#define _Timer_mili_IP _T7IP
#define _Timer_mili_IE _T7IE


#elif(Timer_mili==8)
#define TMR_Timer_mili  TMR8
#define PR_Timer_mili   PR8
#define TCON_Timer_mili T8CON

#define _Timer_mili_Interrupt _T8Interrupt
#define _Timer_mili_IF _T8IF
#define _Timer_mili_IP _T8IP
#define _Timer_mili_IE _T8IE

#elif (Timer_mili == 9)

#define TMR_Timer_mili  TMR9
#define PR_Timer_mili   PR9
#define TCON_Timer_mili T9CON

#define _Timer_mili_Interrupt _T9Interrupt
#define _Timer_mili_IF _T9IF
#define _Timer_mili_IP _T9IP
#define _Timer_mili_IE _T9IE


#elif(Timer_mili==0||Timer_mili>=9)
#error ("Timer_mili NO DEFINIDO")
#endif

//===========================================
extern unsigned int milisegundos;
extern unsigned int milisegundosCPU;
extern unsigned int milisegundos2;
extern unsigned int milisegundos32;

void Inic_Tiempo_Miliseg();
void Inic_Tiempo_32bits_Miliseg();
void Inic_Tiempo_CPU (void);