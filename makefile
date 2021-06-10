CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic # Flags
GLOBALS_FILES = globals.h 
ALL_OBJ =  exeptions.o first_pass.o utils.o binary.o

assembler: $(ALL_OBJ) $(GLOBALS_FILES)
	$(CC) -g assembler.c $(CFLAGS) $(ALL_OBJ) -o $@

exeptions.o: exeptions.c exeptions.h $(GLOBALS_FILES)
	$(CC) -c  exeptions.c $(CFLAGS) -o $@

first_pass.o: first_pass.c first_pass.h $(GLOBALS_FILES)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

utils.o: utils.c utils.h $(GLOBALS_FILES)
	$(CC) -c utils.c $(CFLAGS) -o $@

binary.o: binary.c binary.h $(GLOBALS_FILES)
	$(CC) -c binary.c $(CFLAGS) -o $@
