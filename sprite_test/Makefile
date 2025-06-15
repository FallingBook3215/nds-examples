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

#---------------------------------------------------------------------------------
ARCH	:=	-march=armv5te -mtune=arm946e-s -mthumb

CFLAGS	:=	-g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH)
CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:= -lnds9 -lfat

LIBDIRS	:=	$(LIBNDS)

#---------------------------------------------------------------------------------
export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif
#---------------------------------------------------------------------------------

export OFILES	:=	assets/Sniff-Cat.o $(BUILD)/main.o

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

#---------------------------------------------------------------------------------
.PHONY: assets grit clean $(BUILD)

# Create build directory if missing
$(BUILD):
	@mkdir -p $@

# Generate C and H from PNG with grit
assets/Sniff-Cat.c assets/Sniff-Cat.h: assets/Sniff-Cat.png
	grit $< -g -gB8 -p -o assets/Sniff-Cat

# Compile generated sprite source
assets/Sniff-Cat.o: assets/Sniff-Cat.c assets/Sniff-Cat.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main.cpp, depends on the sprite header and build directory
$(BUILD)/main.o: source/main.cpp assets/Sniff-Cat.h | $(BUILD)
	$(CXX) $(CFLAGS) -c $< -o $@

# Link executable
$(OUTPUT).elf: $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)

# Create .nds from ELF
$(OUTPUT).nds: $(OUTPUT).elf
	ndstool -c $@ -9 $< -b 0x02000000

# Clean up
clean:
	@echo Cleaning project...
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds assets/*.c assets/*.h assets/*.o

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
