COMPILER=gcc
DEBUG_COMPILER=-g -DDEBUG=1
OUTPUT_FILE=ejecutable
FLAGS=-pedantic -std=c99 -lm -Wall -fsanitize=address -g
SOURCES=main.c  funciones.c
OBJECTS=ticketsADT.o utils.o insertTicket.o

CHI: back.o
	$(COMPILER) -o $(OUTPUT_FILE)_CHI -DCHI $(SOURCES) $(OBJECTS) $(FLAGS)
	clean

NYC: back.o
	$(COMPILER) -o $(OUTPUT_FILE)_NYC -DNYC $(SOURCES) $(OBJECTS) $(FLAGS)
	clean

back.o:
	$(COMPILER) -c ticketsADT.c utils.c insertTicket.c

clean:
	rm -r *.o

# debug: COMPILER+=$(DEBUG_COMPILER)
# debug: all
