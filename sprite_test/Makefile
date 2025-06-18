#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

# Settings
TARGET     := sprite_test
BUILD      := build
SOURCES    := source
INCLUDES   := include assets

# Sprite asset info
SPRITE_PNG := assets/Sniff-Cat.png
SPRITE_C   := assets/Sniff-Cat.c
SPRITE_H   := assets/Sniff-Cat.h
SPRITE_O   := assets/Sniff-Cat.o

# Main source files
MAIN_CPP   := source/main.cpp
MAIN_O     := $(BUILD)/main.o

# Output
OUTPUT     := $(CURDIR)/$(TARGET)

ARCH       := -march=armv5te -mtune=arm946e-s -mthumb
CFLAGS     := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH) -DARM9
CXXFLAGS   := $(CFLAGS) -fno-rtti -fno-exceptions
LDFLAGS    := -specs=ds_arm9.specs -g $(ARCH)
LIBS       := -lfat -lnds9
INCLUDE    := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
               -I/opt/devkitpro/libnds/include

.PHONY: all clean

all: $(TARGET).nds

# Create build directory
$(BUILD):
	mkdir -p $@

# Use grit to generate .c/.h files from PNG
$(SPRITE_C) $(SPRITE_H): $(SPRITE_PNG)
	grit $< -g -gB8 -p -fh -o $(basename $<)

# Compile the sprite C file
$(SPRITE_O): $(SPRITE_C) $(SPRITE_H)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Compile the main source, depend on generated .h
$(MAIN_O): $(MAIN_CPP) $(SPRITE_H) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Link everything into ELF
$(TARGET).elf: $(MAIN_O) $(SPRITE_O)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# Convert ELF to NDS
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

clean:
	rm -rf $(BUILD) *.elf *.nds assets/*.c assets/*.h assets/*.o
