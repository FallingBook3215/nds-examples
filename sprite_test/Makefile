#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# Project settings
#---------------------------------------------------------------------------------
TARGET      := $(shell basename $(CURDIR))
BUILD       := build
SOURCES     := source
INCLUDES    := include build

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH    := -march=armv5te -mtune=arm946e-s -mthumb

CFLAGS  := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH)
CFLAGS  += $(INCLUDE) -DARM9
CXXFLAGS    := $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS := -g $(ARCH)
LDFLAGS = -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS    := -lnds9
LIBDIRS := $(LIBNDS)

#---------------------------------------------------------------------------------
# Asset rules
#---------------------------------------------------------------------------------
ASSETDIR := assets
PNG_ASSETS := $(basename $(notdir $(wildcard $(ASSETDIR)/*.png)))
GRIT_HEADERS := $(addprefix $(ASSETDIR)/,$(addsuffix .h,$(PNG_ASSETS)))
GRIT_SFILES  := $(addprefix $(ASSETDIR)/,$(addsuffix .s,$(PNG_ASSETS)))

# Rule to generate .h and .s from .png and .grit
$(ASSETDIR)/%.h: $(ASSETDIR)/%.png $(ASSETDIR)/%.grit
	grit $< -fts -o$(ASSETDIR)/$*

$(ASSETDIR)/%.s: $(ASSETDIR)/%.png $(ASSETDIR)/%.grit
	grit $< -fts -o$(ASSETDIR)/$*

#---------------------------------------------------------------------------------
# Build system
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
# Out-of-source build

export OUTPUT   := $(CURDIR)/$(TARGET)
export VPATH    := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR  := $(CURDIR)/$(BUILD)

CFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES    := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES    := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

ifeq ($(strip $(CPPFILES)),)
    export LD := $(CC)
else
    export LD := $(CXX)
endif

export OFILES    := $(BINFILES:.bin=.o) \
                    $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE   := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                    $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                    -I$(CURDIR)/$(BUILD)

export LIBPATHS  := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean assets

# Ensure assets are generated before building
$(BUILD): assets
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

assets: $(GRIT_HEADERS) $(GRIT_SFILES)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba
	@rm -f $(ASSETDIR)/*.h $(ASSETDIR)/*.s

#---------------------------------------------------------------------------------
else
# In-source build

DEPENDS := $(OFILES:.o=.d)

# Main targets
$(OUTPUT).nds   :   $(OUTPUT).elf
$(OUTPUT).elf   :   $(OFILES)

# Make all object files depend on all generated headers
$(OFILES): $(GRIT_HEADERS)

-include $(DEPENDS)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba
	@rm -f $(ASSETDIR)/*.h $(ASSETDIR)/*.s

endif
#---------------------------------------------------------------------------------------
