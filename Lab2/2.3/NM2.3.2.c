/* Project: Lab 2 2.3.2
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

// Pin definitions
#define RED_LED_PIN 10   // Pin for Red LED (25Hz detection)
#define GREEN_LED_PIN 9  // Pin for Green LED (662Hz detection)

//static variables defined which make them global
static int oldtime232 = 0;
static int period232 = 0;
static float frequency232 = 0;

void init_timer3232() {
    // Set Timer 3 for input capture on rising edge (ICES3 = 1)
    set(TCCR3B, ICES3);

    // Set prescaler to 1024
    set(TCCR3B, CS30); // Clear CS30 bit
    clear(TCCR3B, CS31);   // Set CS31 bit
    set(TCCR3B, CS32);   // Set CS32 bit
}

void frequencyDetect232() {
    // Wait until input capture flag is set
    while (!bit_is_set(TIFR3, ICF3));

    // Clear the input capture flag
    set(TIFR3, ICF3);

    // Measure time between edges (in clock ticks)
    period232 = ICR3 - oldtime232;
    oldtime232 = ICR3;

    // Calculate frequency in Hz (with prescaler 1024, clock frequency 16MHz)
    if (period232 > 0) {
        //Clock frequency = 16 MHz
        //Ticks per second using a 1024 prescaler = 16 MHz / 1024 = 15625 ticks per second
        //Time period = No. of clock ticks / 15625
        //Frequency of the wave = 15626 / No. of clock ticks
        frequency232 = (float)15625.0 / (period232);
        //Printing the frequency of the wave to understand the limits 
        PRINTNUM((int)frequency232);
    }
    // Turn on GREEN LED if frequency is 662Hz
    // For a 25 Hz wave, the limits seen were this
    if (frequency232 > 24 && frequency232 < 26) {
        set(PORTF, 6);   // Turn on GREEN LED
        clear(PORTF, 1);  // Turn off RED LED
    }
    // Turn on RED LED if frequency is 662Hz
    //Limits were 651 and 679 for a 662 Hz wave
    else if (frequency232 > 650 && frequency232 < 680) {
        set(PORTF, 1);   // Turn on RED LED
        clear(PORTF, 6);   // Turn off GREEN LED
    } 
    // Turn off both LEDs if neither frequency is detected
    else {
        clear(PORTF, 6);   // Turn off RED LED
        clear(PORTF, 1);  // Turn off GREEN LED
    }
}

void setupLEDs232() {
    // Set up LEDs as output
    set(DDRF, 6);   // Set RED LED pin as output
    set(DDRF, 1); // Set GREEN LED pin as output
    // Set up C7 as an input
    clear(DDRC, 7); // Set pin C7 as input (amplifier input)
}

int main232(void) {
    m_usb_init();               // Initialize USB communication
    clear(PORTC, 7);            // Pin C7 is configured as an input pin
    init_timer3232();              // Initialize Timer 3 for input capture
    setupLEDs232();                // Set up LED pins as outputs

    while (1) {
        // Measure time between two input signals
        frequencyDetect232();

        // Control the LEDs based on the detected frequency
        //controlLEDs();
    }
}
