CC=avr-gcc
CFLAGS=-g -O0 -Wall -mmcu=atmega168
LDFLAGS=-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm
AVRDUDEFLAGS=-c avr109 -p m168 -b 115200 -P /dev/ttyUSB0

SRC=delay.c lcd.c uart.c main.c
OBJ=${SRC:.c=.o}
BIN=lcdminer

all: ${BIN}-upload

%.o: %.c
	$(CC) ${CFLAGS} -o $@ -c $^

${BIN}: $(OBJ)
	$(CC) ${CFLAGS} ${LDFLAGS} -o $@ $^

%.hex: %
	avr-objcopy -j .text -O ihex $^ $@

%.as: %
	avr-objdump -S -d $^ > $@

%-upload: %.hex
	avrdude ${AVRDUDEFLAGS} -e
	sleep 0.1
	avrdude ${AVRDUDEFLAGS} -D -U flash:w:$^:a

clean:
	rm -f ${BIN} *.o *.hex *.as
