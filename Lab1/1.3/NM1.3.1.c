/* Name: NM1.3.1.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file
#define COMPAREVALUE 6250 //The number we got through calculation

int NM131(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz
    set_led(ON);			// turn on the on board LED
    //_delay_ms(1000);		// wait 1000 ms when at 16 MHz

    /* insert your hardware initialization here */

    //The 5th Pin in Port B is set to an output pin
    set(DDRC,6);
    set(TCCR3B,1);
    //Using a prescaler of /64 so we turn on the specific bits which correspond to this
    set(TCCR3B,CS31); 
    set(TCCR3B,CS30);

    for(;;){
         /* insert your main loop code here */
        if (TCNT3 > COMPAREVALUE) 
        {
            toggle(PORTC,6);
            TCNT3 = 0;
        }
    }
    return 0;   /* never reached */
}
