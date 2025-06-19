#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory for object and intermediate files
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# GRAPHICS is a list of directories containing files to be processed by grit
#---------------------------------------------------------------------------------
TARGET      := $(notdir $(CURDIR))
BUILD       := build
SOURCES     := source
INCLUDES    := include
GRAPHICS    := assets

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH        := -march=armv5te -mtune=arm946e-s -mthumb

CFLAGS      := -g -Wall -O2 -ffunction-sections -fdata-sections $(ARCH)
CFLAGS      += $(INCLUDE) -DARM9
CXXFLAGS    := $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS     := -g $(ARCH)
LDFLAGS     = -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS        := -lnds9
LIBDIRS     := $(LIBNDS)

#---------------------------------------------------------------------------------
# Asset rules
#---------------------------------------------------------------------------------
PNGFILES    := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
GRITFILES   := $(foreach f,$(PNGFILES),$(GRAPHICS)/$(basename $(f)).grit)

# This will generate .h and .s for every .png/.grit pair
OFILES_ASSETS := $(addprefix $(GRAPHICS)/,$(PNGFILES:.png=.o))
HFILES_ASSETS := $(addprefix $(GRAPHICS)/,$(PNGFILES:.png=.h))

#---------------------------------------------------------------------------------
# Build system
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT    := $(CURDIR)/$(TARGET)
export VPATH     := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))
export DEPSDIR   := $(CURDIR)/$(BUILD)

CFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES    := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

ifeq ($(strip $(CPPFILES)),)
    export LD := $(CC)
else
    export LD := $(CXX)
endif

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES         := $(OFILES_ASSETS) $(OFILES_SOURCES)
export INCLUDE        := $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
                         $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                         -I$(CURDIR)/$(BUILD)
export LIBPATHS       := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make BUILDDIR=`cd $(BUILD) && pwd` --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds
	@rm -f $(GRAPHICS)/*.h $(GRAPHICS)/*.s $(GRAPHICS)/*.o

#---------------------------------------------------------------------------------
else

DEPENDS     := $(OFILES:.o=.d)

# main targets
$(OUTPUT).nds   :   $(OUTPUT).elf
$(OUTPUT).elf   :   $(OFILES)

# Rule to generate assembly and header from png/grit
$(GRAPHICS)/%.s $(GRAPHICS)/%.h : $(GRAPHICS)/%.png $(GRAPHICS)/%.grit
	grit $< -fts -o$*

# Asset .o depends on .s
$(GRAPHICS)/%.o : $(GRAPHICS)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

-include $(DEPENDS)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds
	@rm -f $(GRAPHICS)/*.h $(GRAPHICS)/*.s $(GRAPHICS)/*.o

endif
#---------------------------------------------------------------------------------------
