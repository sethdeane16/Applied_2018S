# Source Files
SOURCE1 = TestList.c
SOURCE2 = TestSearch.c
SOURCE3 = TestTime.c
SOURCE4 = LinkedLists.c
SOURCE5 = DynamicArrays.c

# Program Files
PROG1 = TestList
PROG2 = TestSearch 
PROG3 = TestTime
PROG4 = LinkedLists
PROG5 = DynamicArrays

# Output File
OUT  = out.txt
MEM = mem.txt

# C Things
# --------
# CC: gcc
# CFLAGS: warnings all, c99, pedantic?, debugging, optimization 0
# LFLAGS: math library
CC     = gcc 
CFLAGS = -Wall -std=c99 -pedantic -g -O0

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
all: $(PROG1) $(PROG2) $(PROG3)


# # # # # # # # # # TestList # # # # # # # # # #
#
# build
.PHONY:$(PROG1)
$(PROG1): $(SOURCE1) $(SOURCE4) $(SOURCE5)
	$(CC) $(SOURCE1) $(SOURCE4) $(SOURCE5) -o $(PROG1) $(CFLAGS)

# test
.PHONY:test
test: $(SOURCE1) $(SOURCE4) $(SOURCE5)
	./$(PROG1) tests/us-eng-words.txt &> out.txt
	cat out.txt


# # # # # # # # # # TestSearch # # # # # # # # # #
#
# build
.PHONY:$(PROG2)
$(PROG2): $(SOURCE2) $(SOURCE4) $(SOURCE5)
	$(CC) $(SOURCE2) $(SOURCE4) $(SOURCE5) -o $(PROG2) $(CFLAGS) -DSEARCHWORD

# test
.PHONY:search
search: $(SOURCE2) $(SOURCE4) $(SOURCE5)
	./$(PROG2) tests/us-eng-words.txt space &> search.txt
	cat search.txt


# # # # # # # # # # TestTime # # # # # # # # # # 
#
# build
.PHONY:$(PROG3)
$(PROG3): $(SOURCE3) $(SOURCE4) $(SOURCE5)
	$(CC) $(SOURCE3) $(SOURCE4) $(SOURCE5) -o $(PROG3) $(CFLAGS) -DEN_TIME

# test
.PHONY:time
time: $(SOURCE3) $(SOURCE4) $(SOURCE5)
	./$(PROG3) tests/us-eng-words.txt space &> time.txt
	cat time.txt


# # # # # # # # # # Valgrind # # # # # # # # # # 
.PHONY:mem
mem: $(PROG1)
	$(VAL) $(VFLAGS) ./$(PROG1) tests/oneTest.txt &> mem.txt
	cat mem.txt
	

# # # # # # # # # # Help # # # # # # # # # # 
.PHONY:help
help:
	@echo    all: builds $(SOURCE1), $(SOURCE2), and $(SOURCE3).
	@echo   test: runs $(PROG1) and wirtes results to $(OUT).
	@echo search: runs $(PROG2) and writes results to search.txt.
	@echo   time: runs $(PROG3) and writes results to time.txt.
	@echo    mem: runs $(PROG1) with valgrind and writes rsults to $(MEM).
	@echo  clean: removes program and output.


# # # # # # # # # # Clean # # # # # # # # # # 
.PHONY:clean
clean:
	-rm -f $(OUT)
	-rm -f $(MEM)
	-rm -f time.txt
	-rm -f search.txt
	-rm -f $(PROG1)
	-rm -f $(PROG2)
	-rm -f $(PROG3)
	-rm -f $(PROG4)
	-rm -f $(PROG5)

