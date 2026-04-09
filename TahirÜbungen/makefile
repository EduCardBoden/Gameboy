CC = avr-gcc
CFLAGS = -mmcu=atmega328p -DF_CPU=16000000UL -Os
OBJCOPY = avr-objcopy

PORT = /dev/ttyACM0
PROGRAMMER = arduino
BAUD = 115200

# Directory for all build artifacts
BUILD_DIR := build

# Find all .c files (excluding testing)
SRC := $(shell find . -name "*.c" -not -path "./testing/*" -not -path "./$(BUILD_DIR)/*")
# Map source files to object files inside build/
OBJ := $(addprefix $(BUILD_DIR)/, $(SRC:.c=.o))

# Define final paths
ELF := $(BUILD_DIR)/main.elf
HEX := $(BUILD_DIR)/main.hex

all: $(HEX)
	@avr-size --format=avr --mcu=atmega328p $(ELF)
	@echo "Build complete: $(HEX)"

# Create hex from elf (both in build/)
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Link objects into elf
$(ELF): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Compile .c to .o in mirrored directory structure
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
flash: all
	sudo avrdude -c $(PROGRAMMER) -p m328p -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i
clean:
	@echo "Nuking the $(BUILD_DIR) folder..."
	rm -rf $(BUILD_DIR)