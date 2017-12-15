/*
Proyecto: MISE Micros 16bits 
Fichero: Tiempos.c

Grupo: G2    Autor: Michael Sandoval y Ra�l Ruiz Bueno
v0.0	Fecha: 2017-12-5
 * 
 * Descripci�n:
 * Fichero controlador de la UART
 */

#include "Uart.h"
#include "Main_MISE_G0_v0.h"
// ================ Inicializa la UART2 Serial Port ====================== 
int col;
int fil;
int br;

//unsigned int uart2RxBuffA[n_filas * n_columnas] __attribute__((space(dma))); 
//unsigned int uart2RxBuffB[n_filas * n_columnas] __attribute__((space(dma))); 
//unsigned char uart2TxBuffA[n_filas][n_columnas] __attribute__((space(dma))); 
//unsigned char uart2TxBuffB[n_filas][n_columnas] __attribute__((space(dma))); 
//extern unsigned char Ventana_Datos[n_filas][n_columnas] __attribute__((space(dma)));

/*
 * Inicializaci�n del DMA para el RS232
 */
void intRS232_DMA(void){
    initRS232_2_DMA();
    inic_RS232_2();
}

// ================ Inicializa la UART2 Serial Port ====================== 
/*
 * Inicializacion del RS232
 */
void initRS232_2_DMA(void) {
    // === Configuracion DMA asociado a UART 2 ==== 
    // Requiere desactivar interrupciones UART 2 
    DMA0CON = 0;
    // CHEN CANAL habilitado / deshabilitado 
    DMA0CONbits.SIZE = 1; // 1=Byte / 0=Word 
    DMA0CONbits.DIR = 1; // 1= lee de DPSRAM y escribe en perif�rico 
    // 0= lee de perif�rico y escribe en DPSRAM 
    // DMA0CONbits.HALF =?      // 1= interrupcion a  mitad / 0= al final 
    // DMA0CONbits.NULLW =?     // 1= Escribe nulos / 0= operacion normal 
    // DMA0CONbits.AMODE =?     // 10: Direc.Indirect,Perif 
    // 01 Indir.Reg sin Post-incr 
    // 00 Indir.Reg con Post-Incr 
    DMA0CONbits.MODE = 0; // 11: Un bloque + PingPong 
    // 10: Continuo + PingPong 
    // 01: Un bloque 
    // 00: Continuo 
    //   DMA0REQ Selecciona n� IRQ Periferico 
    DMA0REQbits.FORCE = 0; // 1= manual / 0= autom�tico 
    // IRQSEL Selecciona n� IRQ Periferico 
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
    // 0x0D ADC1 
    // 0x15 ADC2 
    // 0x1E UART2RX 
    DMA0REQbits.IRQSEL = 0x1F; // UART2TX 
    // 0x21 SPI2 
    // 0x22 ECAN1-RX 
    // 0x37 ECAN2-RX 
    // 0x46 ECAN1-TX 
    // 0x47 ECAN2-TX 
    // DMA0STA = __builtin_dmaoffset(uart2TxBuffA);  //0; // Inicio dir. offset 
    // DMA0STB = __builtin_dmaoffset(uart2TxBuffB);  //0x0020;// Inicio dir. offset 
    DMA0STA = __builtin_dmaoffset(Ventana_Datos); //0;    // Inicio dir. offset 
  //  DMA0STB = __builtin_dmaoffset(Ventana_Datos); //0x0020; // Inicio dir. offset 
    DMA0PAD = (volatile unsigned int) &U2TXREG; // Direcci�n del perif�rico 
    DMA0CNT = (n_filas * n_columnas) - 1; // N� de trasnsferencias -1 
    DMACS0 = 0; // Borra flags de Colisiones 
    //    INTCON1bits.DMACERR = 0;    // Borraa Flag de interrupci�n de colisiones 
    IFS0bits.DMA0IF = 0;
    IEC0bits.DMA0IE = 1; // Habilita interrupciones DMA 0 
    DMA0CONbits.CHEN = 1; // Enable canal 0 DMA 
} 

// === Atenci�n Interrupcione DMA =================== 
//void __attribute__((__interrupt__)) _DMA0Interrupt(void) 

/*
 * Rutina de la interrupci�n del DMA
 */
void _ISR _DMA0Interrupt(void) {
    Nop();
    Nop();
    Nop();
    IFS0bits.DMA0IF = 0;
}

/*
 * Inicializaci�n del RS232
 */
