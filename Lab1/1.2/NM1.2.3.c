
/* Name: NM1.2.3.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file

int NM123(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz
    set_led(ON);			// turn on the on board LED
    _delay_ms(1000);		// wait 1000 ms when at 16 MHz

    /* insert your hardware initialization here */

    //The 6th Pin in Port C is set to an output pin
    set(DDRC,6);

    for(;;){
        /* insert your main loop code here */
        //First, increase the voltage to 5V for Port C pin 6
        set(PORTC,6);
        //Then we delay for 100 ms which means the LED is on
        _delay_ms(100);
        //To implement the blinking functionality, we set 0V to Port C Pin 6
        clear(PORTC,6);

        set_led(TOGGLE);	// switch the led state
        _delay_ms(1000);		// wait
    }
    return 0;   /* never reached */
}
