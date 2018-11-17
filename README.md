# MARSH: Performance Testing Suite

MARSH is suite of short performance exploration tests for (micro)architecture
explorations. It's primarily intended for hardware characterization and
performance debugging, however could be used for testing compiler optimizations
as well.

## Jump start

Reference run:

```bash
make run
```

Highly optimized run:

```bash
make run OPTLEVEL=fast
```

Highly optimized run with host specific (micro)architecture optimizations:

```bash
make run OPTLEVEL=fast ARCH=native
```

Get rid of intermediate files for current or specific build type.
It leaves intact test executable(s) at the root folder:

```bash
make clean
make clean OPTLEVEL=fast ARCH=native
```

Also removes test executable(s) at the root folder:

```bash
make clobber
make clobber OPTLEVEL=fast ARCH=native
```

Removes everything and reports any left obstacles:

```bash
make nuke
```
