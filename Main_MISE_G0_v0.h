/* 
Proyecto:
Fichero: Main_MISE_G0_v0.h

Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
v0.0	Fecha: 2017-12-5
 */
  
#include "p24HJ256GP610A.h"

//#define _80or8 0 //Seleccionar 0 para 80MHz o 1 para 8 MHz
//
//#if(_80or8 == 0 )
//    #define Fosc    80000000    //Frecuencia reloj CPU 
//    #define Multiplier 10
//
//#endif
//#if (_80or8 == 1 )
//    #define Fosc    8000000    //Frecuencia reloj CPU 
//    #define Multiplier 1
//#endif

#define F80Mhz  80000000
#define F8Mhz   8000000

#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))

#define long_ventana 19
//unsigned char Ventana_LCD[4][long_ventana] __attribute__((space(dma) ));
//#define Ventana_RAM_LCD Ventana_LCD[][] )); //,aligned (128)
#define n_filas 6
#define n_columnas 19



extern unsigned char Ventana_Datos[n_filas][n_columnas] __attribute__((space(dma)));
extern unsigned long Fosc;
extern unsigned long Fcy;
extern unsigned int Fmult;
void Copia_FLASH_RAM(unsigned char *texto);
void initPingPong(void);