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

void checkPC7() { // print state of PC7 every time called
    int pinstate = bit_is_set(PINC, 7); // check if PC7 is high
    if (pinstate) {
        // The LED has to be turned on, 5V has to go out from Pin C5
        clear(PORTC, 5);
    } else {
        // The LED has to be turned off, 0V has to go out from Pin C5
        set(PORTC, 5);
    }
}

int main1() {
    m_usb_init();
    set(PORTC, 7); // turn on internal pull-up on PC7
    set(DDRC, 5);  // Set the 5th Pin in Port C as output for the LED

    while (1) {
        checkPC7();
    }
}
