# Project
TARGET := sprite_test
BUILD := build
SRC := source
ASSETS := assets

# Toolchain
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
LD := arm-none-eabi-g++
OBJCOPY := arm-none-eabi-objcopy

# Flags
ARCH := -march=armv5te -mtune=arm946e-s -mthumb
CFLAGS := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH) -DARM9
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
LDFLAGS := -specs=ds_arm9.specs -g $(ARCH)
LIBS := -lfat -lnds9

# Includes
INCLUDES := -Iinclude -I$(ASSETS) -I/opt/devkitpro/libnds/include
CFLAGS += $(INCLUDES)
CXXFLAGS += $(INCLUDES)

# Files
PNG := $(ASSETS)/Sniff-Cat.png
GFX_C := $(ASSETS)/Sniff-Cat.c
GFX_H := $(ASSETS)/Sniff-Cat.h
GFX_O := $(ASSETS)/Sniff-Cat.o
MAIN_O := $(BUILD)/main.o

.PHONY: all clean

all: $(TARGET).nds

# Convert PNG to .c/.h with grit
$(GFX_C) $(GFX_H): $(PNG)
	grit $< -g -gB8 -p -o $(ASSETS)/Sniff-Cat

# Compile assets
$(GFX_O): $(GFX_C)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main.cpp
$(MAIN_O): $(SRC)/main.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link
$(TARGET).elf: $(MAIN_O) $(GFX_O)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

# Create .nds file
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

# Make build dir
$(BUILD):
	mkdir -p $(BUILD)

# Clean
clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds $(GFX_C) $(GFX_H) $(GFX_O)
