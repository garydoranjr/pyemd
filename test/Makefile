CC = gcc
# -g : allows use of GNU Debugger
# -Wall : show all warnings
FLAGS = -g -Wall -lm
COMMON = ../c_emd/emd.c
HEADER = ../c_emd/emd.h
INCLUDE = -I../c_emd
MAIN = test.o

all: $(MAIN)

%.o: %.c $(COMMON)
	@# Call the compiler with source & output arguments
	$(CC) $(LIBS) $(INCLUDE) $(FLAGS) -o $@ $^
	@# Make the output file executable
	chmod 755 $@

test.c: $(COMMON) $(HEADER)

clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	@rm -f $(MAIN)

test: all
	./$(MAIN)

memtest: all
	valgrind ./$(MAIN)
