# Project name (binary output name)
TARGET := sprite_test
BUILD := build
SOURCES := source
INCLUDES := include assets

# Image assets
GFX := assets/Sniff-Cat.png
GFX_C := assets/Sniff-Cat.c
GFX_H := assets/Sniff-Cat.h
GFX_O := assets/Sniff-Cat.o

# Compiler commands (can be overridden by environment)
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++

LIBS := -lfat -lnds9

.PHONY: all clean

all: $(BUILD) $(TARGET).nds

$(BUILD):
	mkdir -p $(BUILD)

# Grit conversion: generate .c/.h from image
$(GFX_C) $(GFX_H): $(GFX)
	grit $< -g -gB8 -p -o assets/Sniff-Cat

# Compile sprite graphics
$(GFX_O): $(GFX_C)
	$(CC) -c $< -o $@

# Compile main program
$(BUILD)/main.o: source/main.cpp
	arm-none-eabi-g++ -specs=ds_arm9.specs -c $< -o $@ -Iinclude -Iassets -I/opt/devkitpro/libnds/include -DARM9

# Link final ELF
$(TARGET).elf: $(BUILD)/main.o $(GFX_O)
	$(CXX) -specs=ds_arm9.specs -o $@ $^ $(LIBS)

# Create NDS
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

clean:
	rm -rf $(BUILD) *.nds *.elf assets/*.c assets/*.h assets/*.o
