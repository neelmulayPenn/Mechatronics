/* Name: 1.4.1.2.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file
#define LIMIT 1023

int main1(void)
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

    //The increasing_part refers to the first part of the cycle, which is turning the LED on
    double increasing_part = 0.3;//in seconds <1s
    //The decreasing_part refers to the second part of the cycle, which is dimming the LED 
    double decreasing_part = 0.6;//in seconds <1s
    //No. of steps in the both parts of the cycle
    int steps = 100;
    //Loop counter to count the numberof steps
    int i=0;
    for(;;)
    {
        for(i=0;i<steps;i++)
        {
            //In the first part of the cycle, we increase the duty cycle to 100%
            //Since there are 100 steps, we need to increase OCR3A from 0 to 1023, which is 1023/100 which is approximately an increase of 10 per step
            OCR3A=OCR3A+10;
            //The first part of the cycle is the increasing part. Hence the time for each step is given by the formula, and it is in ms
            _delay_ms((increasing_part/100)*1000);
        }

        for(i=(steps-1);i>=0;i--)
        {
            //In the second part of the cycle, we decrease the duty cycle to 0%
            //Since there are 100 steps, we need to decrease OCR3A from 1023 to 0, which is 1023/100 which is approximately a decrease of 10 per step
            OCR3A=OCR3A-10;
            //The second part of the cycle is the decreasing part. Hence the time for each step is given by the formula, , and it is in ms
            _delay_ms((decreasing_part/100)*1000);
        }
    }

    return 0; // Never reached
}
