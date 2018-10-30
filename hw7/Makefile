# Source Files
SOURCE1 = hw7.c
SOURCE2 = gsl_test.c

# Program Files
PROG1 = hw7
PROG2 = gsl_test 

# Input Files
GE1	= tests/ge1.txt
GE2	= tests/ge2.txt
GE3	= tests/ge3.txt
GE4	= tests/ge4.txt
RAND	= tests/rand.txt

# Output File
OUT  = out.txt
MEM = mem.txt

# C Things
# --------
# CC: gcc
# CFLAGS: warnings all, c99, pedantic?, debugging, optimization 0
# LFLAGS: math library
CC     = gcc 
CFLAGS = -Wall -std=c99 -pedantic -g -O1 -lm -lgsl -lgslcblas

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
all: $(PROG1) $(PROG2)


# # # # # # # # # # PROG1 # # # # # # # # # # 
.PHONY:$(PROG1)
$(PROG1): $(SOURCE1) 
	$(CC) $(SOURCE1) -o $(PROG1) $(CFLAGS)


# # # # # # # # # # PROG2 # # # # # # # # # # 
.PHONY:$(PROG2)
$(PROG2): $(SOURCE2) 
	$(CC) $(SOURCE2) -o $(PROG2) $(CFLAGS)


# # # # # # # # # # TEST # # # # # # # # # # 
.PHONY: test
test: $(PROG1)
	-./$(PROG1) -input $(GE1) -v > $(OUT)
	-./$(PROG1) -input $(GE2) -v >> $(OUT)
	-./$(PROG1) -input $(GE3) -v >> $(OUT)
	-./$(PROG1) -input $(GE4) -v >> $(OUT)
	cat $(OUT)


# # # # # # # # # # GSL # # # # # # # # # # 
.PHONY: gsl
gsl: $(PROG2)
	-./$(PROG2) -input $(GE1) -v &> $(OUT)
	-./$(PROG2) -input $(GE2) -v &>> $(OUT) 
	-./$(PROG2) -input $(GE3) -v &>> $(OUT)
	-./$(PROG2) -input $(GE4) -v &>> $(OUT)
	cat $(OUT)


# # # # # # # # # # MEM # # # # # # # # # # 
.PHONY:mem
mem: $(PROG1)
	$(VAL) $(VFLAGS) ./$(PROG1) -input $(GE2) -v &> $(MEM)
	cat $(MEM)


# # # # # # # # # # TIME # # # # # # # # # # 
.PHONY: time
time: $(SOURCE1) $(SOURCE2)
	$(CC) $(SOURCE1) -o $(PROG1) $(CFLAGS) -DEN_TIME
	$(CC) $(SOURCE2) -o $(PROG2) $(CFLAGS) -DEN_TIME
	./$(PROG1) -input $(RAND) > $(OUT) 
	./$(PROG2) -input $(RAND) &>> $(OUT)
	cat $(OUT)


# # # # # # # # # # Help # # # # # # # # # # 
.PHONY:help
help:
	@echo    all: builds $(SOURCE1), $(SOURCE2), and $(SOURCE3).
	@echo   test: runs $(PROG1) with $(GE1) - $(GE4) and writes results to $(OUT).
	@echo 	 gsl: runs $(PROG2) with $(GE1) - $(GE4) and writes results to $(OUT).
	@echo    mem: runs $(PROG1) with $(GE2) and valgrind then writes results to $(MEM).
	@echo   time: runs $(PROG1) and $(PROG2) then writes results to $(OUT).
	@echo  clean: removes program and output.


# # # # # # # # # # Clean # # # # # # # # # # 
.PHONY:clean
clean:
	-rm -f $(OUT)
	-rm -f $(MEM)
	-rm -f $(PROG1)
	-rm -f $(PROG2)
