# Compiler/linker options
CROSS_COMPILE = avr-
CC := $(CROSS_COMPILE)gcc
MMCU = atmega328p
CFLAGS := -Os -DF_CPU=16000000UL -mmcu=$(MMCU) -c
LDFLAGS := -mmcu=$(MMCU) 
OUT_NAME = clock

# Which files to be compiled and linked
C_FILES := i2c.c ds3231.c led.c clock.c main.c
ASM_FILES := ws2812b.S
OBJ_FILES := $(notdir $(C_FILES:.c=.o))
OBJ_FILES += $(notdir $(ASM_FILES:.S=.o))

# avrdude options
AVR_PORT ?= /dev/cu.usbmodem1421
AVR_DEVICE ?= ATMEGA328P
AVR_PROGRAMMER ?= arduino
AVR_BAUD ?= 115200
AVR_FLAGS += -c $(AVR_PROGRAMMER) -p $(AVR_DEVICE) -P $(AVR_PORT) -b $(AVR_BAUD)

all: $(OBJ_FILES) link hex

$(OBJ_FILES): $(C_FILES) $(ASM_FILES)

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

.S.o:
	$(CC) $(CFLAGS) -o $@ $<

link:
	avr-gcc $(LDFLAGS) $(OBJ_FILES) -o $(OUT_NAME).elf

hex:
	avr-objcopy -O ihex -R .eeprom $(OUT_NAME).elf $(OUT_NAME).hex

clean:
	rm -f *.o *.hex *.elf 

avrdude:
	avrdude $(AVR_FLAGS) -U flash:w:$(OUT_NAME).hex

teensy_loader:
	teensy_loader_cli -mmcu=atmega32u4 -w $(OUT_NAME).hex

.PHONY: all clean link hex flash

