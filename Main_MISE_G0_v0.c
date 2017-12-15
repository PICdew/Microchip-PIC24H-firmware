/*
Proyecto: MISE Micros 16bits 
Fichero: Main_MISE_G2_v0.c

Grupo: G2    Autores: Michael Sandoval & Raúl Ruiz
    Fecha: 2017-11-15
v0: 
    Descripción:
 *  Función principal de la aplicación
 */


#include "Main_MISE_G0_v0.h"
#include "Tiempos.h"


//#pragma config FWDTEN = OFF


//==============================================================================
unsigned char asciiCPU[3];
unsigned int consumoCPU;
unsigned long Fosc;//Frecuencia
unsigned long Fcy;//Frecuencia
unsigned int Fmult;//Multiplicador


/*
 * Rutina para pasar de Hex a INT y de INT a char.
 *   
 */
void hex2ASCIICPU_8bit(unsigned int valorHex, unsigned char ascii[]) {
    
    int millares, centenas, decenas, unidades; 
    millares = valorHex / 1000;
    centenas = (valorHex - (millares * 1000)) / 100;
    decenas = (valorHex - (millares * 1000 + centenas * 100)) / 10;
    unidades = valorHex - (millares * 1000 + centenas * 100 + decenas * 10);

    ascii[0] = centenas + '0';
    ascii[1] = decenas + '0';
    ascii[2] = unidades + '0';
}


int main(void) {
    initPIC24();

    while (1) {
        
                
        if(_Timer4_CPU_IF > 0){
            calcularUsoCPU();
        }
        
        milisegundosCPU++;
    } // Fin while(1)

    //    return 0;
} //Fin main

/*
 * Función para calcular el uso de la CPU del PIC24
 * 
 */
void calcularUsoCPU(void){
    consumoCPU = (1-((float)milisegundosCPU/(10 * (Fmult * 4000))))*100;
    hex2ASCIICPU_8bit(consumoCPU, asciiCPU);
    Ventana_Datos[0][13] = asciiCPU[0];
    Ventana_Datos[0][14] = asciiCPU[1];
    Ventana_Datos[0][15] = asciiCPU[2];
    Nop();
    Nop();
    Nop();
    milisegundosCPU = 0;
    TMR_Timer4_CPU = 0;
    _Timer4_CPU_IF = 0;
}
