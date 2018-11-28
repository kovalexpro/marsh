stream_tests := $(wildcard src/stream/stream_*.c)
people_tests := $(wildcard src/popcnt_*.c) src/pchase.c
sysbench_tests := $(wildcard src/sysbench/*.c)

active_tests := $(stream_tests) $(people_tests) $(sysbench_tests)

SOURCE_DATE_EPOCH ?= $(shell date -u +%s)
BUILD_DATE = $(shell date -u -R --date @${SOURCE_DATE_EPOCH})
export BUILD_DATE

# default goal
.DEFAULT_GOAL := default

build-%:
	$(MAKE) -f Makefile.mk tests="$($*_tests)"

buid:
	$(MAKE) -f Makefile.mk tests="$(active_tests)"

run-%:
	$(MAKE) -f Makefile.mk run tests="$($*_tests)"

run:
	$(MAKE) -f Makefile.mk run tests="$(active_tests)"

nuke:
	$(MAKE) -f Makefile.mk nuke

default: run

report:
	@$(MAKE) -s -f Makefile.mk run tests="$(active_tests)" > marsh_report_raw.txt