void inic_RS232_2(void) {
    // No requerido, lo hace el hardware 
    //  UART2_TX_TRIS = 0; 
    //  UART2_RX_TRIS = 1; 
    //  UART2_TX_LAT  = 1; 
    //  UART2_RX_LAT  = 1; 
    U2MODE = 0; // 8bits, sin paridad, 1 stop, Uart parada 
    _UARTEN_U2 = 1;
    // Habilita UART 
    //_USIDL_U2 =0;
    // 0-> continua en modo Idle 
    //_IREN_U2 =0;
    // Habilita IrDA 
    //_RTSMD_U2 =;
    // Modo pata U2RTS 
    //_UEN_U2 =0x00;
    // Habilita U2TX, U2RX, U2CTS y U2RTS 
    //_WAKE_U2 =0;
    // Habilita Wake-up al detectar Start en Sleep 
    //_LPBACK_U2 =0;
    // Habilita modo Loopback 
    //_ABAUD_U2 =0;
    // Habilita Auto-Baud 
    //_RXINV_U2 =0;
    // Invierte polaridad para recepci�n 
    _BRGH_U2 = 0; // BRGH 
    //_PDSEL_U2 =0x00;
    // Paridad: 00= 8 bits sin paridad 
    //_STSEL_U2 =0;
    // Duraci�n bit Stop 
    U2BRG = BAUD_RATEREG_2_BRGH0(Fcy,BAUDRATE2);
    //U2BRG = 8;
    // Aplicando la formula da 7 y no va bien 
    U2STA = 0;
    //_UTXISEL0_U2 =0x00; // Tipo Interrupci�n Transmisi�n 
    //_UTXINV_U2 =0; // Invierte polaridad pata transmisi�n 
    //_UTXBRK_U2 =0; // Bit Breal 

    //_UTXEN_U2 =1; // Habilita Transmisi�n 

    // _UTXBF_U2 =0; // Estado Buffer transmisi�n (solo lectura) 
    // _TRMT_U2 =; // Estado Reg.Desplazamiento (solo lectura) 
    //_URXISEL0_U2 =0; // Tipo interrupci�n en recepci�n 
    // 0x= interrupci�n con cada dato que llega 
    // 10= interrupci�n a 3/4 del buffer 
    // 11= interrupci�n cuando buffer lleno 
    // _ADDEN_U2 =0; // Detecci�n bit de direcci�n en 9 bits 
    // _RIDLE_U2 =; // Estado de la recepci�n (solo lectura) 
    //_PERR_U2 =; // Error de paridad /solo lectura) 
    //_FERR_U2 =; // Error de trama (solo lectura) 
    //_OERR_U2 =0; // Error ocerrun (borrable/ solo lectura) 
    //_URXDA_U2 =; // 0=buffer vac�o, 1= al menos un dato 
    _U2RXIF = 0; // Borra flag int. RX 
    _U2TXIF = 0; // Borra flag int. TX 
    _U2EIF = 0; // Borra flag de Error UART 
    _U2TXIE = 0; // Habilita int. de transmisi�n 
    _U2RXIE = 1; //Habilita int. recepci�n
    //  _U2EIE = 1;      // Habilita Int_ de Error UART 
    _OERR_U2 = 0; // Seg�n versi�n corrige bugg 
    _UARTEN_U2 = 1; // Habilita la UART_2 
    _UTXEN_U2 = 1; //Habilita transmisi�n,  
    
    //Debe activarse despu�s de habilitar UART 
    // RUTINA QUE ESPERA LA DURACION DE UN BIT ANTES DE EMPEZAR A TRANSMITIR 
    //  - - - 
    U2TXREG = 0;
    col = 0;
    fil = 0;
    br = 0;
} // Fin Inicializaci�n UART 2 

// ================ Servicio INTERRUPCION TRANSMISION RS232_2 ============== 
// Trasmite un dato, si hay, al final de transmisi�n del anterior 

/*
 * Rutina de interrupci�n de transmisi�n de la UART
 */
void _ISR_NO_PSV _U2TXInterrupt(void) {
    //U2TXREG = 'H'; // Env�a dato "H"
    /*if (col >= n_columnas && fil < n_filas){
        fil++;
        col = 0;
    }else if(col >= n_columnas && fil >= n_filas-1){
        col = 0;
        fil = 0;
    }
    U2TXREG = Ventana_Datos[fil][col];
    col++;*/
    Nop();
    Nop();
    _U2RXIF = 0;
    _U2TXIF = 0;

} // FIN _U2RXInterruptt 

/*
 * Rutina de interrupci�n de recepci�n de la UART
 */
void _ISR_NO_PSV _U2RXInterrupt(void) {
    //U2TXREG = 'H'; // Env�a dato "H"
    /*if (col >= n_columnas && fil < n_filas){
        fil++;
        col = 0;
    }else if(col >= n_columnas && fil >= n_filas-1){
        col = 0;
        fil = 0;
    }
    U2TXREG = Ventana_Datos[fil][col];
    col++;*/
    Ventana_Datos[1][14]=U2RXREG;
    Nop();
    Nop();
    
    _U2RXIF = 0;
    //_U2TXIF = 0;

} // FIN _U2RXInterruptt 


void _ISR_NO_PSV _U2ErrInterrupt(void) {
    Nop();
    Nop();
    _U2EIF = 0;
}
// ================ TRANSMISION RS232_2 ================== 
// Env�a un caracter por la UART2  

/*
 * Rutina que pone un dato en el RS232
 */
void putRS232_2(unsigned char c) {
    while (_UTXBF_U2 == 1); // Espera mientras buffer Tx est� lleno 
    U2TXREG = c; // Carga dato a transmitir 
} // FIN putRS232_2 
// ================ RECEPCION RS232_2 ================== 
// Espera a recibir un caracter por la UART2 

/*
 * Rutina que recibe un dato del RS232
 */
char getRS232_2(void) {
    while (!_URXDA_U2); // Espera a que llegue un dato 
    return U2RXREG; // recoge el dato 
}// FIN getRS232_2
