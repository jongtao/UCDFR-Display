CC=avr-gcc
MMCU=atmega328p
ARDCOREDIR=-I/usr/share/arduino/hardware/arduino/cores/arduino/
ARDPINDIR=-I/usr/share/arduino/hardware/arduino/variants/standard
ARDUINO=$(ARDCOREDIR) $(ARDPINDIR)


all: ucdfrdisplay.elf
	avr-objcopy -j .text -j .data -O ihex ucdfrdisplay.elf ucdfrdisplay.hex

ucdfrdisplay.elf: ucdfr-display.o lcd.o graphics.o logic.o
	$(CC) -mmcu=$(MMCU) -Wall -Os $(ARDUINO) -o $@ ucdfr-display.o lcd.o graphics.o logic.o


ucdfr-display.o: ucdfr-display.c
	$(CC) -Os -mmcu=$(MMCU) $(ARDUINO) -c ucdfr-display.c

lcd.o: lcd.c
	$(CC) -Os -mmcu=$(MMCU) $(ARDUINO) -c lcd.c

graphics.o: graphics.c
	$(CC) -Os -mmcu=$(MMCU) $(ARDUINO) -c graphics.c

logic.o: logic.o
	$(CC) -Os -mmcu=$(MMCU) $(ARDUINO) -c logic.c

upload:
	avrdude -c arduino -p m328p -P/dev/ttyACM0 -e -U flash:w:ucdfrdisplay.hex


clean:
	rm *.o ucdfrdisplay.elf
