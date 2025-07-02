/*
 * File:   stepper_controller.c
 * Author: amoralesma
 *
 * Created on June 28, 2025, 11:42 PM
 */


#include <xc.h>
#include <stdint.h>

#include "config_pic.h"
#include "stepper_controller.h"


int move_stepper(int8_t distance){
    int i; 
    for(i = 0; i < distance; i++){
        LATAbits.LATA1 = 1; 
        __delay_ms(5); 
        LATAbits.LATA1 = 0;
        __delay_ms(5);        
    }
    return !(i < distance); 
}
