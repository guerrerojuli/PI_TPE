#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ticketsADT.h"
#include "funciones.c"
#define MAX_INFRACTION_NYC 30
#define MAX_AGENCY_NYC 35
#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[]) {
    if ( argc < 3 || argc > 5 ) {
        // salida por error
    }

    ticketsADT ticketsNYC = createTicketADT(argc, argv, MAX_INFRACTION_NYC, MAX_AGENCY_NYC);
    char* delimiters = ";\n"; 
    char buffer_line[MAX_LINE_LENGTH];

    FILE *file_infr = openFile(argv[2]);
    loadInfractions(ticketsNYC, file_infr, delimiters, buffer_line);
    fclose(file_infr);

    FILE *file_tickets = openFile(argv[1]);
    loadTickets(ticketsNYC, file_tickets, delimiters, buffer_line);
    fclose(file_tickets);

    query1(ticketsNYC);
    query2(ticketsNYC);
    query3(ticketsNYC);
    query4(ticketsNYC);

    freeTickets(ticketsNYC);
}

void loadTicketsNYC(ticketsADT ticketsNYC, FILE *file_tickets, char *delimiters, char buffer_line[]){
    tTicket ticket_aux;
    fgets(buffer_line, MAX_LINE_LENGTH, file_tickets);
    while ( fgets(buffer_line, MAX_LINE_LENGTH, file_tickets) != NULL ) {
        strcpy(ticket_aux.patente, strtok(buffer_line, delimiters)); 
        ticket_aux.year = atoi(strtok(NULL, "-"));       
        ticket_aux.month = (char) atoi(strtok(NULL, "-")); // atoi me da int, casteo a char¿?
        strtok(NULL, delimiters); // descarto el dia
        strtok(NULL, delimiters); // descarto el valor de la multa
        ticket_aux.agency = strtok(NULL, delimiters);
        insertTicket(ticket_aux, ticketsNYC);
    }
}

/*
plate;issueDate;infractionId;fineAmount;issuingAgency
KJZ9028;2022-05-23;21;65;TRAFFIC
JFS2559;2020-03-06;38;35;TRAFFIC
JPP3277;2021-08-06;36;50;DEPARTMENT OF TRANSPORTATION

typedef struct ticket {
  size_t id;
  char *agency;
  size_t year;
  char month;
  char patente[10];
} tTicket;
*/