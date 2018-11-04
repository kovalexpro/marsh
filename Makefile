
# default targets (tests)
tests ?= src/popcnt.c src/popcnt_u4.c src/popcnt_a4.c

# default goal
.PHONY: default
.DEFAULT_GOAL := default

# target architecture specifics
ARCH ?= BLEND
CFLAGS_BLEND = -msse4.2 -mpopcnt
CFLAGS_NATIVE = -march=native

# optimization level
OPTLEVEL ?= BASE
CFLAGS_NOOPT = -O0 $(CFLAGS_$(ARCH))
CFLAGS_BASE = -O2 $(CFLAGS_$(ARCH))
CFLAGS_FAST = -Ofast $(CFLAGS_$(ARCH))
CFLAGS_OPT := $(CFLAGS_$(OPTLEVEL))

# general flags
INCL_DIR = .
CFLAGS += -MT $@ -MMD -MP -MF $(OUTDIR)/$*.Td
CFLAGS += $(addprefix -I, $(INCL_DIR))
CFLAGS += -g

# toolchain
CC = gcc
LD = gcc

# intermediates
OBJDIR ?= obj
OUTDIR := $(OBJDIR)/$(ARCH)_$(OPTLEVEL)
$(OUTDIR):
	mkdir -p $@

# test driver
DRIVER ?= single.c
$(OUTDIR)/$(DRIVER).o: $(DRIVER) $(OUTDIR)/$(DRIVER).d | $(OUTDIR)
	$(CC) -c -O0 $(CFLAGS) $< -o $@ -MT $@ -MMD -MP -MF $*.Td
	@mv $*.Td $*.d && touch $@

# C-written tests
vpath %.c $(dir $(filter %.c,$(tests)))
files = $(notdir $(tests))
files_c = $(filter %.c,$(files))
objs_c = $(addprefix $(OUTDIR)/, $(files_c:.c=.o))
deps_c = $(addprefix $(OUTDIR)/, $(files_c:.c=.d)) $(OUTDIR)/$(DRIVER).d
exes_c = $(addprefix $(OUTDIR)/, $(files_c:.c=.exe))

$(objs_c): $(OUTDIR)/%.o : %.c $(OUTDIR)/%.d $(OUTDIR)/$(DRIVER).o | $(OUTDIR)
	$(CC) -c $(CFLAGS) $(CFLAGS_OPT) $< -o $@
	@mv $(OUTDIR)/$*.Td $(OUTDIR)/$*.d && touch $@

$(exes_c): $(OUTDIR)/%.exe : $(OUTDIR)/%.o $(OUTDIR)/$(DRIVER).o | $(OUTDIR)
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^)
	cp -f $@ $(notdir $(@:.exe=.$(ARCH).$(OPTLEVEL).exe))

default: $(exes_c)

clean:
	rm -rf $(objs_c) $(deps_c)

clobber: clean
	rm -rf $(OUTDIR) $(exes_c)

nuke:
	rm -rf $(OBJDIR)

# debugging
print-%  : ; @echo $* = $($*)

$(deps_c): ;
.PRECIOUS: $(deps_c)
-include $(deps_c)
