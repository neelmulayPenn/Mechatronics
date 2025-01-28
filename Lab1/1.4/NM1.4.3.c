/* Name: 1.4.3.c
 * Author: Neel Mulay
 */
#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file

// Custom delay function using a loop to simulate variable delay
void custom_delay3(double ms) {
    while(ms > 0) {
        _delay_ms(1);  // Delay 1 ms at a time
        ms -= 1;
    }
}

void pulse_LED3(double increasing_part, double decreasing_part, int steps, double duty_cycle) {
        
        //Increasing part is 0 to 0.1 s in the example, where the brightness goes to 100%
        //Decreasing part is 0.1 to 0.5 s in the example, where the brightness goes to 0 %
        
         //OCR3AMax is the upper limit for the OCR3A register. It equals 1023 for a duty cycle of 100%.
        double OCR3AMax = ((duty_cycle/100.0) * 1023.0);
        //The per_step is the admissible change for the OCR3A bit per step.
        int per_step = (int) (OCR3AMax/steps);
        
        //For the first part of the increase from 0 to I1 in 0.1s
        for(int i=0;i<steps;i++)
        {
            //The intensity is controlled by the OCR3A register.
            OCR3A=OCR3A+per_step;

            //I used a custom delay function since it was not allowing me to use variables with _delay_ms
            //Delay per step is 0.1/100 s = 1 ms
            custom_delay3((increasing_part/100)*1000);
        }
    
        //In a similar fashion, we go down from I1 to 0 in 0.4 s
        for(int j=(steps-1);j>=0;j--)
        {
            OCR3A=OCR3A-per_step;
            //Delay per step is 0.4/100 s = 4 ms
            custom_delay3((decreasing_part/100)*1000);
        }
        
        
    }

int main3(void)
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
    //Initial max intensity I1
    double d1=100.0;
    //Initial max intensity I2
    double d2=50.0;

    for(;;)
    {
        //We have two waveform with I1 and I2
        // Part 1 of cycle
        pulse_LED3(0.1,0.4,100,(d1)); 
        // Part 2 of cycle
        pulse_LED3(0.1,0.4,100,(d2)); 
        //I1 and I2 are followed by a 2 s gap
        // Part 3 of cycle
        _delay_ms(2000); 
        //Now, to get reducing brightness, we reduce d1 and d2 such that they go to 0 in 20 cycles
        d1=d1-5;
        d2=d2-2.5;
        //We terminate the code at the end of 20 cycles if either of the pulses are 0 intensity.
        if(d1==0 || d2==0)
        break;
        
    }
    return 0; // Never reached
}
    