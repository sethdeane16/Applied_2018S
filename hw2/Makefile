# Source Files
SOURCE1 = DynamicArrays.c
SOURCE2 = TestDarray_hw.c 

# Object Files
OBJ1   = $(patsubst %.c, %.o, $(SOURCE1))
OBJ2   = $(patsubst %.c, %.o, $(SOURCE2))

# Program Files
PROG1 = TestDarray_hw

# Output File
OUT  = out.txt
MEM = mem.txt

# C Things
# --------
# CC: gcc
# CFLAGS: warnings all, c99, O1 optimization, pedantic?, debugging
# LFLAGS: math library
CC     = gcc 
CFLAGS = -Wall -std=c99 -O1 -pedantic -g
LFLAGS = -lm

# Valgrind Things
# ---------------
# VAL: valgrind
# VFLAGS: flags to run valgrind
VAL    = valgrind
VFLAGS = --tool=memcheck --leak-check=yes --track-origins=yes -v

# GDB Things
# ----------
# GDB: gdb
GDB = gdb

# Dont print out each execution line
.SILENT:

#############################################################################
.PHONY:all
all: $(PROG1)

$(OBJ1): $(SOURCE1)
	$(CC) $(SOURCE1) -c $(CFLAGS)

$(OBJ2): $(SOURCE2)
	$(CC) $(SOURCE2) -c $(CFLAGS)

$(PROG1): $(OBJ1)$(OBJ2)
	$(CC) $(OBJ1) $(OBJ2) -o $(PROG1) $(CFLAGS)

.PHONY:test
test: $(PROG1)
	./$(PROG1) us-eng-words.txt > $(OUT)
	cat $(OUT)

.PHONY:mem
mem: $(PROG1)
	$(VAL) $(VGLAGS) ./$(PROG1) us-eng-words.txt &> $(MEM)
	cat $(MEM)

.PHONY:help
help:
	@echo all: builds $(SOURCE1)
	@echo test: runs tests identified by homework prompt for "$(PROG1)" then
	@echo       writes $(OUT).
	@echo mem: runs standard valgrind command on "$(PROG1)" with good equation and\
	           then writes and displays "$(MEM)"
	@echo clean: removes temporary and immediate files.

.PHONY:clean
clean:
	-rm -f $(OUT)
	-rm -f $(MEM)
	-rm -f $(PROG1)
	-rm -f $(OBJ1)
	-rm -f $(OBJ2)
