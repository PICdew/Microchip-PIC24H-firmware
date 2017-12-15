/*
 *  Proyecto: MISE Micros 16bits 
    Fichero: Main_MISE_G2_v0.c

    Grupo: G2    Autores: Michael Sandoval & Raúl Ruiz
    Fecha: 2017-11-15
 
    Descripción:
    Inicialización  del  CAD  basada  
    en  el  ejemplo  3,  para  el  pot
    enciómetro  y  el  sensor  de  
    temperatura de la placa Explorer 16.
 */


#include "CAD.h"

unsigned int CADFlag;
unsigned int ADCValue;
unsigned int sensorTemperatura, potenciometro, acum[4];
unsigned char valorPotenciometroASCII[3];
int cnt;
int SCflag;

//Buffer CAD + DMA
unsigned int DMAbuffer[64] __attribute__((space(dma)));


void Inic_CAD(void) //  
{
    //CONFIGURAMOS EL ADC1 PARA MUESTREAR AN2,AN3,AN4,AN5, por canal CH0 
    AD1CON1 = 0; // 0x00E0 
    //_ADON= 0;       // CAD OFF 
    //_ADSIDL = 0;      //1=Se para en modo Idle / 0=Continua en modo Idle 
    
    
    _ADDMABM = 0;    //1=bufer DMA en orden de conversión 
    // 0=bufer DMA en modo Scatter/Gather 
    
    
    //_AD12B = 0;       //1=Modo operacion de 12 bits / 0=Modo operacion de 10 bits  
    //_FORM = 0;       //Formato datossalida: 00=Integer (0000 00dd dddd dddd) 
    // 01=Signed integer (ssss sssd dddd dddd) 
    _SSRC = 0b111; //Termina muestreo y comienza conversion del CAD 
    //111=Auto?convert / 110, 101, 100, 011 RESERVADOS 
    // 010=TMR3  ADC1 y TMR5 ADC2 
    // 001=INT0  
    // 000= hay que borrar SAMP 
    //_SIMSAM = 0;  //1=Muestrea simultaneamente 
    // 0=Muestrea canales individuales secuencialmente 
    //_ASAM = 1;  //1=Muestreo comienza seguido de la conversion 
    // 0=comienza cuando SAMP=1 
    //_SAMP  
    //_DONE 
    AD1CON2 = 0; //  
    //_VCFG = 0;   //Vref+? 000= AVdd y AVss  
    // 001=Vref+ y AVss 
    // 010=AVdd y Vref?  
    // 011=Vref+ y Vref? 
    //_CSCNA = 0;  //1=Escaneo canal cero / 0= no escaneo 
    //_CHPS = 0;     //Canales utilizados:00=CH0 /01=CH0 y CH1 /1x= CH0,CH1,CH2 y CH3 
    //_BUFS  es solo de lectura: con BUFM=1?> 1=activa 2º mitad de buffer 
    // 0= activa 1ª mitad 
    //_SMPI = 0;   //Incrementos dir DMA +1 o 
    // sin DMA genera una interrupción cada cada SMPI+1
    //_BUFM = 0;   //1= 2 medios Buffers de 8 / 0= 1 buffer de 16 
    //_ALTS = 0;   //Modo de muestreo:1= Alternado / 0= Siempre canal A 
    //AD1CON3 
    _ADRC = 0; //1= reloj RC / 0= Reloj CPU 
    _SAMC = 31; //Tiempo auto muestreo0 nºde Tad (de 0 a 31Tad) 
    _ADCS = 3; //Tad= >75ns = Tcy(ADCS+1) ?> a 80MHz Tcy= 25ns  
    AD1CON4 = 0;
    //_DMABL = 3;  //Cada buffer contiene 8 palabras  
    AD1CHS123 = 0; // reg. seleccion de entrada canal 1,2 y 3  
    //_CH123NA= 0;    //Selección entrada negativa Canales 1, 2, y 3 pata Sample A  
    //0x= CH1=CH2=CH3?> Vref? / 10= CH1=AN6,CH2=AN7,CH3=AN8 
    // 11= CH1=AN9,CH2=AN10,CH3=AN11 
    //_CH123SA= 0;     //Selección entrada positiva Canales 1, 2, y 3 para Sample A 
    //1= AN3?AN4?AN5 / 0= AN0?AN1?AN2 
    //_CH123NB = 0; 
    //_CH123SB = 0; 
    AD1CHS0 = 0; // registro de seleccion de entrada canal 0 
    //_CH0NA = 0;   //Entrada negativa 1=  AN1 / 0 = Vref? 


    //----------------------------------------------
    _CH0SA = 5; //Entrada positiva = canal(CH0SA) 

    //------------------------------------------------
    //_CH0SB = 3; 
    //_CH0NB = 0;  
    AD1CSSH = 0x0000; //Selección entradas escaneo de la 16 a la 31
    AD1CSSL = 0x0000; //Selección entradas escaneo de 0 a 15.  
    // 1 =selecciona para escaneo / no escanea 
    AD1PCFGH = 0xFFFF; //Configura entradas: 1= digital / 0= Analog 
    AD1PCFGL = 0xFFFF;
    //_PCFG0=     // AN0 
    _PCFG4 = 0; // AN4 ?> RB4 ?> Sensor Temperatura 
    _PCFG5 = 0; // AN5 ?> RB5 ?> Potenciómetro 
    //IFS0 
    _AD1IF = 0; //Flag a cero del A/D interrrupt. 
    //IEC0 

    //------------------------------------
    _AD1IE = 0; // Habilita la interrupcion A/D.
    //------------------------------------


    //AD1CON1bits 
    _ADON = 1; //Comienzo de conversion 
    definirCanales();
    _SAMP= 1;
} // Fin Inic_CAD 

