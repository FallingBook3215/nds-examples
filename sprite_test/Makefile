#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	include assets build

ARCH	:=	-march=armv5te -mtune=arm946e-s -mthumb

CFLAGS	:=	-g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH)
CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:= -lnds9 -lfat
LIBDIRS	:=	$(LIBNDS)

#---------------------------------------------------------------------------------
# Source files (C, C++, ASM)
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

# Choose linker (C++ if cpp files exist)
ifeq ($(strip $(CPPFILES)),)
	LD := $(CC)
else
	LD := $(CXX)
endif

# Object files (include grit asset objects)
OFILES	:= assets/Sniff-Cat.o $(addprefix $(BUILD)/,$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o))

# Include paths
INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

#---------------------------------------------------------------------------------
.PHONY: all clean

all: $(BUILD) assets/Sniff-Cat.c assets/Sniff-Cat.h $(TARGET).nds

# Create build directory
$(BUILD):
	@mkdir -p $@

# Convert PNG to C and H files with grit
assets/Sniff-Cat.c assets/Sniff-Cat.h: assets/Sniff-Cat.png
	grit $< -g -gB8 -p -o assets/Sniff-Cat

# Compile grit-generated assets
assets/Sniff-Cat.o: assets/Sniff-Cat.c assets/Sniff-Cat.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(BUILD)/%.o: source/%.cpp | $(BUILD)
	$(CXX) $(CFLAGS) -c $< -o $@

# Compile C source files
$(BUILD)/%.o: source/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly files
$(BUILD)/%.o: source/%.s | $(BUILD)
	$(AS) $(ASFLAGS) $< -o $@

# Link ELF
$(TARGET).elf: $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)

# Build final NDS file
$(TARGET).nds: $(TARGET).elf
	ndstool -c $@ -9 $< -b 0x02000000

clean:
	@echo Cleaning...
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds assets/*.c assets/*.h assets/*.o

#---------------------------------------------------------------------------------
