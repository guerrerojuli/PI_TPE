#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "./ticketsADT.h"
#include "./funciones.c"

int main(int argc, char *argv[]) {
    if ( argc < 3 || argc > 5 ) {
        fprintf(stderr, "Error: cantidad de parámetros inválida\n");
        return ERROR;
    }

    // Creo mi ADT, en el cual guardaré los datos

    #ifdef CHI
        ticketsADT tickets = createTicketADT(argc, argv, MAX_INFRACTION_CHI, MAX_AGENCY_CHI, LONG_PATENTE);
    #else   
        ticketsADT tickets = createTicketADT(argc, argv, MAX_INFRACTION_NYC, MAX_AGENCY_NYC, LONG_PATENTE);
    #endif

    char* delimiters = ";\n"; // Delimitadores para strtok
    char buffer_line[MAX_LINE_LENGTH]; // Buffer donde voy guardando los strings que retorna de strtok

    // Primera parte: Leo argv[2], el cual será el archivo csv donde guardo infracciones con su respectivo id
    // Formarto: id;descripcion
    FILE *file_infr = openFile(argv[2]);
    loadInfractions(tickets, file_infr, delimiters, buffer_line);
    fclose(file_infr);

    // Segunda parte: Leo argv[1], el cual será el archivo csv donde guardo multas
    // Formarto: issueDate;plateRedacted;unitDescription;infrId;fineLevel1Amount
    FILE *file_tickets = openFile(argv[1]);

    #ifdef CHI
        loadTicketsCHI(tickets, file_tickets, delimiters, buffer_line);
    #else
        loadTicketsNYC(tickets, file_tickets, delimiters, buffer_line);
    #endif

    fclose(file_tickets);

    query1(tickets);
    query2(tickets);
    query3(tickets);
    query4(tickets);

    freeTickets(tickets);
    return 0;
}