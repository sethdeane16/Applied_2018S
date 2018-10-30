# Source Files
SOURCE1 = hw6.c
SOURCE2 = poly.c

# Object Files
OBJ1  	= $(patsubst %.c, %.o, $(SOURCE1))
OBJ2  	= $(patsubst %.c, %.o, $(SOURCE2))

# Program Files
PROG1 = hw6

# Input Files
IFILE  = tests/polynomials.txt
#IFILE  = tests/simple.txt

# Output Files
OUT  = out.txt
MEM = mem.txt

# C Things
# --------
# CC: gcc
# CFLAGS: warnings all, c99, pedantic?, debugging, optimization 0
# LFLAGS: math library
CC     = gcc 
CFLAGS = -Wall -std=c99 -pedantic -g -O1 -lm

# VALGRING THINGS
# ---------------
#  VAL: valgrind
#  VFLAGS: Flags to run valgrind
VAL    = valgrind
VFLAGS = --tool=memcheck --leak-check=yes

# Dont print out each execution line
.SILENT:

#############################################################################

.PHONY:all
all: $(PROG1)


.PHONY:$(OBJ1)
$(OBJ1): $(SOURCE1)
	$(CC) $(SOURCE1) -c $(CFLAGS)	

.PHONY:$(OBJ2)
$(OBJ2): $(SOURCE2)
	$(CC) $(SOURCE2) -c $(CFLAGS)	

.PHONY:$(PROG1)
$(PROG1): $(OBJ1) $(OBJ2)
	$(CC) $(OBJ1) $(OBJ2) -o $(PROG1) $(CFLAGS)

.PHONY:roots
roots: $(PROG1)
	./$(PROG1) -i $(IFILE) -v &> $(OUT)
	cat $(OUT)

.PHONY:mem
mem: $(PROG1)
	$(VAL) $(VFLAGS) ./$(PROG1) -i $(IFILE) &> $(MEM)
	cat mem.txt
	

# # # # # # # # # # Help # # # # # # # # # # 
.PHONY:help
help:
	@echo    all: builds $(SOURCE1) and $(SOURCE2) into binary.
	@echo  roots: runs $(PROG1) with $(IFILE) and writes results to $(OUT).
	@echo    mem: runs $(PROG1) with valgrind and writes rsults to $(MEM).
	@echo  clean: removes program and output.


# # # # # # # # # # Clean # # # # # # # # # # 
.PHONY:clean
clean:
	-rm -f $(OUT)
	-rm -f $(MEM)
	-rm -f $(PROG1)
	-rm -f $(OBJ1)
	-rm -f $(OBJ2)

