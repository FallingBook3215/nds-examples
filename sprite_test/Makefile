#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# Settings
#---------------------------------------------------------------------------------
TARGET     := sprite_test
BUILD      := build
SOURCES    := source
INCLUDES   := include assets

#---------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------
GFX_PNG    := assets/Sniff-Cat.png
GFX_C      := assets/Sniff-Cat.c
GFX_H      := assets/Sniff-Cat.h
GFX_O      := assets/Sniff-Cat.o
MAIN_CPP   := source/main.cpp
MAIN_O     := $(BUILD)/main.o

#---------------------------------------------------------------------------------
# Output
#---------------------------------------------------------------------------------
OUTPUT     := $(CURDIR)/$(TARGET)

#---------------------------------------------------------------------------------
# Includes, flags, libs
#---------------------------------------------------------------------------------
ARCH       := -march=armv5te -mtune=arm946e-s -mthumb

CFLAGS     := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH) -DARM9
CXXFLAGS   := $(CFLAGS) -fno-rtti -fno-exceptions

LDFLAGS    := -specs=ds_arm9.specs -g $(ARCH)
LIBS       := -lfat -lnds9

INCLUDE    := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
              -I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# Rules
#---------------------------------------------------------------------------------
.PHONY: all clean

all: $(TARGET).nds

# Create build directory
$(BUILD):
	mkdir -p $@

# Generate graphics from PNG using grit
$(GFX_C) $(GFX_H): $(GFX_PNG)
	grit $< -g -gB8 -p -o $(basename $<)

# Compile sprite C file
$(GFX_O): $(GFX_C) $(GFX_H)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Compile main.cpp (wait for Sniff-Cat.h)
$(MAIN_O): $(MAIN_CPP) $(GFX_H) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Link everything into ELF
$(TARGET).elf: $(MAIN_O) $(GFX_O)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# Convert ELF to NDS
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

# Clean files
clean:
	rm -rf $(BUILD) *.elf *.nds assets/*.c assets/*.h assets/*.o
