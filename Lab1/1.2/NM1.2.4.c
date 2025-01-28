/* Name: NM1.2.4.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file

int NM124(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz
    set_led(ON);			// turn on the on board LED
    //_delay_ms(1000);		// wait 1000 ms when at 16 MHz

    /* insert your hardware initialization here */

    //The 6th Pin in Port C is set to an output pin
    //The duty cycle can be changed and it can be verified
    double duty_cycle = 80.0;
    double period = 100.0;
    set(DDRC,6);

    for(;;){
        /* insert your main loop code here */
        if(duty_cycle>=100.0)
        {
            //The LED will remain on for the whole cycle
            set(PORTC,6);
        }
        else if(duty_cycle==0.0)
        {
            //The LED will remain off
            clear(PORTC,6);
        }
        else
        {
            //The LED is ON for a percentage of duty cycle
            set(PORTC,6);
            _delay_ms((duty_cycle/100.0)*period);
            
            //The LED is OFF for the remainder of the duty cycle
            clear(PORTC,6);
            _delay_ms(((100.0-duty_cycle)/100.0) * period);
        }
    

        //set_led(TOGGLE);	// switch the led state
        //_delay_ms(1000);		// wait
    }
    return 0;   /* never reached */
}
