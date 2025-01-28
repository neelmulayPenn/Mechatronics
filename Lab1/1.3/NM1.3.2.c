/* Name: NM1.3.2.c
 * Author: Neel Mulay
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file
#define COMPAREVALUE 6250 //The number we got through calculation

int NM132(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz
    set_led(ON);			// turn on the on board LED
    //_delay_ms(1000);		// wait 1000 ms when at 16 MHz

    /* insert your hardware initialization here */

    //The 5th Pin in Port B is set to an output pin
    set(DDRC,6);
    set(TCCR3B,1);
    //Using a prescaler of /64 so we turn on the specific bits which correspond to this
    clear(TCCR3B,CS32); //Changing to a prescaler of /16
    clear(TCCR3B,CS30);
    set(TCCR3B,CS31);

    for(;;){
         /* insert your main loop code here */
        if (TCNT3 > COMPAREVALUE) 
        {
            toggle(PORTC,6);
            TCNT3 = 0;
            //I see a different value from 1.3.1 when I use a pre-scaler of /16. The frequency changes to 160 Hz.
            //The frequency in 1.3.1 was 20 Hz while using a pre-scaler of /64.
        }
    }
    return 0;   /* never reached */
}