// ???????????????????????????????????????????????????????????????????????????????? 
// Bucle por encuesta 
//while (1) // Bucle infinito 
//{
//    AD1CON1bits.SAMP = 1; // Comienza el muestreo y al de 31Tad empieza la conversión 
//    while (!AD1CON1bits.DONE); // ¿Se ha realizado la conversión? 
//    ADCValue = ADC1BUF0; //  Si ? deja el valor en ADCValue y repetimos 
//}
// ????????????????????????????????????????????????????????????????????????????????

void definirCanales(void){
    CADFlag = 0;//flag para seleccionar
    
    //definicion de los canales
    potenciometro = 5;
    sensorTemperatura = 4;
}

void seleccionarCanal(void){
    switch (CADFlag) {
        case 0:
            _CH0SA = sensorTemperatura;
            break;
        case 1:
            _CH0SA = potenciometro;
            break;
    }
}

void _ISR _ADC1Interrupt(void) // para interrumpir cada conversión 
{
    seleccionarCanal();
    ADCValue = ADC1BUF0 >> 2;
    hex2ASCII_8bit(ADCValue, valorPotenciometroASCII);
    cambiarVentana();
    _AD1IF = 0; //Flag a cero del A/D interrrupt. 
    _SAMP = 1;
} // FIN ADC1Interrupt


void hex2ASCII_8bit(unsigned int valorHex, unsigned char ascii[]) {
    //Rutina para pasar de Hex a INT y de INT a char
    int millares, centenas, decenas, unidades;
    millares = valorHex / 1000;
    centenas = (valorHex - (millares * 1000)) / 100;
    decenas = (valorHex - (millares * 1000 + centenas * 100)) / 10;
    unidades = valorHex - (millares * 1000 + centenas * 100 + decenas * 10);

    ascii[0] = centenas + '0';
    ascii[1] = decenas + '0';
    ascii[2] = unidades + '0';
}

