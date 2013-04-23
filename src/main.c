// tempsensor.c
// for NerdKits with ATmega168
// mrobbins@mit.edu

#define F_CPU 14745600

#include <stdio.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "lcdminer.h"


// PIN DEFINITIONS:
//
// PC1 -- LCD backlight output

int main() {
    // start up the LCD
    lcd_init();
    FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
    lcd_home();

    DDRC |= (1<<PC1);
    PORTC |= (1<<PC1);
    // TODO: Flash the LCD backlight if serial connection is lost

    // start up the serial port
    uart_init();
    FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,
            _FDEV_SETUP_RW);
    stdin = stdout = &uart_stream;

    uint8_t result;
    char tmp_c;
    int16_t count = 0;

    // write message to LCD
    lcd_home();
    lcd_write_string(PSTR("LCD Miner v0.1"));
    lcd_line_two();

    while(1) {
        result = scanf_P(PSTR("%c"), &tmp_c);
        if (result == 1) {
            lcd_line_two();
            lcd_write_byte(tmp_c);
            lcd_write_string(PSTR("     <   "));
        }
        lcd_line_three();
        lcd_write_string(PSTR("result: "));
        lcd_write_int16(result);
        lcd_write_string(PSTR(" count: "));
        lcd_write_int16(count++);

    }


    return 0;
}
