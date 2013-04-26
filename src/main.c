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


uint8_t baudrateH, baudrateL;
    

int8_t set_baud_rate(uint16_t baud) {
    uint16_t baudrate;

    baudrate = (float)F_CPU/16/baud-1;
    baudrateH = baudrate >> 8;
    baudrateL = baudrate & 0xff;
    UBRR0H = baudrateH; //(F_CPU/16/BAUD-1)>>8;
    UBRR0L = baudrateL; //(F_CPU/16/BAUD-1)&&0xff;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0); // enable uart
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); //8 bits
    return(1);
}

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
    /* set_baud_rate(115200); */
    stdin = stdout = &uart_stream;

    /* An entire screen update is 80 characters, but fgets() may snarf 82
     * when it includes the CRLF terminator.*/
    char data[82];
    char x;
    uint8_t pos = 0, i = 0;

    // write message to LCD
    lcd_home();
    lcd_write_string(PSTR("LCD Miner v0.1"));
    /* lcd_line_two(); */
    /* fprintf_P(&lcd_stream,PSTR("Testing-%i-%i"),baudrateH,baudrateL); */


    while(1) {
        if (uart_char_is_waiting()) {
            x = uart_read();

            if (x == '\r' || x == '\n') {
                // render
                lcd_home();
                fprintf_P(&lcd_stream, PSTR("%s"), data);

                pos = 0;
                data[pos] = '\0';
            }
            else {
                data[pos++] = x;
                data[pos] = '\0';
                if (pos > 80)
                    pos--;
                /* lcd_line_four(); */
                /* fprintf_P(&lcd_stream,PSTR("%i"),pos); */
            }
        }
    }

    return 0;
}
