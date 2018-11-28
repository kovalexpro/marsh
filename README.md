# MARSH: Performance Testing Suite

MARSH is suite of short performance oriented tests for (micro)architecture
explorations. It's primarily intended for hardware characterization and
performance debugging, however could be used for testing compiler optimizations
as well.

## Jump start

Clone sources:

```console
git clone https://github.com/kovalexpro/marsh.git
```

Build relies on GNU's Make, GCC C compiler and binutils visible in default shell.
Reporting script is written in Python 2.7 syntax.

By default, MARSH is not requiring that much system resources except for processor.
Memory footprint should be around a megabyte. It's possible to run multiple kernels
in parallel - just add ```-j``` option to make's parameters (see below).

Reference run:

```console
make run
make run -j
```

Highly optimized run:

```console
make run OPTLEVEL=fast
```

Highly optimized run with host specific (micro)architecture optimizations:

```console
make run OPTLEVEL=fast ARCH=native
```

Get rid of intermediate files for default or specific build type.
It leaves intact test executable(s) at the root folder:

```console
make clean
make clean OPTLEVEL=fast ARCH=native
```

Also removes test executable(s) at the root folder:

```console
make clobber
make clobber OPTLEVEL=fast ARCH=native
```

Removes everything and reports any left obstacles:

```console
make nuke
```

## Collecting results for offline analysis

There is some reporting facility (script and make's goals) for generating
reports suitable for further analysis (like exporting to database or using
in spreadsheets).

For a single report for quick review (show results on screen and saves
them to marsh_report.txt as well):

```console
make report -j
make report -j OPTLEVEL=fast ARCH=native
```

For performance summary (saves results into marsh_summary.txt)
Don't supply ARCH and/or OPTLEVEL parameters - this time they're ignored:

```console
make summary -j
```
