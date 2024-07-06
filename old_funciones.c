// Archivo donde guardamos las implementaciones viejas de ciertas funciones
#include "ticketsADT.h"
#include "funciones.c"

void loadInfractions(ticketsADT tickets, FILE *file_infr, char* delimiters, char buffer_line[]) {
    tInfraction infr_aux; // Estructura donde iré guardando los datos de cada infracción a medida que leo el archivo
    fgets(buffer_line, MAX_LINE_LENGTH, file_infr); // Sacamos los nombre de los campos de la primer línea
    while ( fgets(buffer_line, MAX_LINE_LENGTH, file_infr) != NULL ) {
        infr_aux.id = atoi(strtok(buffer_line, delimiters));
        infr_aux.description = strtok(NULL, delimiters);
        // Al mandar la descripcion, el CDT por atrás ya hace la copia. En el proximo bucle, se pisa
        // buffer_line pero no interesa, ya que en el cdt vos tenes la copia
        insertInfraction(infr_aux, tickets);
        if ( ERRNO == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            return ERROR;
        }
    }
}

// Especificamente para Chicago (Difiere el orden de los datos en el .csv con NYC)
void loadTicketsCHI(ticketsADT ticketsCHI, FILE *file_tickets, char* delimiters, char buffer_line[]) {
    tTicket ticket_aux; // Estructura donde iré guardando los datos de cada multa a medida que leo el archivo
    fgets(buffer_line, MAX_LINE_LENGTH, file_tickets); // Sacamos los nombre de los campos de la primer línea
    while ( fgets(buffer_line, MAX_LINE_LENGTH, file_tickets) != NULL ) {
        ticket_aux.year = atoi(strtok(buffer_line, '-')); // Guardo yyyy
        ticket_aux.month = (char)atoi(strtok(NULL, '-')); // Guardo MM
        strtok(NULL, delimiters); // Acá esta dd HH-mm-SS que no me interesa, no lo guardo
        strcpy(ticket_aux.patente, strtok(NULL, delimiters));
        ticket_aux.agency = strtok(NULL, delimiters);
        ticket_aux.id = atoi(strtok(NULL, delimiters));
        strtok(NULL, delimiters); // No me interesa el monto
        insertTicket(ticket_aux, ticketsCHI);
        if ( ERRNO == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de multas por falta de memoria.\n");
            return ERROR;
        }
    }
}

// Implementacion con fscanf
void loadTicketsCHI2(ticketsADT ticketsCHI, FILE *file_tickets) {
    char id_aux[MAX_LONG_INT];
    char year_aux[MAX_LONG_INT];
    char month_aux[MAX_LONG_INT];
    char agency_aux[MAX_AGENCY_CHI];
    tTicket ticket_aux;
    fscanf(file_tickets, "%*[^\n]\n"); // Sacamos los nombre de los campos de la primer línea
    while ( fscanf(file_tickets, "%[^-]-%[^-]-%*[^;];%[^;];%[^;];%[^;];%*[^\n]\n", year_aux, month_aux, ticket_aux.patente, agency_aux, id_aux) == TICKETS_FIELDS ) {
        ticket_aux.id = atoi(id_aux);
        ticket_aux.year = atoi(year_aux);
        ticket_aux.month = (char)atoi(month_aux);
        ticket_aux.agency = agency_aux;
        if ( insertTicket(ticket_aux, ticketsCHI) == -1 ) {
            fprintf(stderr, "Error al cargar datos del archivo de multas por falta de memoria.\n");
            return ERROR;
        }
    }
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

void loadTicketsNYC2(ticketsADT ticketsNYC, FILE *file_tickets){
    tTicket ticket_aux;
    fscanf(file_tickets,"%*[^\n]\n");

    char year_ascii[11]; //magic
    char month_ascii[11];
    char id_ascii[11];
    char agency_ascii[36];

    while ( fscanf(file_tickets, "%[^;];%[^-]-%[^-]-%*[^;];%[^;];%*[^;];[^\n]\n", ticket_aux.patente, year_ascii, month_ascii, id_ascii, agency_ascii) == TICKETS_FIELDS) {
        ticket_aux.id = atoi(id_ascii);
        ticket_aux.year = atoi(year_ascii);
        ticket_aux.month = (char) atoi(month_ascii);
        ticket_aux.agency = agency_ascii;
        insertTicket(ticket_aux, ticketsNYC);
    }
}