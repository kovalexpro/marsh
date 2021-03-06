stream_tests := $(wildcard src/stream/stream_*.c)
people_tests := $(wildcard src/popcnt*.c) src/pchase.c
sysbench_tests := $(wildcard src/sysbench/*.c)

active_tests := $(stream_tests) $(people_tests) $(sysbench_tests)

SOURCE_DATE_EPOCH ?= $(shell date -u +%s)
BUILD_DATE = $(shell date -u -R --date @${SOURCE_DATE_EPOCH})
export BUILD_DATE

# default goal
.DEFAULT_GOAL := default

build-%:
	$(MAKE) -f Makefile.mk tests="$($*_tests)"

build:
	$(MAKE) -f Makefile.mk tests="$(active_tests)"

run-%:
	$(MAKE) -f Makefile.mk run tests="$($*_tests)"

run:
	$(MAKE) -f Makefile.mk run tests="$(active_tests)"

nuke:
	$(MAKE) -f Makefile.mk nuke
	rm -rf marsh_report*.txt

default: run

report: nuke
	@$(MAKE) -s -f Makefile.mk run tests="$(active_tests)" > marsh_report_raw.txt
	@python marsh_report.py -i marsh_report_raw.txt > marsh_report.txt
	@cat marsh_report.txt

summary:
	@rm -f marsh_summary.txt
	@touch marsh_summary.txt
	@echo --------------------------------------------------------------------- >> marsh_summary.txt
	@$(MAKE) -s -f Makefile.mk run tests="$(active_tests)" > marsh_report_raw.txt
	@python marsh_report.py -i marsh_report_raw.txt >> marsh_summary.txt
	@echo --------------------------------------------------------------------- >> marsh_summary.txt
	@$(MAKE) -s -f Makefile.mk run tests="$(active_tests)" OPTLEVEL=fast > marsh_report_raw.txt
	@python marsh_report.py -i marsh_report_raw.txt >> marsh_summary.txt
	@echo --------------------------------------------------------------------- >> marsh_summary.txt
	@$(MAKE) -s -f Makefile.mk run tests="$(active_tests)" ARCH=native > marsh_report_raw.txt
	@python marsh_report.py -i marsh_report_raw.txt >> marsh_summary.txt
	@echo --------------------------------------------------------------------- >> marsh_summary.txt
	@$(MAKE) -s -f Makefile.mk run tests="$(active_tests)" ARCH=native OPTLEVEL=fast > marsh_report_raw.txt
	@python marsh_report.py -i marsh_report_raw.txt >> marsh_summary.txt
