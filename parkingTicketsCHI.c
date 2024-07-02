#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "./ticketsADT.h"
#include "./funciones.c"
#define MAX_INFRACTION_CHI 50
#define MAX_AGENCY_CHI 13
#define MAX_LINE_LENGTH 256 // Preguntar si es necesario que sea exactamente id + ; + name

int main(int argc, char *argv[]) {
    if ( argc < 3 || argc > 5 ) {
        // Salida por error de parámetros. Debo chequear potenciales archivos que rompar el programa
    }

    // Creo mi ADT, en el cual guardaré los datos
    ticketsADT ticketsCHI = createTicketADT(argc, argv, MAX_INFRACTION_CHI, MAX_AGENCY_CHI);
    char* delimiters = ";\n"; // Delimitadores para strtok
    char buffer_line[MAX_LINE_LENGTH]; // Buffer donde voy guardando los strings que retorna de strtok

    // Primera parte: Leo argv[2], el cual será el archivo csv donde guardo infracciones con su respectivo id
    // Formarto: id;descripcion
    FILE *file_infr = openFile(argv[2]); 
    loadInfractions(ticketsCHI, file_infr, delimiters, buffer_line);
    fclose(file_infr);

    // Segunda parte: Leo argv[1], el cual será el archivo csv donde guardo multas
    // Formarto: issueDate;plateRedacted;unitDescription;infrId;fineLevel1Amount
    FILE *file_tickets = openFile(argv[1]);
    loadTicketsCHI(ticketsCHI, file_tickets, delimiters, buffer_line);
    fclose(file_tickets);

    query1(ticketsCHI);
    query2(ticketsCHI);
    query3(ticketsCHI);
    query4(ticketsCHI);
}

// Especificamente para Chicago (Difiere el orden de los datos en el .csv con NYC)
void loadTicketsCHI(ticketsADT ticketsCHI, FILE *file_tickets, char* delimiters, char buffer_line[]) {
    tTicket ticket_aux; // Estructura donde iré guardando los datos de cada multa a medida que leo el archivo
    fgets(buffer_line, MAX_LINE_LENGTH, file_tickets); // Sacamos los nombre de los campos de la primer línea
    while ( fgets(buffer_line, MAX_LINE_LENGTH, file_tickets) != NULL ) {
        ticket_aux.year = atoi(strtok(buffer_line, '-')); // Guardo yyyy
        ticket_aux.month = atoi(strtok(NULL, '-')); // Guardo MM
        strtok(NULL, delimiters); // Acá esta dd HH-mm-SS que no me interesa, no lo guardo
        strcpy(ticket_aux.patente, strtok(NULL, delimiters));
        ticket_aux.agency = strtok(NULL, delimiters);
        ticket_aux.id = atoi(strtok(NULL, delimiters));
        strtok(NULL, delimiters); // No me interesa el monto
        insertTicket(ticket_aux, ticketsCHI);
    }
}