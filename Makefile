COMPILER=gcc
OUTPUT_FILE=parkingTickets
FLAGS=-pedantic -std=c99 -lm -Wall -fsanitize=address -g
FRONT=main.c loadFile.c queries.c
BACK=ticketsADT.o utils.o plateTree.o

CHI: back.o
	$(COMPILER) -o $(OUTPUT_FILE)CHI -DCHI $(FRONT) $(BACK) $(FLAGS)

NYC: back.o
	$(COMPILER) -o $(OUTPUT_FILE)NYC -DNYC $(FRONT) $(BACK) $(FLAGS)

back.o:
	$(COMPILER) -c ticketsADT/ticketsADT.c ticketsADT/plateTree.c ticketsADT/utils.c

clean:
	rm -r *.o
