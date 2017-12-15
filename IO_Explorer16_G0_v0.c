/*
Proyecto: MISE Micros 16bits   
Fichero: IO_Explorer16_G0_v0.c

Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
v0.0	Fecha: 2017-12-5

 *
 * Descripción:
 * Gestión de las entradas y salidas del PIC24.
 * En este fichero se gestionan los LEDS y los Pulsadores.
 */

#include "p24HJ256GP610A.h"
#include "Main_MISE_G0_v0.h"
#include "LCD_Explorer16_Gx_vx.h"
#include "IO_Explorer16_G0_v0.h"

unsigned int Pulsado_S3;
unsigned int Pulsado_S4;
unsigned int Pulsado_S5;
unsigned int Pulsado_S6;
unsigned int flagReprogramar;
unsigned int flagScroll;
unsigned int flagFrecuencia;
unsigned int flagScatter;
unsigned int flagCambiaFrecuencia;

/* LEDS Explorer16
 Led3= RA0, Led4= RA1, ... , Led10= RA7
 */

// ============================================
// Inicializa LEDS placa Explorer16: RA0, ..., RA7

/*
 * Inicializa las patas de los leds de salida y el valor inicial
 */
void Inic_Leds(void) 
{
    // -> faltan algunos

    TRIS_LED_D3 = 0; // _TRISA0= 0;    // Led_3 -> A0,  '0' -> Salida (Output) 
    TRIS_LED_D4 = 0;
    TRIS_LED_D5 = 0; // '1' -> Entrada (Input) // _TRISA3= 0;	  // Led_6 -> A3
    TRIS_LED_D6 = 0;
    TRIS_LED_D7 = 0;
    TRIS_LED_D8 = 0;
    TRIS_LED_D9 = 0;
    TRIS_LED_D10 = 0; //
    flagReprogramar = 0;

    // Los leds están conectados a tierra ->
    // -> Un '1' en la salida del PIC lo enciende
    // -> Un '0' lo Apaga
    _LATA0 = 0; // Valor de inicio LED_3 -> Apagado
    _LATA1 = 0;
    _LATA2 = 0;
    _LATA3 = 0; // Valor de inicio LED_6
    _LATA4 = 0;
    _LATA5 = 0;
    _LATA6 = 0;
    _LATA7 = 0;

} // Fin Inic_Leds

//==============================================================================
//   PULSADORES

/*
 * Inicializa los pulsadores
 */
void Inic_Pulsadores(void) {
    //  TRISDbits.TRISD6 = 1;	//1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S3 = 1; //1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S4 = 1;
    TRIS_PULSADOR_S5 = 1;
    TRIS_PULSADOR_S6 = 1;

    Pulsado_S3 = 0; // Estado despulsado
    Pulsado_S4 = 0;
    Pulsado_S5 = 0;
    Pulsado_S6 = 0;
    
    flagScroll = 0;
    flagFrecuencia = 0;
    flagScatter = 0;
    flagCambiaFrecuencia = 0;
    
    IE_PULSADOR_S3 = 1; //Habilita interrupción de S3 -> RD6
    IE_PULSADOR_S4 = 1;
    IE_PULSADOR_S5 = 1; //
    IE_PULSADOR_S6 = 1;

    _CNIP = 2; // Determina Prioridad, 1-> minima, 2 ..., 6, 7 -> maxima
    _CNIF = 0; // Borra Flag de todas la CN
    _CNIE = 1; // Habilita interrupcion general de CN

} //FIN Inic_Pulsadores_Interrupcion


/*
 * Rutina atencion interrupciones pulsadores
 */
void _ISR_NO_PSV _CNInterrupt(void) {
    
    
    if (PULSADOR_S3 == 0) {
        LED_D6 = OFF;
        LED_D3 = OFF; // _TRISA0= 0;    // Led_3 -> A0,  '0' -> Salida (Output) 
        LED_D4 = OFF;
        LED_D5 = OFF; // '1' -> Entrada (Input) // _TRISA3= 0;	  // Led_6 -> A3

        LED_D7 = OFF;
        LED_D8 = OFF;
        LED_D9 = OFF;
        LED_D10 = OFF;
    }

    if (PULSADOR_S3 == 1) {
        LED_D6 = ON;
        LED_D3 = ON; // _TRISA0= 0;    // Led_3 -> A0,  '0' -> Salida (Output) 
        LED_D4 = ON;
        LED_D5 = ON; // '1' -> Entrada (Input) // _TRISA3= 0;	  // Led_6 -> A3

        LED_D7 = ON;
        LED_D8 = ON;
        LED_D9 = ON;
        LED_D10 = ON;
    }
    
    if (PULSADOR_S5 == 0) {
        if(flagScatter ==1){
            Scatter_Continuo();
            flagScatter =0;
        }
    }

    if (PULSADOR_S5 == 1) {
       
        flagScatter =1;
        
    }
    
    if (PULSADOR_S6 == 0) {
        
         flagScroll=1;
    }

    if (PULSADOR_S6 == 1) {

        if(flagScroll==1){
            if(scroll < n_filas - 2)
                scroll++;
            else{
                scroll=0;
            }
            
            
        }
        
        flagScroll=0;
    }

    if (PULSADOR_S4 == 0) {

        if(flagCambiaFrecuencia == 1){
            flagCambiaFrecuencia=0;
            cambiarFrecuenciaOscilador();
        }
    }
    
    
    if (PULSADOR_S4 == 1) {
        //Scatter_Continuo();
        flagCambiaFrecuencia = 1;
    }



    _CNIF = 0; //Borra flag de interrupción general
} // Fin _CNInterrupt


/*
 * Rutina para reprogramar que la pata que comparten el Pulsador S5 y el LED D10 sea de entrada o de salida
 * 
 */
void Reprogramar(void) {
    if (flagReprogramar > 0) {
        TRIS_PULSADOR_S5 = 1;
        flagReprogramar = 0;
    } else {
        TRIS_LED_D10 = 0;
        flagReprogramar = 1;
    }
}


