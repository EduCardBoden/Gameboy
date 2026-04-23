CC = avr-gcc
CFLAGS = -mmcu=atmega328p -DF_CPU=16000000UL -Os
OBJCOPY = avr-objcopy

PORT = /dev/ttyACM0
PROGRAMMER = arduino
BAUD = 115200

SRC = main.c

# Output im Parent-Ordner
OBJ = ../main.o
ELF = ../main.elf
HEX = ../main.hex

all: $(HEX)
	@avr-size --format=avr --mcu=atmega328p $(ELF)
	@echo "Build complete: $(HEX)"

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

flash: all
	sudo avrdude -c $(PROGRAMMER) -p m328p -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

clean:
	rm -f ../*.o ../*.elf ../*.hex