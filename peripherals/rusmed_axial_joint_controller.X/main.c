/*
 * File:   main.c
 * Author: Andrés Morales Martínez 
 *
 * Created on June 26, 2025, 4:57 PM
 */

#include <xc.h>
#include <stdint.h>

#include "config_pic.h"
#include "stepper_controller.h"
#include "i2c_init_config.h"

// ==========================
// CONFIG BITS
// ==========================
#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF

// ==========================
// DEFINES Y VARIABLES
// ==========================
#define DELAY_TIME 10 // ms

// Prototipos
void i2c_slave_init(void); 
void __interrupt() isr(void); 

// Pines:
// RB1: SDA1
// RB4: SCL1

volatile int pulses = 0; 
const float angle_per_pulse = 0.174f; 
volatile float theta = 0.0f; 

volatile int buff_ctl = 0; 
unsigned char* buffPtr; // Puntero para enviar buffer en I2C

volatile unsigned char flag = 0; 

// ==========================
// MAIN
// ==========================
void main(void) {
    // Configurar oscilador interno a 4 MHz
    OSCCON = 0b01100000;

    i2c_slave_init();  // Inicializar I2C en modo esclavo e interrupciones

    // Configurar interrupción externa INT0
    INTCONbits.INTE = 1; 
    INTCONbits.INTF = 0; 

    // Configurar pines como salida
    TRISAbits.TRISA3 = 0; 
    TRISAbits.TRISA4 = 0; 
    TRISAbits.TRISA1 = 0; 
    TRISBbits.TRISB3 = 0; 
    ANSELBbits.ANSB3 = 0; // Desactivar analógico en RB3

    // Señal de inicio: encender LEDs 1 s
    LATAbits.LATA1 = 1; 
    LATBbits.LATB3 = 1; 
    __delay_ms(1000); 
    LATAbits.LATA1 = 0; 
    LATBbits.LATB3 = 0; 

    while(1) {
        theta = ((float)pulses) * angle_per_pulse;

        LATAbits.LATA3 = flag; 
        LATAbits.LATA4 = ~flag; 

        // Espacio para PID en lazo cerrado
    }
}

// ==========================
// INTERRUPT SERVICE ROUTINE
// ==========================
void __interrupt() isr(void) {
    // INT0: Contador de pulsos
    if (INTCONbits.INTF) {
        pulses++; 
        LATBbits.LATB3 ^= 1; // Indicador de pulso
        INTCONbits.INTF = 0; // Limpiar flag
    }

    // I2C Slave
    if (SSP1IF && SSP1IE) {
        if (!SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
            // Dirección recibida
            volatile uint8_t dummy = SSP1BUF;
            buff_ctl = 0; 
        }
        else if (SSP1STATbits.D_nA && !SSP1STATbits.R_nW) {
            // Dato recibido
            int8_t data = (int8_t) SSP1BUF;
            if (data == 0x02) {
                flag = 0;
            } else if (data == 0x03) {
                flag = 1;
            }
            SSP1CON1bits.CKP = 1; // Reanudar reloj
        }
        else if (SSP1STATbits.R_nW) {
            // Master solicita lectura
            volatile uint8_t dummy = SSP1BUF;

            unsigned char* path = (unsigned char*)&theta; 
            SSP1BUF = *(path + buff_ctl);  
            buff_ctl = (buff_ctl + 1) % 4; 

            SSP1CON1bits.CKP = 1; // Reanudar reloj
        }

        SSP1IF = 0; // Limpiar flag
    }
}
