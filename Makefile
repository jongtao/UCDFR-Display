UPFIVE=../../../../../
DIR=$(UPFIVE)home/jongtao/Arduino/ucdfr-display/
SKETCH=$(DIR)ucdfr-display.ino
BOARD=arduino:avr:uno
PORT=/dev/ttyACM0

all:
	arduino --verify --board $(BOARD) $(SKETCH)

upload:
	arduino --upload --board $(BOARD) --port $(PORT) $(SKETCH)
