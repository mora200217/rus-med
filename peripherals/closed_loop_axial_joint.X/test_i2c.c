/*
 * File:   main.c
 * Author: Andrés Morales Martínez 
 *
 * Created on June 26, 2025, 4:57 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "i2c_init_config.h"

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF

#pragma CONFIG FOSC = INTOSC; // Por default 

#define _XTAL_FREQ 4000000



void setup_routine(void); 

void i2c_slave_init(void); 
void __interrupt() isr(void); 

// RB1: SDA1
// RB4: SCL1

char dist_to_move; 
int ctl; 

bool has2send; 

void main(void) {
    setup_routine(); 
    
    while(1){
        // TODO: Lectura de encoder 
        // TODO: Lazo de control 
        
        if(count > 0 && count < N){
            __delay_ms(DELAY_TIME); 
            LATAbits.LATA ^= 1; 
            count--; 
        }
        
    }
   
    return;
}

/**
 * ISR para la lectura de I2C en Slave Mode 
 */
void __interrupt() isr(void) {
    if (SSP1IF && SSP1IE) {
        
        // Dirección recibida
        if (!SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
            
            volatile uint8_t dummy = SSP1BUF;
        }
        // Dato recibido
        else if (SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
            uint8_t data = SSP1BUF;
            LATAbits.LATA1 = 1; // LED ON
            if (data == 0x02) {
                LATAbits.LATA1 = 1; // LED ON
            } else if (data == 0x01) {
                LATAbits.LATA1 = 0; // LED OFF
            }
            SSP1CON1bits.CKP = 1;
        }
        // Lectura solicitada por el master (opcional aquí)
        else if (SSP1STATbits.R_nW) {
            volatile uint8_t dummy = SSP1BUF;
            SSP1BUF = 0x42;
            SSP1CON1bits.CKP = 1;
        }

        SSP1IF = 0;
    }
}

/*
 *  setup_routine (routine) 
 *  - 
 *  Initial configuration. Here is to put all 
 *  the config register, initial values and 
 *  comms 
 * 
 *  
 */
void setup_routine(void){
     OSCCON = 0b01100000;
    i2c_slave_init();  // Activación de interrupciones y modo esclavo 
    
    // I2C Configuration 
    TRISAbits.TRISA1 = 0; // salida led 
    LATAbits.LATA1 = 1; 
    __delay_ms(1000); 
    LATAbits.LATA1 = 0; 
    __delay_ms(1000); 
    
    float set_angle = 0; // Set point 
    
}