void cambiarVentana(void){
//    switch (CADFlag) {
//        case 0:
//            Ventana_Datos[1][8] = valorPotenciometroASCII[0]; //mostramos en pantalla lo calculado en iteración anterior
//            Ventana_Datos[1][9] = valorPotenciometroASCII[1];
//            Ventana_Datos[1][10] = valorPotenciometroASCII[2];
//            CADFlag = 1;
//            break;
//        case 1:
//            Ventana_Datos[1][13] = valorPotenciometroASCII[0];
//            Ventana_Datos[1][14] = valorPotenciometroASCII[1];
//            Ventana_Datos[1][15] = valorPotenciometroASCII[2];
//            CADFlag = 0;
//            break;
//    }
    unsigned char tmpVal[3];
    hex2ASCII_8bit(acum[0],tmpVal);
    Ventana_Datos[2][13] = tmpVal[0];
    Ventana_Datos[2][14] = tmpVal[1];
    Ventana_Datos[2][15] = tmpVal[2];
    
    hex2ASCII_8bit(acum[1],tmpVal);
    Ventana_Datos[3][13] = tmpVal[0];
    Ventana_Datos[3][14] = tmpVal[1];
    Ventana_Datos[3][15] = tmpVal[2];
    
    hex2ASCII_8bit(acum[2],tmpVal);
    Ventana_Datos[4][13] = tmpVal[0];
    Ventana_Datos[4][14] = tmpVal[1];
    Ventana_Datos[4][15] = tmpVal[2];
    
    hex2ASCII_8bit(acum[3],tmpVal);
    Ventana_Datos[5][13] = tmpVal[0];
    Ventana_Datos[5][14] = tmpVal[1];
    Ventana_Datos[5][15] = tmpVal[2];
}



// ------------------------------------------------------------- 

