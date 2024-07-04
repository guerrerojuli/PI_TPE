COMPILER=gcc
DEBUG_COMPILER=-g -DDEBUG=1
OUTPUT_FILE=ejecutable
FLAGS=-pedantic -std=c99 -lm -Wall -fsanitize=address -g

CHI: back.o
	$(COMPILER) -o $(OUTPUT_FILE)_CHI parkingTicketsCHI.c  funciones.c ticketsADT.o utils.o insertTicket.o $(FLAGS)
	clean

NYC: back.o
	$(COMPILER) -o $(OUTPUT_FILE)_NYC parkingTicketsNYC.c  funciones.c ticketsADT.o utils.o insertTicket.o $(FLAGS)
	clean

back.o:
	$(COMPILER) -c ticketsADT.c utils.c insertTicket.c

debug: COMPILER+=$(DEBUG_COMPILER)
debug: all

clean:
	rm -r $(OUTPUT_FILE)_* *.o