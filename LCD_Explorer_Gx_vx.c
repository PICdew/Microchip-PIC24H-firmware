/**********************************************************************
 * ? 2013 LG
 *
 * Proyecto: MISE Micros 16bits 
 * Fichero: LCD_Explorer16_24H_v2.C

    Grupo: G2    Autor: Michael Sandoval y Raúl Ruiz Bueno
    v0.0	Fecha: 2017-12-5
 * 
 * Descripción:
 * Fichero controlador del LCD de la placa de desarrollo Explorer16
 *
 **********************************************************************/

#include "p24HJ256GP610A.h"
#include "LCD_Explorer16_Gx_vx.h"
#include "Main_MISE_G0_v0.h"
//#include "Tiempos.h"

unsigned int milisegundos_LCD;
unsigned int flagLCDCompleto;

int c,f,scroll;

/* Patas utilizadas por el LCD en la Explorer 16
   RS -> R--
   E  -> R--
   RW -> R--
   DATA -> RE-- - RE--   
 */

/*****	LCD  *****/
//========= Envio de COMANDO =========
// Es bloqueante, espera a que termine el tiempor de TMR4 lo reprograma y
// al salir recupera la configuracion anterior de TMR4 y la interrupcion

void lcd_cmd(unsigned char cmd) // subrutina para enviar comandos 
{
    int x;
    //  TRISE &= 0xFF00;  // RE7-RE0 deben ser de salida 
    RW = 0; // Selecciona Escritura 
    RS = 0; // 0= Selecciona Registro (direccion) de comando 
    DATA &= 0xFF00;
    // Prepara los 8 bits del dato (RE7-RE0) 
    DATA |= cmd;
    // Copia comando en los 8 bits de menos peso de PORT E 
    // Entre el cambio en RS y/o RW y E= 1, hay que espera tAS > 60ns
    E = 1; // Inicio de operación, "E=1" debe permanecer un tiempo mímimo 
    Nop(); // Tiempo de espera para respetar el parámetro-> PWEH > 230ns
    for( x = 0; x < 6; x++){
        Nop();
    }
    // AÑADIR nº Nop en funcion de Fcy 
    E = 0; // Fin de operación 
    RW = 1; // Desactiva escritura por seguridad por si se producen glitches 
} // FIN lcd_cmd 


/*
 * Envío de datos al LCD
 */
void lcd_dato(unsigned char dato) // subrutina para enviar comandos 
{
    int x;
    //  TRISE &= 0xFF00;  // RE7-RE0 deben ser de salida 
    RW = 0; // Selecciona Escritura 
    RS = 1; // 0= Selecciona Registro (direccion) de dato 
    DATA &= 0xFF00;
    // Prepara los 8 bits del dato (RE7-RE0) 
    DATA |= dato;
    // Copia comando en los 8 bits de menos peso de PORT E 
    // Entre el cambio en RS y/o RW y E= 1, hay que espera tAS > 60ns
    E = 1; // Inicio de operación, "E=1" debe permanecer un tiempo mímimo 
    Nop(); // Tiempo de espera para respetar el parámetro-> PWEH > 230ns
    for(x = 0; x < 6; x++){ // AÑADIR nº Nop en funcion de Fcy
        Nop();
    } 
    E = 0; // Fin de operación 
    RW = 1; // Desactiva escritura por seguridad por si se producen glitches 
} // FIN lcd_cmd 

/*
 * Secuencia de comandos para inicializar el LCD de la placa.
 */
