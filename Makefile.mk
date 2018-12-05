
# default targets (tests)
tests ?= src/popcnt.c src/popcnt_u4.c src/popcnt_a4.c \
	src/pchase.c src/sysbench_cpu.c src/sysbench_cpu_o1.c

# default goal
.DEFAULT_GOAL := default

# target architecture specifics
ARCH ?= blend
export ARCH
CFLAGS_blend = -msse4.2 -mpopcnt
CFLAGS_native = -march=native

# optimization level
OPTLEVEL ?= base
export OPTLEVEL
CFLAGS_noopt = -O0 $(CFLAGS_$(ARCH))
CFLAGS_base = -O2 $(CFLAGS_$(ARCH))
CFLAGS_fast = -Ofast $(CFLAGS_$(ARCH))
CFLAGS_OPT := $(CFLAGS_$(OPTLEVEL))

# general flags
INCL_DIR = .
CFLAGS += -MT $@ -MMD -MP -MF $(OUTDIR)/$*.Td
CFLAGS += $(addprefix -I, $(INCL_DIR))
CFLAGS += -g

# linker flags
LDFLAGS += -lm

# toolchain
CC = gcc
LD = gcc
OD = objdump

# version
BUILD_REV := $(shell git describe --always --dirty)
BUILD_BRANCH := $(shell git describe --all)
BUILD_VARIANT := "$(CC).$(ARCH).$(OPTLEVEL)"
CFLAGS += -DBUILD_REV="\"$(BUILD_REV)\""
CFLAGS += -DBUILD_BRANCH="\"$(BUILD_BRANCH)\""
CFLAGS += -DBUILD_VARIANT="\"$(BUILD_VARIANT)\""
CFLAGS += -DBUILD_DATE="\"$(BUILD_DATE)\""

# intermediates
OBJDIR ?= obj
OUTDIR := $(OBJDIR)/$(ARCH)_$(OPTLEVEL)
$(OUTDIR):
	mkdir -p $@

executables:
	mkdir $@

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

$(exes_c): $(OUTDIR)/%.exe : $(OUTDIR)/%.o $(MORE_DEPS) | $(OUTDIR) executables
	$(LD) -o $@ $(filter %.o,$^) $(LDFLAGS)
	$(OD) -d $@ >  $@.listing
	cp -f $@ executables/$(notdir $(@:.exe=.$(ARCH).$(OPTLEVEL).exe))

default: $(exes_c)

clean:
	rm -rf $(objs_c) $(deps_c) $(exes_c)

clobber: clean
	rm -rf $(OUTDIR) *.$(ARCH).$(OPTLEVEL).exe

nuke:
	rm -rf obj executables
	git clean -fdn

run-%:
	$(addprefix ./executables/, $(notdir $(*:.exe=.$(ARCH).$(OPTLEVEL).exe)))

run: $(exes_c)
	$(MAKE) -f Makefile.mk $(addprefix run-,$(notdir $(exes_c))) tests="$(tests)"

# debugging
print-%  : ; @echo $* = $($*)

$(deps_c): ;
.PRECIOUS: $(deps_c)
-include $(deps_c)

.PHONY: default clean clobber nuke run
