/*
Proyecto: MISE Micros 16bits 2017
Fichero: CAD.h

Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
v0.0	Fecha: 2017-12-5
 */

//====================================================================== 
#include "p24HJ256GP610A.h"
#include "Main_MISE_G0_v0.h"
//====================================================================== 

void obtenerADCValue(unsigned int * ADCValue);
void Inic_CAD(void);
void cambiarVentana(void);
void hex2ASCII_8bit(unsigned int valorHex, unsigned char ascii[]);
void definirCanales(void);
void Inic_CAD_DMA(void);
void recibirValores(void);
void Scatter_Continuo(void);

extern unsigned int DMAbuffer[64] __attribute__((space(dma)));