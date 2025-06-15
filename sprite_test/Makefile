TARGET := sprite_test
BUILD := build
SOURCES := source/main.cpp
INCLUDES := -Iinclude -Iassets -I/opt/devkitpro/libnds/include

# Asset files
GFX := assets/Sniff-Cat.png
GFX_C := assets/Sniff-Cat.c
GFX_H := assets/Sniff-Cat.h
GFX_O := assets/Sniff-Cat.o

CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
CFLAGS := -specs=ds_arm9.specs -march=armv5te -mthumb -DARM9 $(INCLUDES)
LIBS := -lnds9 -lfat

.PHONY: all clean

all: $(BUILD) $(TARGET).nds

$(BUILD):
	mkdir -p $(BUILD)

# Generate .c and .h files from PNG with grit
$(GFX_C) $(GFX_H): $(GFX)
	grit $< -g -gB8 -p -o assets/Sniff-Cat

# Compile the generated C asset file
$(GFX_O): $(GFX_C) $(GFX_H)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main source, depends on the generated header
$(BUILD)/main.o: $(SOURCES) $(GFX_H) | $(BUILD)
	$(CXX) $(CFLAGS) -c $< -o $@

# Link objects
$(TARGET).elf: $(BUILD)/main.o $(GFX_O)
	$(CXX) -specs=ds_arm9.specs -o $@ $^ $(LIBS)

# Create the final NDS file
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds assets/*.c assets/*.h assets/*.o
