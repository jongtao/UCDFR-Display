CC=avr-gcc
STD=-std=c99
OPTIMIZE=-O3
WARN=-Wall

MMCU=at90usb1286
MMCU_CODE=usb1286



# Rules

all: ucdfrdisplay.hex

ucdfrdisplay.hex: ucdfrdisplay.elf
	avr-objcopy -j .text -j .data -O ihex ucdfrdisplay.elf ucdfrdisplay.hex



ucdfrdisplay.elf: ucdfr-display.o lcd.o graphics.o engine.o utilities.o
	$(CC) $(OPTIMIZE) $(WARN) $(STD) -mmcu=$(MMCU) -o $@ $^

ucdfr-display.o: ucdfr-display.c lcd.h graphics.h engine.h utilities.h
	$(CC) $(OPTIMIZE) $(WARN) $(STD) -mmcu=$(MMCU) -c $<

lcd.o: lcd.c lcd.h
	$(CC) $(OPTIMIZE) $(WARN) $(STD) -mmcu=$(MMCU) -c $<

graphics.o: graphics.c graphics.h
	$(CC) $(OPTIMIZE) $(WARN) $(STD) -mmcu=$(MMCU) -c $<

engine.o: engine.c engine.h
	$(CC) $(OPTIMIZE) $(WARN) $(STD) -mmcu=$(MMCU) -c $<

utilities.o: utilities.c utilities.h
	$(CC) $(OPTIMIZE) $(WARN) $(STD) -mmcu=$(MMCU) -c $<



upload: ucdfrdisplay.hex
	teensy_loader_cli -mmcu=at90usb1286 -v -w ucdfrdisplay.hex



clean:
	rm -f *.o ucdfrdisplay.elf
