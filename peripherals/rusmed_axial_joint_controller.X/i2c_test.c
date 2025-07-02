/*
 * File:   i2c_test.c
 * Author: amoralesma
 *
 * Created on June 26, 2025, 4:57 PM
 */


#include <xc.h>
#include <stdint.h>

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF

#pragma CONFIG FOSC = INTOSC; // Por default 

#define _XTAL_FREQ 4000000


void i2c_slave_init(void); 
void __interrupt() isr(void); 

// RB1: SDA1
// RB4: SCL1



void main(void) {
    OSCCON = 0b01100000;
    i2c_slave_init(); 
    
    
    // I2C Configuration 
    TRISAbits.TRISA1 = 0; // salida led 
    LATAbits.LATA1 = 1; 
    __delay_ms(1000); 
    LATAbits.LATA1 = 0; 
    __delay_ms(1000); 
    
    
    
    while(1); 
    
    
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
void i2c_slave_init(void) {
    // Enable peripheral interrupts
    PIE1bits.SSP1IE = 1;    // Enable SSP1 (I2C) interrupt
    PIR1bits.SSP1IF = 0;    // Clear the SSP1 interrupt flag

    // Enable global and peripheral interrupts
    INTCONbits.PEIE = 1;    // Peripheral Interrupt Enable
    INTCONbits.GIE  = 1;   


    TRISBbits.TRISB1 = 1; // SCL as input
    TRISBbits.TRISB4 = 1; // SDA as input

    SSP1CON1 = 0b00110110; // I2C Slave mode, 7-bit address
    SSP1CON2 = 0;          // Default settings
    SSP1STAT = 0;          // Clear status

    SSP1ADD = (0x69 << 1); // Load slave address into SSP1ADD (shifted)

    SSP1CON1bits.SSPEN = 1; // Enable MSSP
}