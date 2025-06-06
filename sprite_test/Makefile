# Project name (binary output name)
TARGET := sprite_test
BUILD := build
SOURCES := source/main.cpp
INCLUDES := -Iinclude -Iassets -I/opt/devkitpro/libnds/include

# Image assets
GFX := assets/Sniff-Cat.png
GFX_C := assets/Sniff-Cat.c
GFX_H := assets/Sniff-Cat.h
GFX_O := assets/Sniff-Cat.o

CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++

CFLAGS := -specs=ds_arm9.specs -mcpu=arm946e-s -mthumb -DARM9 $(INCLUDES)

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
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main program (depend on GFX_H so changes rebuild main.o)
$(BUILD)/main.o: $(SOURCES) $(GFX_H)
	$(CXX) $(CFLAGS) -c $< -o $@

# Link final ELF (add CPU and thumb flags here too)
$(TARGET).elf: $(BUILD)/main.o $(GFX_O)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

# Create NDS
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

clean:
	rm -rf $(BUILD) *.nds *.elf assets/*.c assets/*.h assets/*.o
