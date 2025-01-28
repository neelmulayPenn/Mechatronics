/* Project: Lab 2 2.1.1.
* Author: Neel Mulay
* Copyright (C) 2024 Neel Mulay - All Rights Reserved
* License:
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3.0 license.
*/

#include "MEAM_general.h"
#include "m_usb.h"
#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)
// char 10 and 13 are carriage return and line feed

#define ON 1
#define OFF 0


int main221() {
    m_usb_init();
    set(PORTC, 7); // turn on internal pull-up on PC7
    set(DDRC, 6);  // Set the 6th Pin in Port C as output for the LED
    set(PORTC, 6); 
    while (1) {
        
       
    }
}