void Inic_CAD_DMA(void) // 
{
    //ORDER MODE 
    //************************** 
    //CONFIGURAMOS EL ADC1 PARA MUESTREAR AN4,AN5, por canal CH2 
    AD1CON1 = 0;
    // 0x00E0 
    //_ADON= 0;  
    // CAD OFF 
    //_ADSIDL = 0;   
    //1=Se para en modo Idle / 0=Continua en modo Idle 
    
    // DMA _ADDMABM = --; //1=bufer DMA en orden de conversión 
                          // 0=bufer DMA en modo Scatter/Gather 
    //_AD12B = 0;     
    //1=Modo operacion de 12 bits / 0=Modo operacion de 10 bits
    
    //_FORM = 0;      
    //Formato datos salida: 00=Integer (0000 00dd dddd dddd) 
    // 01=Signed integer (ssss sssd dddd dddd) 
    
    _SSRC = 0b111;
    //Termina muestreo y comienca conversion del CAD 
    //111=Auto-coonvert            
    //      010=TMR3            ADC1      y      TMR5      ADC2      
    // 001=INT0 / 000= hay que borrar SAMP 
    
    //_SIMSAM = 0;  
        //1=Muestrea simultaneamente 
        // 0=Muestrea canales individuales sequencialmente 
    
    _ASAM = 1;
    //1=Muestreo comienza seguido de la conversion 
    // 0=comienza cuando SAMP=1 
    
    //_SAMP    
    //_DONE 
    AD1CON2 = 0; // 0x00E0 
    //_VCFG = 0; //Vref+- 000=ACdd y AVss / 001=Vref+ y AVss 
                 // 010=AVdd y Vref- / 011=Vref+ y Vref- 
    
    _CSCNA = 1; //1=Escaneo canal cero / 0= no escaneo 
    
    //_CHPS = 0;  //Canales utilizados:00=CH0 /01=CH0 y CH1 /1x= CH0,CH1,CH2 y CH3
    
    //_BUFS   es solo de lectura: con BUFM=1-> 1=activa 2º mitad de buffer 
                // 0= activa 1ª mitad 
    
    //_SMPI = --; //Incrementos dir DMA +1 o 
    
    //_SMPI = 0;   
    //Incrementos dir DMA +1 o 
    // sin DMA genera una interrupción cada cada SMPI+1 
    //_BUFM = 0;     //1= 2 medios Buffers de 8 / 0= 1 buffer de 16 
    //_ALTS = 0;  
    //Modo de muestreo: 1= Alternado / 0= Siempre canal A 
    
    AD1CON3 = 0;
        _ADRC = 0;      //1= reloj RC / 0= Reloj CPU 
        _SAMC = 31;     //Tiempo auto muestreo0 nºde Tad (de 0 a 31Tad) 
        _ADCS = 3;      //Tad= >75ns = Tcy(ADCS+1) -> a 80MHz Tcy= 25ns 
                        // Tad = (3+1)Tcy 
        
    AD1CON4 = 0;
        //_DMABL = --;  //Cada buffer contiene 8 palabras 
    
    AD1CHS123 = 0;  // reg. seleccion de entrada canal 1,2 y 3
    
    //_CH123NA= 0;  //Selección entrada negativa Canales 1, 2, y 3 pata Sample A  
                    //0x=      CH1=CH2=CH3->      Vref-      
                    //10= CH1=AN6,CH2=AN7,CH3=AN8 
                    //11= CH1=AN9,CH2=AN10,CH3=AN11
    
    //_CH123SA= 0;  //Selección entrada positiva Canales 1, 2, y 3 para Sample A 
                    //1=      AN3-AN4-AN5      /      0=      AN0-AN1-AN2
    
    //_CH123NB = 0; 
    //_CH123SB = 0; 
    AD1CHS0 = 0; // Seleccion de entrada canal 0 
    //_CH0NA = 0;   //Entrada negativa 1=  AN1 / 0 = Vref- 
    //_CH0SA = 5;    //Entrada positiva = canal(CH0SA) 
    //_CH0SA = 4; 
    //_CH0SB = 3;
    //_CH0NB = 0;
    
    AD1CSSH = 0x0000; //Selección entradas escaneo de la 16 a la 31 
    AD1CSSL = 0x0330; //Selección entradas escaneo de 0 a 15. 
                      //binario 0 0 0 0 0 0 1 1 0 0 1 1 0 0 0 0 <- entradas 0 a 15 de derecha a izquierda
                      //        15...                ...3 2 1 0  

    AD1PCFGH = 0xFFFF; //Configura entradas: 1= digital 0= Analog 
    AD1PCFGL = 0xFFFF;
    //_PCFG0=       // AN0 
    _PCFG4 = 0;     // AN4 -> RB4 -> Sensor Temperatura 
    _PCFG5 = 0;     // AN5 -> RB5 -> Potenciómetro
    
    _PCFG8 = 0;     // AN8 -> RB8 -> Joystick_X 
    _PCFG9 = 0;     // AN9 -> RB9 -> Joystick_Y
    
    //IFS0 
        _AD1IF = 0; //Flag a cero del A/D interrrupt. 
    //IEC0 
        _AD1IE = 0; // Habilita la interrupcion A/D.
        
        
    //------------------------------------------------------------------ 
    DMA2CON = 0;
    // CHEN CANAL habilitado / deshabilitado 
    DMA2CONbits.SIZE = 0; // 1=Byte / 0=Word //Word por la cantidad de bits de CAD
    DMA2CONbits.DIR = 0;  // 1=lee de RAM y escribe a periférico 
                          // 0=lee de periférico y escribe en RAM
    
    // HALF interrupcion a  mitad / al final 
    // NULLW Escribe nulos / operacion normal 
    // DMA2CONbits.AMODE    //10: Direc.Indirect,Perif
                            // 01 Indir.Reg sin Post-incr 
                            // 00 Indir.Reg con Post-Incr
    
    //DMA2CONbits.MODE = -; // 11:One PP / 10:Continuo + PP 
                            // 01: One / 00: Continuo 
    //   DMA0REQ 
    DMA2REQbits.FORCE = 0; // manual / automático
    
    // IRQSEL 
        // 0x00 INT0 
        // 0x01 IC1 
        // 0x02 OC1 
        // 0x05 IC2 
        // 0x06 OC2 
        // 0x07 TMR2 
        // 0x08 TMR3 
        // 0x0A SPI1 
        // 0x0B UART1RX 
        // 0x0C UART1TX
        // 0x0D CAD
    DMA2REQbits.IRQSEL = 0x0D; // ADC1 //DMA_IRQ_ADC1 
    
        // 0x15 ADC2 
        // 0x1E UART2RX 
    // DMA2REQbits.IRQSEL = 0x1F; // UART2TX 
        // 0x21 SPI2 
        // 0x22 ECAN1-RX 
        // 0x37 ECAN2-RX 
        // 0x46 ECAN1-TX 
        // 0x47 ECAN2-TX
    
    DMA2STA = __builtin_dmaoffset(DMAbuffer); //0;      //Inicio dir. offset    //se usan ambas para ping pong
    //DMA2STB = __builtin_dmaoffset(CAD_BuffB); //0x0020; //Inicio dir. offset  //se usan ambas para ping pong
    //DMA2STA = __builtin_dmaoffset(Ventana_LCD); //0;          // Inicio dir. offset 
    //DMA2STB = __builtin_dmaoffset(Ventana_LCD); //0x0020;  // Inicio dir. offset
    
    DMA2PAD = (volatile unsigned int) &ADC1BUF0;
    
    AD1CON1bits.ADDMABM = 0;    //1=buffer DMA en orden de conversión -> AMODE = 0
                                //0=buffer DMA en modo Scatter/Gather -> AMODE = 2
    
    AD1CON2bits.SMPI = 3;   //Incrementos dir DMA + 1
    AD1CON4bits.DMABL = 0;  //Nº palabras del buffer por entrada analógica
                            //0->1, 1->2, 3->8, ..., 7->128
    
    DMA2CONbits.AMODE = 2;  //2 Direc. Indirect, Perif
                            //3 Reservado
                            //1 Indir. Reg sin Post-incr
                            //0 Indir. Reg con Post-incr
    
    DMA2CONbits.MODE = 0;   //3:Una + PP
                            //2:Continuo + PP
                            //1:Una
                            //0:Continuo
    
    //DMA2CNT = 4 - 1; // Nº de trasnsferencias -1
    DMA2CNT = 16-1;  //Scatter
    DMACS0 = 0; // Borra Colisiones 
    //IPC9bits.DMA2IP = 6;
    IFS1bits.DMA2IF = 0;
    IEC1bits.DMA2IE = 1;    //Habilita la interrupción
    
    DMA2CONbits.CHEN = 1; // Activa canal 2 DMA 
    //AD1CON1bits 
    _ADON = 1;
    //Habilita convertidor 
    _SAMP = 1;
    //Lanza conversion
    cnt = 0;
    acum[0] = 0;
    acum[1] = 0;
    acum[2] = 0;
    acum[3] = 0;
    SCflag=0;
} // Fin Inic_CAD DMA 
// ------------------------------------------------------------------- 
void Scatter_Continuo(void ){
    
    if(SCflag>0){
        SCflag=0;
        AD1CON1bits.ADDMABM = 0; //Modo Scatter
        DMA2CONbits.AMODE = 2;
        DMA2CNT = 4-1;
        Nop();
        Nop();
    }
    else{
        SCflag=1;
        AD1CON1bits.ADDMABM = 1; //Modo continuo
        DMA2CONbits.AMODE = 0;
        DMA2CNT = 4-1;
        Nop();
        Nop();
    }
    
    Nop();
    Nop();
    
}
void _ISR _DMA2Interrupt(void) {
    Nop();
    Nop();
    Nop();
    //cambiarVentana();
    recibirValores();
    IFS1bits.DMA2IF = 0;
}

void recibirValores(void){
    if(cnt < 8){
        if(SCflag==0){
            acum[0]+= DMAbuffer[4] >> 2;
            acum[1]+= DMAbuffer[5] >> 2;
            acum[2]+= DMAbuffer[8] >> 2;
            acum[3]+= DMAbuffer[9] >> 2;
        }
        else{
            acum[0]+= DMAbuffer[0] >> 2;
            acum[1]+= DMAbuffer[1] >> 2;
            acum[2]+= DMAbuffer[2] >> 2;
            acum[3]+= DMAbuffer[3] >> 2;
        }
        cnt++;
    }else{
        acum[0] /= 8;
        acum[1] /= 8;
        acum[2] /= 8;
        acum[3] /= 8;
        cambiarVentana();
        acum[0] = 0;
        acum[1] = 0;
        acum[2] = 0;
        acum[3] = 0;
        cnt = 0;
    }
    
}