MCU     = atmega328p
F_CPU   = 16000000UL
CC      = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
TARGET  = main

SRCS = main.c \
       drivers/spi.c \
       drivers/ili9341.c \
       inputs/button.c \
       font/font.c

all: $(TARGET).hex

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

flash: $(TARGET).hex
	avrdude -c arduino -p $(MCU) -P /dev/cu.usbmodem1301 -b 115200 -U flash:w:$(TARGET).hex

clean:
	rm -f *.elf *.hex