void Inic_LCD(void) {
    c = 0;
    f = 0;
    scroll=0;
    
    inic_timer_LCD(); //1º
    inic_pines_LCD(); //2º
    espera_LCD_ms(15); //3º
    function_set();     //4º
 //Nop(); Nop();  
    espera_LCD_ms(5);   //5º
    function_set();     //6º
  //Nop(); Nop();  // 
  espera_LCD_us(100); //7º
     // espera_LCD_ms(1);

    function_set();     //8º
   //Nop(); Nop();  // 
   espera_LCD_us(100);    //9º
      //espera_LCD_ms(1);
  function_set();       
   // espera_LCD_ms(1);
    //Nop(); Nop();  //  
   espera_LCD_us(100);
  
  display_on();         //10º
   espera_LCD_us(100);//11º
    //espera_LCD_ms(1);
   entry_mode();        //12º
   init_maquina_estados();
} //FIN Init_LCD

/*
 * Inicializador de la máquina de estados
 */
void init_maquina_estados(void){
    TMR_Timer_LCD = 0;
    PR_Timer_LCD = 2 * ( (Fmult *2000) - 1);
    TCON_Timer_LCD = 0x8000;
    _Timer_LCD_IF = 0;
    _Timer_LCD_IE = 1; // Habilita Interrupción del Timer 2
}

//================================================

/*
 * Función para inicializar el TIMER del LCD
 */

void inic_timer_LCD(void) {
    milisegundos_LCD = 0;

    TMR_Timer_LCD = 0; // Borra valor del timer
    PR_Timer_LCD = (Fmult *4000) - 1; // Define periodo
    TCON_Timer_LCD = 0x8000; // Habilitado, prescaler 1:1, CK interno
    //  TCON_Timer_LCD = 0b1000000000000000;	// Equivalente a la linea anterior

    // Inicializa interrupción Timer 2
    //  _Timer_LCD_IP = 4;  // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_LCD_IF = 0; // Borra flag de interrupción
    _Timer_LCD_IE = 0; // Habilita Interrupción del Timer 2
    Nop();
    Nop();
}

/*
 * Función de interrupción del Timer del LCD
 */
void _ISR_NO_PSV _Timer_LCD_Interrupt(void) {
    milisegundos_LCD++; // se incrementa cada milisegundo
    maquina_estados();
    
    _Timer_LCD_IF = 0;
} // Fin T1Interrupt


/*
 * Función de espera bloqueante en ms
 * Espera a que pasen n milisegundos
 */
void espera_LCD_ms(unsigned int n_miliseg) {
    TMR_Timer_LCD = 0;
    PR_Timer_LCD = n_miliseg * ((Fmult *4000) - 1);
    TCON_Timer_LCD = 0x8000;
    _Timer_LCD_IF = 0;
    while ( _Timer_LCD_IF == 0 );
    _Timer_LCD_IF = 0;
    Nop();
    Nop();
    Nop();
    Nop();
    //milisegundos_LCD = 0;
    Nop();
    Nop();

}

/*
 * Función de espera bloqueante en us
 * Espera a que pasen n microsegundos
 */
void espera_LCD_us(unsigned int n_microseg) {
    TMR_Timer_LCD = 0;
    PR_Timer_LCD = n_microseg * ((Fmult *400) - 1);
    TCON_Timer_LCD = 0x8000;
    _Timer_LCD_IF = 0;
    while ( _Timer_LCD_IF == 0 );
    _Timer_LCD_IF = 0;
    Nop();
    Nop();
}

/*
 * Función inicialización pines del LCD
 */
void inic_pines_LCD(void) {
    RW = 1;
    RS = 0;
    E = 0;
    RW_TRIS = 0;
    RS_TRIS = 0;
    E_TRIS = 0;

    TRISDATA = 0;
    DATA = 0;
}

/*
 * Máquina de estados para cambiar toda la ventana de carácteres del LCD
 */
void maquina_estados(void){
    Nop();
    Nop();
    if(c == 0 && f == 0){
        line_1();
        c++;
    }else if(c == 17 && f < 1){
        line_2();
        c = 1;
        f++;
    }else if(c == 17 && f == 1){
        f = 0;
        c = 0;
    }else{
        lcd_dato(Ventana_Datos[f+scroll][c-1]);
        c++;
    }
}