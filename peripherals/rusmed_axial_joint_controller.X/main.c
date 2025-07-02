/*
 * File:   main.c
 * Author: Andrés Morales Martínez 
 *
 * Created on June 26, 2025, 4:57 PM
 */


#include <xc.h>
#include "config_pic.h"
#include "stepper_controller.h"
#include <stdint.h>
#include "i2c_init_config.h"

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF

#pragma CONFIG FOSC = INTOSC; // Por default 


#define DELAY_TIME 10 // ms 


void i2c_slave_init(void); 
void __interrupt() isr(void); 

// RB1: SDA1
// RB4: SCL1


int pulses = 0; 
const float angle_per_pulse = 0.174; 
float theta = 0.0; 

int buff_ctl = 0; 

unsigned char* buffPtr; // Pointer to send buff in I2C handshake  

unsigned char flag = 0; 




void main(void) {
    OSCCON = 0b01100000;
    i2c_slave_init();  // Activación de interrupciones y modo esclavo 
    
    // INT0 a
    INTCONbits.INTE = 1; // Habilitar interrupciones INT0 
    INTCONbits.INTF = 0; // Flag deshabilitada 
    
    // Configuration for as 
    TRISAbits.TRISA3 = 0; 
    TRISAbits.TRISA4 = 0; 
    
    // I2C Configuration 
    TRISAbits.TRISA1 = 0; // salida led 
    TRISBbits.TRISB3 = 0; // salida led 
    ANSELBbits.ANSB3 = 0;
    
    LATAbits.LATA1 = 1; 
    LATBbits.LATB3 = 1; 
    __delay_ms(1000); 
    LATAbits.LATA1 = 0; 
    LATBbits.LATB3 = 0; 
    
    
  
    while(1){
        theta = ( (float) pulses ) * angle_per_pulse;
        LATAbits.LATA3 = flag; 
        LATAbits.LATA4 = ~flag; 
       
        // PID Closed loop 
        
        
    }
    
   
    return;
}

/**
 * ISR para la lectura de I2C en Slave Mode 
 */
void __interrupt() isr(void) {
    if(INTCONbits.INTF){
        pulses++; 
        LATBbits.LATB3 ^= 1; 
        INTCONbits.INTF = 0; // Limpiar flag de interrupcion
        
        
    }
    
    
    
    
    if (SSP1IF && SSP1IE) {
        
        // Dirección recibida
        if (!SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
            buff_ctl = 0; 
            volatile uint8_t dummy = SSP1BUF;
        }
        // Dato recibido
        else if (SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
            int8_t data = (int8_t) SSP1BUF;
            if (data == 0x02){
                   flag = 0;
            }else if(data == 0x03){
                flag = 1;
            }
            
            
            SSP1CON1bits.CKP = 1;
        }
        // Lectura solicitada por el master (opcional aquí)
        else if (SSP1STATbits.R_nW) {
            volatile uint8_t dummy = SSP1BUF;
            
            unsigned char* path = (unsigned char*) &theta; 
            SSP1BUF = *(path + buff_ctl);  
            buff_ctl = (buff_ctl + 1) % 4; 
            
            SSP1CON1bits.CKP = 1;
            
        }

        SSP1IF = 0;
    }
}
