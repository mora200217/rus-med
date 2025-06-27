/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#define I2C_ADDRESS 0x69 
#include <xc.h> // include processor files - each processor file is guarded.  



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

    SSP1ADD = (I2C_ADDRESS << 1); // Load slave address into SSP1ADD (shifted)

    SSP1CON1bits.SSPEN = 1; // Enable MSSP
}

#endif	/* XC_HEADER_TEMPLATE_H */

