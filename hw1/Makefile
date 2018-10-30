# Source Files
SOURCE1 = QuadraticSolver.c
SOURCE2 = val.c

# Object Files
OBJ1   = $(patsubst %.c, %.o, $(SOURCE1))
OBJ2   = $(patsubst %.c, %.o, $(SOURCE2))

# Program Files
PROG1 = qs
PROG2 = val

# Output Files
RESULTS = out.txt
MEMORY = mem.txt

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
VFLAGS = --tool=memcheck --leak-check=yes --track-origins=yes

# Dont print out each execution line
.SILENT:

#############################################################################
.PHONY:all
all: $(PROG1) $(PROG2)

$(OBJ1): $(SOURCE1)
	$(CC) $(SOURCE1) -c $(CFLAGS)

$(OBJ2): $(SOURCE2)
	$(CC) $(SOURCE2) -c $(CFLAGS)

$(PROG1): $(OBJ1)
	$(CC) $(OBJ1) -o $(PROG1) $(CFLAGS) $(LFLAGS)

$(PROG2): $(OBJ2)
	$(CC) $(OBJ2) -o $(PROG2) $(CFLAGS)

.PHONY:test
test: $(PROG1)
	./$(PROG1) 2 2392.2 -766267.2 > $(RESULTS)
	./$(PROG1) 1 -1 1 >> $(RESULTS)
	./$(PROG1) 0 1 -10 >> $(RESULTS)
	cat $(RESULTS)

.PHONY:mem1
mem1: $(PROG1)
	$(VAL) $(VGLAGS) ./$(PROG1) s2 4 8 &> $(MEMORY)
	cat $(MEMORY)

.PHONY:mem2
mem2: $(PROG2)
	$(VAL) $(VFLAGS) ./$(PROG2) &>> $(MEMORY)
	cat $(MEMORY)

.PHONY:help
help:
	@echo all: builds $(SOURCE1) and $(SOURCE2)
	@echo test: runs tests identified by homework prompt for "$(PROG1)" then writes
	@echo       and displays "$(RESULTS)"
	@echo mem1: runs standard valgrind command on "$(PROG1)" with good equation and\
	            then writes and displays "$(MEMORY)"
	@echo mem2: runs standard valgrind command on "$(PROG2)" then writes and displays\
	            "$(MEMORY)"
	@echo clean: removes temporary and immediate files.

.PHONY:clean
clean:
	-rm -f $(RESULTS)
	-rm -f $(MEMORY)
	-rm -f $(PROG1)
	-rm -f $(PROG2)
	-rm -f $(OBJ1)
	-rm -f $(OBJ2)
