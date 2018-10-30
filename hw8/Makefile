# Source Files
SOURCE1 = hw8.c
SOURCE2 = correction.c
SOURCE3 = myplot.c
SOURCE4 = DynamicArrays.c

# Program Files
PROG1 = hw8
PROG2 = correction
PROG3 = myplot

# Input Files
BI1	= givenbinaries/realDevice
BI2	= givenbinaries/detError
BI3	= givenbinaries/diffVal

# Output File
OUT  = out.txt
MEM = mem.txt
PNG = plotfile.png
REGDAT  = data/data.txt
DEVDAT  = data/realdevice.txt
CORDAT  = data/correction.txt
ALLDAT  = data/alldata.txt

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
all: $(PROG1) $(PROG2) $(PROG3)
	./$(BI1) | ./$(BI3) > $(REGDAT)


# # # # # # # # # # PROG1 # # # # # # # # # # 
.PHONY:$(PROG1)
$(PROG1): $(SOURCE1) $(SOURCE4)
	./$(BI1) | ./$(BI3) > $(REGDAT)
	$(CC) $(SOURCE1) $(SOURCE4) -o $(PROG1) $(CFLAGS) -DHW8


# # # # # # # # # # PROG2 # # # # # # # # # # 
.PHONY:$(PROG2)
$(PROG2): $(SOURCE2)
	./$(BI1) | ./$(BI3) > $(REGDAT)
	$(CC) $(SOURCE2) -o $(PROG2) $(CFLAGS)


# # # # # # # # # # PROG3 # # # # # # # # # # 
.PHONY:$(PROG3)
$(PROG3): $(SOURCE3)
	./$(BI1) | ./$(BI3) > $(REGDAT)
	$(CC) $(SOURCE3) -o $(PROG3) -Wall -pedantic -g -O1 -lm


# # # # # # # # # # base # # # # # # # # # # 
.PHONY:base
base:
	./$(BI1) | ./$(BI3) > $(REGDAT)
	./$(BI1) | ./$(BI2) > $(OUT)
	cat $(OUT)


# # # # # # # # # # correct # # # # # # # # # # 
.PHONY:correct
correct: $(BI2) $(PROG2)
	./$(BI1) | ./$(BI3) > $(REGDAT)
	./$(BI1) | ./$(PROG2) | ./$(BI2) > $(CORDAT)
	cat $(CORDAT)


# # # # # # # # # # plot # # # # # # # # # # 
.PHONY:plot
plot: $(PROG3) 
	./$(BI1) | ./$(BI3) > $(REGDAT)
	./$(BI1) > $(DEVDAT)
	./$(BI1) | ./$(PROG2) > $(CORDAT)
	paste $(DEVDAT) $(CORDAT) > $(ALLDAT)
	./myplot -in $(ALLDAT) -out $(PNG)


# # # # # # # # # # MEM # # # # # # # # # # 
.PHONY:mem
mem: $(PROG1)
	$(VAL) $(VFLAGS) ./$(PROG1) -ge -o 3 -p $(REGDAT) &> $(MEM)
	cat $(MEM)


# # # # # # # # # # Help # # # # # # # # # # 
.PHONY:help
help:
	@echo     all: builds $(PROG1), $(PROG2), and $(PROG3).
	@echo    base: runs $(PROG1) with $(GE1) - $(GE4) and writes results to $(OUT).
	@echo correct: runs $(PROG2) with $(GE1) - $(GE4) and writes results to $(CORDAT).
	@echo  	 plot: runs $(PROG3) with the corrected data.
	@echo     mem: runs $(PROG1) with $(GE2) and valgrind then writes results to $(MEM).
	@echo    help: You found me :)
	@echo   clean: removes program and output.


# # # # # # # # # # Clean # # # # # # # # # # 
.PHONY:clean
clean:
	-rm -f $(OUT)
	-rm -f $(MEM)
	-rm -f $(PNG)
	-rm -f $(DEVDAT)
	-rm -f $(REGDAT)
	-rm -f $(CORDAT)
	-rm -f $(ALLDAT)
	-rm -f $(PROG1)
	-rm -f $(PROG2)
	-rm -f $(PROG3)
