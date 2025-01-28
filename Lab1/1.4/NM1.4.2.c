/* Name: 1.4.2.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file
#define LIMIT 1023


// Custom delay function using a loop to simulate variable delay
void custom_delay2(double ms) {
    while(ms > 0) {
        _delay_ms(1);  // Delay 1 ms at a time
        ms -= 1;
    }
}

void pulse_LED2(double increasing_part, double decreasing_part, int steps, double duty_cycle) {
        
        //limit is the upper limit for the OCR3A register. It equals 1023 for a duty cycle of 100%.
        double limit = ((duty_cycle/100) * 1023);
        ////The per_step is the admissible change for the OCR3A bit per step of the part of the cycle.
        int per_step = (int) (limit/steps);
        
        //For the first part of the increase of intensity from 0 to I1 in t1 s
        for(int i=0;i<steps;i++)
        {
            //If the OCR3A register reaches its limit, then it means that the max intensity has been reached.
            //This line of code helps termination in case of the 50% intensity part.
            if(OCR3A>limit)
            break;
            //The intensity is controlled by the OCR3A register.
            OCR3A=OCR3A+per_step;
           //I used a custom delay function since it was not allowing me to use variables with _delay_ms
            custom_delay2((increasing_part/100)*1000);
        }
        //For the second part of the decrease of intensity from I1 to 0 in t2 s
        for(int j=(steps-1);j>=0;j--)
        {
            //The intensity is controlled by the OCR3A register.
            OCR3A=OCR3A-per_step;
            //I used a custom delay function since it was not allowing me to use variables with _delay_ms
            custom_delay2((decreasing_part/100)*1000);
        }  
    }


int main2(void)
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
    for(;;)
    {
        //Part 1 of the Cycle.
        pulse_LED2(0.1,0.4,100,100.0);
        //Part 2 of the Cycle.
        pulse_LED2(0.1,0.4,100,50.0);
        //Part 3 of the Cycle.
        _delay_ms(2000);
    }
    return 0; // Never reached
}
    