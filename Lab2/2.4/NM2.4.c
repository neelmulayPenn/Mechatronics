/* Project: Lab 2 2.4.1, 2.4.2 and 2.4.2 EC Same code for all 3 questions
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

// Static variables
static int oldtime = 0;
static int period = 0;
static float frequency = 0;
static volatile uint16_t overflow_count = 0;

// Initialize Timer 3 with input capture and overflow interrupt
void init_timer3() {
    // Set Timer 3 for input capture on rising edge (ICES3 = 1)
    set(TCCR3B, ICES3);

    // Enable Timer 3 overflow interrupt
    set(TIMSK3, TOIE3);

    // Set prescaler to /64
    set(TCCR3B, CS30); // Set CS30 bit
    set(TCCR3B, CS31);   // Clear CS31 bit
    clear(TCCR3B, CS32);   // Set CS32 bit
}

// Timer 3 overflow interrupt service routine
//Used GPT for this ISR part to introduce an ISR for the overflow coutner
ISR(TIMER3_OVF_vect) {
    overflow_count++;  // Increment overflow count each time Timer 3 overflows
}

void frequencyDetect() {
    // Wait until input capture flag is set
    while (!bit_is_set(TIFR3, ICF3));

    // Clear the input capture flag
    set(TIFR3, ICF3);

    // Measure time between edges (in clock ticks)
    int current_time = ICR3;
    //Using the overflow method as given in the slides
    period = (overflow_count * 65536) + current_time - oldtime;
    oldtime = current_time;

    // Reset overflow counter since the period has been calculated
    overflow_count = 0;

    // Calculate frequency in Hz (with prescaler 64, clock frequency 16MHz)
    if (period > 0) {
        // Ticks per second using a /64 prescaler = 16 MHz / 64 = 250000 ticks per second
        frequency = (float)250000.0 / (period);
        PRINTNUM((int)frequency);  // Print the frequency for debugging
    }

    // Turn on GREEN LED if frequency is 25Hz (2.4.2)
    if (frequency > 23 && frequency < 27) {
        set(PORTF, 6);   // Turn on GREEN LED
        clear(PORTF, 1);  // Turn off RED LED
    }
    // Turn on RED LED if frequency is 662Hz (2.4.1)
    //A very small range so that it can be turned on and off easily on putting my hand between sensor and LED
    else if (frequency > 660 && frequency < 663) {
        set(PORTF, 1);   // Turn on RED LED
        clear(PORTF, 6);   // Turn off GREEN LED
    }
    //For the extra credit 2.4.2 EC
    //Turning off green and red LED and turning on blue LED if 200 Hz source
    else if (frequency > 195 && frequency < 205) {
        clear(PORTF, 1);   // Turn off RED LED
        clear(PORTF, 6);   // Turn off GREEN LED
        set(PORTC, 6);   // Turn on BLUE LED
    }
    // Turn off both LEDs if neither frequency is detected
    else {
        clear(PORTF, 6);   // Turn off RED LED
        clear(PORTF, 1);  // Turn off GREEN LED
        clear(PORTC, 6);   // Turn off BLUE LED
    }
}

void setupLEDs() {
    // Set up LEDs as output
    set(DDRF, 6);   // Set RED LED pin as output (Port F6)
    set(DDRF, 1);   // Set GREEN LED pin as output (Port F1)
    set(DDRC, 6);   // Set BLUE LED pin as output (Port C6)
    // Set up C7 as an input
    clear(DDRC, 7); // Set pin C7 as input (amplifier input)
}

int main24(void) {
    m_usb_init();               // Initialize USB communication
    sei();                      // Enable global interrupts
    clear(PORTC, 7);            // Pin C7 is configured as an input pin
    init_timer3();              // Initialize Timer 3 for input capture
    setupLEDs();                // Set up LED pins as outputs

    while (1) {
        // Measure time between two input signals
        frequencyDetect();
    }
}
