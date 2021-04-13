First, the RIOT application is compiled with the following options:

```
$ BUILD_IN_DOCKER=1 PICOLIBC=1 BOARD=nucleo-f767zi make -C examples/suit_update
```

The memory usage of the resulting binary is then analysed using the following command: 

```
$ bloaty -c suit.bloaty -d sections,suit -n 0 -s vm <PATH TO .ELF FILE>
```

The configuration file for Bloaty (`suit.bloaty`) can be found in /src/evaluation/memory_usage/.

The flash memory usage is calculated by adding together the `.text` and `.relocate` sections.
The RAM usage is calculated by adding together the `.bss`, `.stack` and `.relocate` sections.

To see how the compile units (i.e. files) or symbols (i.e. function or variable names) are grouped into the components (“core”, “network”, etc.), run the command using `-d sections,suit,compileunits` or `-d sections,suit,symbols`.

To compare two binaries, append `-- <PATH TO SECOND .ELF FILE>` to the command.
