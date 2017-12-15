/*
Fichero: IO_Explorer16_G0_v0.h

Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
v0.0	Fecha: 2017-12-5

*/

// Los leds están conectados a tierra ->
// -> Un '1' en la salida del PIC lo enciende
// -> Un '0' lo Apaga

//#include "p24hj256gp610A.h"
#include "Main_MISE_G0_v0.h"


//==============================================================================
#define LED_D3      _LATA0  // LATAbits.LATA0
#define LED_D4      _LATA1
#define LED_D5      _LATA2
#define LED_D6      _LATA3
#define LED_D7      _LATA4
#define LED_D8      _LATA5
#define LED_D9      _LATA6
#define LED_D10      _LATA7


#define TRIS_LED_D3 _TRISA0 // TRISAbits.TRISA0
#define TRIS_LED_D4 _TRISA1
#define TRIS_LED_D5 _TRISA2
#define TRIS_LED_D6 _TRISA3
#define TRIS_LED_D7 _TRISA4
#define TRIS_LED_D8 _TRISA5
#define TRIS_LED_D9 _TRISA6
#define TRIS_LED_D10 _TRISA7

#define ON   1
#define OFF     0


#define PULSADOR_S3         _RD6    // PORTDbits.PORTD6
#define PULSADOR_S4         _RD13
#define PULSADOR_S5         _RA7
#define PULSADOR_S6         _RD7 

#define TRIS_PULSADOR_S3    _TRISD6
#define TRIS_PULSADOR_S4    _TRISD13
#define TRIS_PULSADOR_S5    _TRISA7
#define TRIS_PULSADOR_S6    _TRISD7

#define IE_PULSADOR_S3      _CN15IE
#define IE_PULSADOR_S4      _CN19IE
#define IE_PULSADOR_S5      _CN23IE
#define IE_PULSADOR_S6      _CN16IE

//==============================================================================
void Inic_Leds (void);
void Inic_Pulsadores (void);
void _ISR _CNInterrupt (void);
void Reprogramar(void);

