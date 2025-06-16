#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# Project Settings
#---------------------------------------------------------------------------------
TARGET := sprite_test
BUILD := build
SOURCES := source
INCLUDES := assets

#---------------------------------------------------------------------------------
# File Definitions
#---------------------------------------------------------------------------------
ASSET_PNG := assets/Sniff-Cat.png
ASSET_C := assets/Sniff-Cat.c
ASSET_H := assets/Sniff-Cat.h
ASSET_O := assets/Sniff-Cat.o

#---------------------------------------------------------------------------------
ARCH := -march=armv5te -mtune=arm946e-s -mthumb
CFLAGS := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH)
CFLAGS += $(INCLUDE) -DARM9
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS := -lnds9 -lfat
LIBDIRS := $(LIBNDS)

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

export OFILES := $(BINFILES:.bin=.o) \
                 $(CPPFILES:.cpp=.o) \
                 $(CFILES:.c=.o) \
                 $(SFILES:.s=.o) \
                 $(notdir $(ASSET_O))

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                  $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                  -I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).nds assets/*.c assets/*.h assets/*.o

#---------------------------------------------------------------------------------
# Grit Rule: Convert PNG to .c/.h
#---------------------------------------------------------------------------------
$(ASSET_C) $(ASSET_H): $(ASSET_PNG)
	grit $< -g -gB8 -p -o assets/Sniff-Cat

#---------------------------------------------------------------------------------
# Compile Grit Output to .o
#---------------------------------------------------------------------------------
$(ASSET_O): $(ASSET_C) $(ASSET_H)
	$(CC) $(CFLAGS) -c $(ASSET_C) -o $(ASSET_O)

else

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).nds: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif
