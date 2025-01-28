/* Project: Lab 2 2.1.1.
* Author: Neel Mulay
* Copyright (C) 2024 Neel Mulay - All Rights Reserved
* License:
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3.0 license.
*/

#include "MEAM_general.h"
#include "m_usb.h"
// char 10 and 13 are carriage return and line feed

#define ON 1
#define OFF 0

void checkPhotoTransistor222() { // print state of PB6 every time called
    int pinstate = bit_is_set(PINB, 6); // check if PB6 is high
    if (pinstate) {
        // The LED has to be turned off, 0V has to go out from Pin F5
        set(PORTF, 4);
    } else {
        // The LED has to be turned on, 5V has to go out from Pin F5
        clear(PORTF, 4);
    }
}

int main222() {
    m_usb_init();
    clear(DDRB, 6);
    clear(PORTB, 6); // turn off internal pull-up on PB6
    //Port B6 is an input Bit. The output from the Phototransistor is going to Pin B6
    
    set(DDRC, 6);  // Set the 6th Pin in Port C as output 
    //Port C6 is set as an output which powers the phototransistor circuit
    set(PORTC, 6); 

    //Port F4 is used as an output pin, to power the LED circuit
    set(DDRF,4);

    while (1) {
        checkPhotoTransistor222();
    }
}
