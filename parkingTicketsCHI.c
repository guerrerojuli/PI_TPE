#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "./ticketsADT.h"
#define MAX_INFRACTION_CHI 50
#define MAX_AGENCY_CHI 13
#define MAX_LINE_LENGTH 256 // Preguntar si es necesario que sea exactamente id + ; + name

int main(int argc, char *argv[]) {
    if ( argc < 3 || argc > 5 ) {
        // Salida por error de parámetros. Debo chequear potenciales archivos que rompar el programa
    }

    // Creo mi ADT
    ticketsADT ticketsCHI = createTicketADT(argc, argv, MAX_INFRACTION_CHI, MAX_AGENCY_CHI);
    char* delimiters = ";\n"; // Delimitadores para strtok

    // Primera parte: Leo argv[2], el cual será el archivo csv donde guardo infracciones con su respectivo id
    // Formarto: id;descripcion
    FILE *file_infr = openFile(argv[2]);
    
    loadInfractions(ticketsCHI, file_infr, delimiters, MAX_INFRACTION_CHI);

    fclose(file_infr);

    // Segunda parte: Leo argv[1], el cual será el archivo csv donde guardo multas
    // Formarto: issueDate;plateRedacted;unitDescription;infrId;fineLevel1Amount
    FILE *file_tickets = openFile(argv[1]);

    loadTicketsCHI(ticketsCHI, file_tickets, delimiters);

    fclose(file_tickets);
}

FILE *openFile(char* arg) {
    FILE *res = fopen(arg, "r");
    if ( res == NULL ) {
        perror("Error al abrir archivo.");
        exit(1);
    }
    return res;
}

ticketsADT createTicketADT(int argc, char *argv[], size_t max_description, size_t max_agency_name) {
    time_t now;
    struct tm *local;
    time(&now);
    local = localtime(&now);
    int year = local->tm_year + 1900;
    ticketsADT tickets;
    if ( argc == 3 ) { // Si no me pasan el año de cierre, obtengo el actual con la librería time.h
        tickets = newTickets(0, year, max_description, max_agency_name);
    }
    else if ( argc == 4 ) {
        tickets = newTickets(atoi(argv[3]), year, max_description, max_agency_name);
    }
    else {
        int beginYear = atoi(argv[3]);
        int endYear = atoi(argv[4]);
        if ( beginYear > endYear ) {
            // Salida por error de los parámetros.
        }
        tickets = newTickets(beginYear, endYear, max_description, max_agency_name);
    }
    return tickets;
}

void loadInfractions(ticketsADT tickets, FILE *file_infr, char* delimiters, int max_infraction) {
    tInfraction infr_aux; // Estructura donde iré guardando los datos de cada infracción a medida que leo el archivo
    char aux_line[MAX_LINE_LENGTH];
    fgets(aux_line, MAX_LINE_LENGTH, file_infr); // Sacamos los nombre de los campos de la primer línea
    while ( fgets(aux_line, MAX_LINE_LENGTH, file_infr) != NULL ) {
        infr_aux.id = atoi(strtok(aux_line, delimiters));
        infr_aux.description = strtok(NULL, delimiters);
        insertInfraction(infr_aux, tickets, max_infraction);
        // Al mandar la descripcion, el CDT por atrás ya hace la copia. En el proximo bucle, se pisa
        // aux_line pero no interesa, ya que en el cdt vos tenes la copia
    }
}

// Especificamente para Chicago (Difiere el orden de los datos en el .csv con NYC)
void loadTicketsCHI(ticketsADT ticketsCHI, FILE *file_tickets, char* delimiters) {
    tTicket ticket_aux; // Estructura donde iré guardando los datos de cada multa a medida que leo el archivo
    char aux_line[MAX_LINE_LENGTH];
    fgets(aux_line, MAX_LINE_LENGTH, file_tickets); // Sacamos los nombre de los campos de la primer línea
    while ( fgets(aux_line, MAX_LINE_LENGTH, file_tickets) != NULL ) {
        ticket_aux.year = atoi(strtok(aux_line, '-')); // Guardo yyyy
        ticket_aux.month = atoi(strtok(NULL, '-')); // Guardo MM
        strtok(NULL, delimiters); // Acá esta dd HH-mm-SS que no me interesa, no lo guardo
        strcpy(ticket_aux.patente, strtok(NULL, delimiters));
        ticket_aux.agency = strtok(NULL, delimiters);
        ticket_aux.id = atoi(strtok(NULL, delimiters));
        strtok(NULL, delimiters); // No me interesa el monto
        insertTicket(ticket_aux, ticketsCHI);
    }
}