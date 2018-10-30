# Source Files
SOURCE1 = hw3t.c
SOURCE2 = hw3opt.c
SOURCE3 = hw3t.cpp

# Program Files
PROG1 = hw3t_c
PROG2 = hw3opt
PROG3 = hw3t_cpp

# Output File
OUT  = out.txt
MEM = mem.txt

# C Things
# --------
# CC: gcc
# CFLAGS: warnings all, c99, O1 optimization, pedantic?, debugging
# LFLAGS: math library
CC     = gcc 
CFLAGS = -Wall -std=c99 -pedantic -g
LFLAGS = -lm


# CPP Things
# --------
# CPP: g++ things
CPP   = g++ -Wall -pedantic -O0 


# Dont print out each execution line
.SILENT:

#############################################################################
.PHONY:all
all: $(PROG1) $(PROG2) $(PROG3)


# HW3C
.PHONY:$(PROG1)
$(PROG1): $(SOURCE1) 
	$(CC) $(SOURCE1) -o $(PROG1) -O0 $(CFLAGS) -DEN_TIME

#HW3OPT
.PHONY:$(PROG2)
$(PROG2): $(SOURCE2)
	$(CC) $(SOURCE2) -o $(PROG2) -O0 $(CFLAGS) -DEN_TIME

# HW3CPP
.PHONY:$(PROG3)
$(PROG3): $(SOURCE3)
	$(CPP) $(SOURCE3) -o $(PROG3) -DEN_TIME


.PHONY:test
test: $(PROG1) $(PROG2) $(PROG3)
	@echo "Testing $(PROG1)"
	@echo "$(PROG1):" > $(OUT)
	@./$(PROG1) data.txt &>> $(OUT)
	@echo " " >> $(OUT)
	@echo "Testing $(PROG2)"
	@echo "$(PROG2):" >> $(OUT)
	@./$(PROG2) data.txt &>> $(OUT)
	@echo " " >> $(OUT)
	@echo "Testing $(PROG3)"
	@echo "$(PROG3):" >> $(OUT)
	@./$(PROG3) data.txt &>> $(OUT)


# TODO: redo this
.PHONY:help
help:
	@echo all: builds $(SOURCE1), $(SOURCE2), and $(SOURCE3) into programs.
	@echo test: runs timing information testing the performance of $(PROG1),
	@echo       $(PROG2), and $(PROG3) and writes that to $(OUT).
	@echo clean: removes temporary and immediate files.


.PHONY:clean
clean:
	-rm -f $(OUT)
	-rm -f $(MEM)
	-rm -f $(PROG1)
	-rm -f $(PROG2)
	-rm -f $(PROG3)
