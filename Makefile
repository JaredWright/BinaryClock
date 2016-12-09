all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o blink.o blink.c
	avr-gcc -mmcu=atmega328p blink.o -o blink
	avr-objcopy -O ihex -R .eeprom blink blink.hex

clean:
	rm *.o *.hex blink

flash_osx:
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/cu.usbmodem14231 -b 115200 -U flash:w:blink.hex

