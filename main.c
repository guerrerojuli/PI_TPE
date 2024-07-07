#include <stdio.h>
#include "ticketsADT/ticketsADT.h"
#include "main.h"

#include "queries.c"
#include "funciones.c"

int main(int argc, char *argv[]) {
    if ( argc < 3 || argc > 5 ) {
        fprintf(stderr, "Error: cantidad de parámetros inválida\n");
        return ERROR;
    }

    // Creo mi ADT, en el cual guardaré los datos
    ticketsADT tickets = createTicketADT(argc, argv, MAX_INFRACTION, MAX_AGENCY, LONG_PATENTE);


    // Primera parte: Leo argv[2], el cual será el archivo csv donde guardo infracciones con su respectivo id
    // Formarto: id;descripcion
    FILE *file_infr = openFile(argv[2]);
    loadWithBlocks(tickets, file_infr, processBufferInfractions);
    fclose(file_infr);

    // Segunda parte: Leo argv[1], el cual será el archivo csv donde guardo multas
    // Formarto: issueDate;plateRedacted;unitDescription;infrId;fineLevel1Amount
    FILE *file_tickets = openFile(argv[1]);

    loadWithBlocks(tickets, file_tickets, processBufferTickets);

    fclose(file_tickets);

    query1(tickets);
    query2(tickets);
    query3(tickets);
    query4(tickets);

    freeTickets(tickets);
    return 0;
}
