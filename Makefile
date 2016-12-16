# CROSS_COMPILE = avr-
# CC := $(CROSS_COMPILE)gcc
#
# C_FILES := led.c blink.c
# ASM_FILES := ws2812b.S
# OBJ_FILES := $(notdir $(C_FILES:.c=.o))
# OBJ_FILES += $(notdir $(ASM_FILES:.S=.o))
#
#
# all: $(OBJ_FILES)
#
# $(OBJ_FILES): $(C_FILES) $(ASM_FILES)
#
# %.o: %.c
#         $(CC) $(DBGFLAGS) -c $(INC) -o /$@ $<

all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o blink.o blink.c
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o led.o led.c
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o ws2812b.o ws2812b.S
	avr-gcc -mmcu=atmega328p blink.o led.o ws2812b.o -o blink
	avr-objcopy -O ihex -R .eeprom blink blink.hex

clean:
	rm *.o *.hex blink
	rm -r adafruit_neopixel

flash_osx:
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/cu.usbmodem1411 -b 115200 -U flash:w:blink.hex

neopixel:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o neopixel.o neopixel.S
