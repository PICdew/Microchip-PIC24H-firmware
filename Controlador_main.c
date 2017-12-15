/*
 * Proyecto: MISE Micros 16bits 
 * File:   Controlador_main.c
 * Author: ehu
 *
 * Created on 22 de noviembre de 2017, 18:28
 * 
 * Descripción:
 * Fichero con las funciones de controlador del PIC24.
 * 
 */


#include "Main_MISE_G0_v0.h"
#include "Tiempos.h"
#include "LCD_Explorer16_Gx_vx.h"
#include "Uart.h"
#include "CAD.h"
#include "IO_Explorer16_G0_v0.h"

//unsigned char Ventana_Datos[n_filas][n_columnas];
unsigned char Ventana_Datos[n_filas][n_columnas] __attribute__((space(dma)));
void cambiarFrecuenciaOscilador(void);

const char Texto_1[]={"Uso CPU:        " "\r\n\x0"
                      "Uart:           " "\r\n\x0"
                      "Temper:         " "\r\n\x0"
                      "Potenc:         " "\r\n\x0"
                      "JoyX:           " "\r\n\x0"
                      "JoyY:           " "\x1b[H"};

//const char Texto_1[]={"-- PIC24H -- Gx " "\r\n\x0"
//                      "-Michael y Raul-" "\r\n\x0"
//                      "Temper:         " "\r\n\x0"
//                      "Potenc:         " "\r\n\x0"
//                      "JoyX:           " "\r\n\x0"
//                      "JoyY:           " "\r\n\x0"
//                      "UART:           " "\x1b[H"};

//"UART:           " "\r\n\x0"
//const char Texto_1[] = {"-- PIC24H -- Gx "
//    "Tiempos:--/--/--"};

/*
 * Copia el contenido del array Texto_1 dentro de la matriz ventana de datos;
 *  
 */
void Copia_FLASH_RAM(unsigned char *texto) {
    int i, j;
    for (i = 0; i < n_filas; i++)
        for (j = 0; j < n_columnas; j++)
            Ventana_Datos[i][j] = texto[i * n_columnas + j];

    Nop();
    Nop();
    Nop();
}

/*
 * Función para inializar el PIC24
 */
void initPIC24(void){
    Nop();
    Nop();
    //  Inic_Oscilador ();	//Seleciona Frecuencia del oscilador principal
    AD1PCFGL = 0xFFFF; // Pone todas las patas analógicas de I/O digital
    AD1PCFGH = 0xFFFF; // Por defecto las patas analógicas están habilitadas
    
    // Para la conversion AD
    //int millares,centenas,decenas,unidades;
    
    //int potenciometro_temperatura=1; //1 -> Potenciometro 0-> Temperatura

    // ========================
    // Inicializaciones 
    Nop();
    Nop();
    
    //inicialización a 80Mhz
    Fosc = F80Mhz;
    Fcy = Fosc/2;
    Fmult = 10;
    Inic_Oscilador();
    
    //Conjunto de rutinas para inicializar los componentes de la placa de desarrollo
    Inic_Leds();
    Inic_Pulsadores();
    Inic_Tiempo_Miliseg();
    Inic_Tiempo_32bits_Miliseg ();
    Copia_FLASH_RAM(Texto_1);
    Inic_LCD();
    Inic_CAD_DMA();
    intRS232_DMA();
    Inic_Tiempo_CPU();
}


/*
 * Función para cambiar la frecuencia del PIC24 de 8 a 80MHz y viceversa.
 * 
 */
void cambiarFrecuenciaOscilador(void){
    if(Fosc>=F80Mhz){
        Fmult = 1;
        Fosc = F8Mhz;
    }else{
        Fmult = 10;
        Fosc = F80Mhz;
    }
    Fcy = Fosc/2;
    Inic_Oscilador();
    Inic_Tiempo_Miliseg();
    Inic_Tiempo_32bits_Miliseg ();
    intRS232_DMA();
    Inic_Tiempo_CPU();
}