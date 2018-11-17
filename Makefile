
# default targets (tests)
tests ?= src/popcnt.c src/popcnt_u4.c src/popcnt_a4.c

# default goal
.PHONY: default
.DEFAULT_GOAL := default

# target architecture specifics
ARCH ?= blend
CFLAGS_blend = -msse4.2 -mpopcnt
CFLAGS_native = -march=native

# optimization level
OPTLEVEL ?= base
CFLAGS_noopt = -O0 $(CFLAGS_$(ARCH))
CFLAGS_base = -O2 $(CFLAGS_$(ARCH))
CFLAGS_fast = -Ofast $(CFLAGS_$(ARCH))
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
$(OUTDIR)/$(DRIVER).o: $(DRIVER) $(OUTDIR)/$(DRIVER).d Makefile | $(OUTDIR)
	$(CC) -c -O0 $(CFLAGS) $< -o $@ -MT $@ -MMD -MP -MF $*.Td
	@mv $*.Td $*.d && touch $@

# framework
MARSH = marsh.c
$(OUTDIR)/$(MARSH).o: $(MARSH) $(OUTDIR)/$(MARSH).d Makefile | $(OUTDIR)
	$(CC) -c -O0 $(CFLAGS) $< -o $@ -MT $@ -MMD -MP -MF $*.Td
	@mv $*.Td $*.d && touch $@

# C-written tests
vpath %.c $(dir $(filter %.c,$(tests)))
files = $(notdir $(tests))
files_c = $(filter %.c,$(files))
objs_c = $(addprefix $(OUTDIR)/, $(files_c:.c=.o))
deps_c = $(addprefix $(OUTDIR)/, $(files_c:.c=.d)) $(OUTDIR)/$(DRIVER).d $(OUTDIR)/$(MARSH).d
exes_c = $(addprefix $(OUTDIR)/, $(files_c:.c=.exe))

MORE_DEPS = $(OUTDIR)/$(DRIVER).o $(OUTDIR)/$(MARSH).o Makefile

$(objs_c): $(OUTDIR)/%.o : %.c $(OUTDIR)/%.d $(MORE_DEPS) | $(OUTDIR)
	$(CC) -c $(CFLAGS) $(CFLAGS_OPT) $< -o $@
	@mv $(OUTDIR)/$*.Td $(OUTDIR)/$*.d && touch $@

$(exes_c): $(OUTDIR)/%.exe : $(OUTDIR)/%.o $(MORE_DEPS) | $(OUTDIR)
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^)
	cp -f $@ $(notdir $(@:.exe=.$(ARCH).$(OPTLEVEL).exe))

default: $(exes_c)

clean:
	rm -rf $(objs_c) $(deps_c) $(exes_c)

clobber: clean
	rm -rf $(OUTDIR) *.exe

nuke:
	rm -rf obj *.exe

# debugging
print-%  : ; @echo $* = $($*)

$(deps_c): ;
.PRECIOUS: $(deps_c)
-include $(deps_c)
