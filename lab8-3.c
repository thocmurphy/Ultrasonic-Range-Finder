/**********************
 Thomas Murphy Lab 8.3
***********************/

#define TRIG PC5 // PC5
#define ECHO PC4 // PC4
#define RANGE_PER_CLOCK 1.098
#define PERSISTENCE 2
#define COUNTTIME 50

#include <avr/io.h>
#include <util/delay.h>
#include <string.h> // Defines strlen() function
#include <stdlib.h> // Defines itoa() function

void timer0_init(void);

int main(void)
{
    unsigned char rising_edge_clocks, falling_edge_clocks, echo_width_clocks;
    float target_range;
    DDRC = 1 << TRIG;      // TRIG is output pin;
    PORTC &= ~(1 << TRIG); // Set the TRIG pin low
    timer0_init();         // Initialize timer0

    unsigned char ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
    unsigned char DIG1, DIG2, DIG3, DIG4;
    DDRD = 0xFF;
    DDRB = 0xFF;

    while (1)
    {
        unsigned int r = (int)target_range;

        DIG1 = r % 10;         // 1's digit
        DIG2 = (r / 10) % 10;  // 10's digit
        DIG3 = (r / 100) % 10; // 100's digit
        DIG4 = (r / 1000);     // 1000's digit

        PORTD = ledDigits[DIG1]; // digit 1
        PORTB &= ~(1 << 3); // turn the LED on
        _delay_ms(PERSISTENCE);
        PORTB |= (1 << 3);  // turn the LED off

        PORTD = ledDigits[DIG2]; // digit 2
        PORTB &= ~(1 << 2); // turn the LED on
        _delay_ms(PERSISTENCE);
        PORTB |= (1 << 2);  // turn the LED off

        PORTD = ledDigits[DIG3]; // digit 3
        PORTB &= ~(1 << 1); // turn the LED on
        _delay_ms(PERSISTENCE);
        PORTB |= (1 << 1);  // turn the LED off

        PORTD = ledDigits[DIG4]; // digit 4
        PORTB &= ~(1 << 0); // turn the LED on
        _delay_ms(PERSISTENCE);
        PORTB |= (1 << 0);  // turn the LED off

        TCNT0 = 0;             // Load counter with 0
        PORTC |= 1 << TRIG;    // These three lines of code
        _delay_us(10);         // Put a 10 usec pulse on the
        PORTC &= ~(1 << TRIG); // TRIG pin.

        // Wait till the ECHO pulse goes high
        while ((PINC & (1 << ECHO)) == 0)
            ;
        rising_edge_clocks = TCNT0; // Note the time
        // Now wait till the ECHO pulse goes low
        while (!((PINC & (1 << ECHO)) == 0))
            ;
        falling_edge_clocks = TCNT0;

        if (falling_edge_clocks > rising_edge_clocks)
        {
            // Compute target range and send it to the serial monitor
            echo_width_clocks = falling_edge_clocks - rising_edge_clocks;
            target_range = echo_width_clocks * RANGE_PER_CLOCK;
        }
    }
    return 0;
}

// Initialize timer0: normal mode (count up), divide clock by 1024
void timer0_init()
{
    TCCR0A = 0; // Timer 1 Normal mode (count up)
    TCCR0B = 5; // Divide clock by 1024
    TCNT0 = 0;  // Start the timer at 0
}