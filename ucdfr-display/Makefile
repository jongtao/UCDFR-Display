CC=avr-gcc
#MMCU=atmega328p
MMCU=at90usb1286
MMCU_CODE=usb1286
#ARDCOREDIR=-I/usr/share/arduino/hardware/arduino/cores/arduino/
#ARDPINDIR=-I/usr/share/arduino/hardware/arduino/variants/standard
#ARDUINO=$(ARDCOREDIR) $(ARDPINDIR)


all: ucdfrdisplay.hex

ucdfrdisplay.hex: ucdfrdisplay.elf
	avr-objcopy -j .text -j .data -O ihex ucdfrdisplay.elf ucdfrdisplay.hex

ucdfrdisplay.elf: ucdfr-display.o lcd.o graphics.o
	$(CC) -mmcu=$(MMCU) -Wall -Os -o $@ ucdfr-display.o lcd.o graphics.o


ucdfr-display.o: ucdfr-display.c
	$(CC) -Os -mmcu=$(MMCU) -c ucdfr-display.c

lcd.o: lcd.c lcd.h
	$(CC) -Os -mmcu=$(MMCU) -c lcd.c

graphics.o: graphics.c graphics.h
	$(CC) -Os -mmcu=$(MMCU) -c graphics.c

upload: ucdfrdisplay.hex
	teensy_loader_cli -mmcu=at90usb1286 -v -w ucdfrdisplay.hex
	#avrdude -c arduino -p $(MMCU_CODE) -Pusb -e -U flash:w:ucdfrdisplay.hex


clean:
	rm -f *.o ucdfrdisplay.elf
