CC=avr-gcc
MMCU=at90usb1286
MMCU_CODE=usb1286
OPTIMIZE=-O3
WARN=-Wall



# Rules

all: ucdfrdisplay.hex

ucdfrdisplay.hex: ucdfrdisplay.elf
	avr-objcopy -j .text -j .data -O ihex ucdfrdisplay.elf ucdfrdisplay.hex



ucdfrdisplay.elf: ucdfr-display.o lcd.o graphics.o engine.o
	$(CC) $(OPTIMIZE) $(WARN) -mmcu=$(MMCU) -o $@ $^

ucdfr-display.o: ucdfr-display.c lcd.h graphics.h engine.h
	$(CC) $(OPTIMIZE) $(WARN) -mmcu=$(MMCU) -c $<

lcd.o: lcd.c lcd.h
	$(CC) $(OPTIMIZE) $(WARN) -mmcu=$(MMCU) -c $<

graphics.o: graphics.c graphics.h
	$(CC) $(OPTIMIZE) $(WARN) -mmcu=$(MMCU) -c $<

engine.o: engine.c engine.h
	$(CC) $(OPTIMIZE) $(WARN) -mmcu=$(MMCU) -c $<



upload: ucdfrdisplay.hex
	teensy_loader_cli -mmcu=at90usb1286 -v -w ucdfrdisplay.hex



clean:
	rm -f *.o ucdfrdisplay.elf
