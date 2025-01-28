/* Name: 1.4.1.1.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file
#define LIMIT 1023

int main11(void)
{
    _clockdivide(0); // Set the clock speed to 16 MHz
    set_led(ON);     // Turn on the onboard LED

    // Set the 6th Pin in Port C to output (OC3A or OC3B)
    set(DDRC, 6); // OC3A on PC6

    // Set Timer 3 to Mode 7 (Fast PWM, 10-bit)
    set(TCCR3A, WGM30); // WGM3[3:0] = 0111 for Mode 7
    set(TCCR3A, WGM31);
    clear(TCCR3B, WGM33);
    set(TCCR3B, WGM32);

    // Set non-inverting mode on OC3A (Pin 6 on Port C)
    set(TCCR3A, COM3A1); // Non-inverting PWM on OC3A
    clear(TCCR3A, COM3A0);

    // Set the prescaler to /64 (gives slower PWM frequency, easier to read on the scope)
    clear(TCCR3B, CS32); // CS32:0 = 011 for prescaler /64
    set(TCCR3B, CS31);
    set(TCCR3B, CS30);

    // Set OCR3A to control the duty cycle (value between 0 to 1023)
    
    OCR3A = 0; // Set for 0 and it goes up
    // For a 50% duty cycle (10-bit PWM, so 512/1023 = ~50%)
    int i=0;
    for(;;)
    {
        for(i=0;i<100;i++)
        {
            //In the first half of the cycle, we increase the duty cycle to 100%
            
            OCR3A=OCR3A+10;
            
            _delay_ms(5);
        }

        for(i=99;i>=0;i--)
        {
            OCR3A=OCR3A-10;
            _delay_ms(5);
        }
    }

    return 0; // Never reached
}
