/* Project: Lab 2 2.1.3.
* Author: Neel Mulay
* Copyright (C) 2024 Neel Mulay - All Rights Reserved
* License:
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3.0 license.
* screen /dev/tty.usbmodem4101
*/

#include "MEAM_general.h"
#include "m_usb.h"

#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)

static int oldtime213 = 0;
static int presstime213 =0;

void init_timer3213() {
    // Set Timer 3 for input capture on rising edge (ICES3 = 1)
    set(TCCR3B, ICES3);

    // Set prescaler to 1024
    set(TCCR3B, CS30); // Clear CS30 bit
    clear(TCCR3B, CS31);   // Set CS31 bit
    set(TCCR3B, CS32);   // Set CS32 bit
}

void waitforpress213() {
    //The system waits for the button to be pressed
    while(!bit_is_set(TIFR3,ICF3));
    set (TIFR3, ICF3); // clear flag by writing 1 to flag (!)
    //The difference in time is measured
    presstime213 = ICR3-oldtime213;
    //The start time is updated 
    oldtime213 = ICR3;
    PRINTNUM(presstime213);
}

int main213(void) {
    m_usb_init();               // Initialize USB communication
    clear(PORTC, 7);           // Pin C7 is configured as an input pin
    init_timer3213();          // Initialize Timer 3 for input capture

    while (1) {
        // Measure time between two button presses
        waitforpress213();
    }
}
