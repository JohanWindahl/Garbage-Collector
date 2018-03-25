# IOOP/M Björnen

## Folders
* [src](src) 	- Source files
* [test](test) - Test files
* [obj](obj) 	- Object files
* [bin](bin) 	- Full (linked) binaries (including test-binaries)
* [doc](doc) 	- Documentation configuration & destination

## Compiling
Compiling files, tests or the entire project is done using `make` from the root
directory of the repository. For help with available make-targets, simply type
`make help` in your terminal.

### Tests
All binaries, including the tests, will end up in the [bin](/bin) folder. For
further info on the test files (making, running, etc.), see README of
[test](/test).

## MAKE
A "living" list of targets and their use will also be added below.
``` make

	make help       # Displays available Make-targets

	make docs       # Clear & re-make documentation in /doc

	make clean      # Clean out all temporary- and binary files
	
	make obj        # Compiles all source files in src/ to object files in obj/.

	make bin        # Compiles object files and builds static library.

	obj/%.o         # Compiles src/%.c to object file obj/%.o
```
