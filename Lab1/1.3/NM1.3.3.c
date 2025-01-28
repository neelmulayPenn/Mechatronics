/* Name: 1.3.3.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file

int NM133(void)
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

    // Set the prescaler to 1024 (gives slower PWM frequency, easier to read on the scope)
    set(TCCR3B, CS32); // CS32:0 = 101 for prescaler /1024
    clear(TCCR3B, CS31);
    set(TCCR3B, CS30);

    // Set OCR3A to control the duty cycle (value between 0 to 1023)
    double duty_cycle = 60; //In Percentage
    //The duty cycle can be changed here as asked in the lab

    OCR3A = (duty_cycle*1023)/100; // Set for 60% duty cycle
    // For a 50% duty cycle (10-bit PWM, so 512/1023 = ~50%)

    for(;;)
    {
        // Main loop can be left empty as PWM is hardware-controlled
    }

    return 0; // Never reached
}
