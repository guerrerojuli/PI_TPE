COMPILER=gcc
DEBUG_COMPILER=-g -DDEBUG=1
OUTPUT_FILE=ejecutable
FLAGS=-pedantic -std=c99 -lm -Wall -fsanitize=address -g
SOURCES=main.c
OBJECTS=ticketsADT.o utils.o plateTree.o

CHI: back.o
	$(COMPILER) -o $(OUTPUT_FILE)_CHI -DCHI $(SOURCES) $(OBJECTS) $(FLAGS)

NYC: back.o
	$(COMPILER) -o $(OUTPUT_FILE)_NYC -DNYC $(SOURCES) $(OBJECTS) $(FLAGS)

back.o:
	$(COMPILER) -c ticketsADT/ticketsADT.c ticketsADT/plateTree.c ticketsADT/utils.c

clean:
	rm -r *.o

# debug: COMPILER+=$(DEBUG_COMPILER)
# debug: all
