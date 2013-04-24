// tempsensor.c
// for NerdKits with ATmega168
// mrobbins@mit.edu

#define F_CPU 14745600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    /* An entire screen update is 80 characters, but fgets() may snarf 82
     * when it includes the CRLF terminator.*/
    char data[82];

    // write message to LCD
    lcd_home();
    lcd_write_string(PSTR("LCD Miner v0.1"));

    while(1) {
        fgets(data, 82, stdin);
        data[strlen(data)-1] = '\0';
        lcd_home();
        fprintf_P(&lcd_stream, PSTR("%s"), data);
    }

    return 0;
}
