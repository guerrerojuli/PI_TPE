#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include "ticketsADT.h"
#define MAX_MONTHS 12 // podrian utilizarse los 6 primeros
#define MAX_LINE_LENGTH 256
#define START_YEAR 1900
#define SUCCESS 0
#define ERROR 1
#define ERRNO 0
#define MAX_INFRACTION_NYC 31 // cuento el 0
#define MAX_AGENCY_NYC 36 // cuento el 0
#define TICKETS_FIELDS 5
#define MAX_LONG_INT 11 // Maxima cantidad de caracteres de un int y el cero final
#define LONG_PATENTE 11
#define MAX_INFRACTION_CHI 51 // cuento el 0
#define MAX_AGENCY_CHI 14 // cuento el 0
#define BUFFER_SIZE 65536 // 2^16

// Archivo en el cual guardamos las funciones auxiliares utilizadas en el main de CHICAGO y NYC análogamente
FILE *openFile(char* arg) {
    FILE *res = fopen(arg, "r");
    if ( res == NULL ) {
        fprintf(stderr, "Error al abrir archivo: %s\n", arg);
        exit(ERROR);
    }
    return res;
}

static int notOnlyNumbers(char *s) {
    while ( *s != '\0' ) {
        if ( !isdigit(*s) ) {
            return 1;
        }
        s++;
    }
    return 0;
}

ticketsADT createTicketADT(int argc, char *argv[], size_t max_description, size_t max_agency_name, size_t long_patente) {
    time_t now;
    struct tm *local;
    time(&now);
    local = localtime(&now);
    int year = local->tm_year + 1900;
    ticketsADT tickets;
    int beginYear, endYear;
    if ( argc == 3 ) { // Si no me pasan el año de cierre, obtengo el actual con la librería time.h
        tickets = newTickets(START_YEAR, year, max_description, max_agency_name, long_patente);
    }
    else if ( argc == 4 && (beginYear = atoi(argv[3])) >= 0 ) {
        if ( notOnlyNumbers(argv[3]) ) {
            fprintf(stderr, "Error en los años pasados como parametros\n");
            exit(ERROR);
        }
        if ( beginYear > year ) {
            beginYear = year + 1;
            tickets = newTickets(beginYear, beginYear, max_description, max_agency_name, long_patente);
        }
        else {
            tickets = newTickets(beginYear, year, max_description, max_agency_name, long_patente);
        }
    }
    else { // argc == 5
        if ( notOnlyNumbers(argv[4]) || notOnlyNumbers(argv[3]) || (beginYear = atoi(argv[3])) > (endYear = atoi(argv[4])) || endYear < 0 || beginYear < 0 ) { 
            // Chequea que ambos parametros sean solo numeros, y que beginYear sea menor o igual a endYear
            fprintf(stderr, "Error en los años pasados como parametros\n");
            exit(ERROR);
        }
        if ( beginYear > year ) {
            beginYear = year + 1;
            tickets = newTickets(beginYear, beginYear, max_description, max_agency_name, long_patente);
        }
        else {
            if ( endYear > year ) {
                endYear = year + 1;
            }
            tickets = newTickets(beginYear, endYear, max_description, max_agency_name, long_patente);
        }       
    }

    return tickets;
}

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

void processBufferInfractions(char buffer[], ticketsADT tickets) {
    tInfraction infr_aux; // Estructura donde iré guardando los datos de cada infracción
    char id_aux[MAX_LONG_INT];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^;];%[^\n]\n", id_aux, infr_aux.description);
        infr_aux.id = atoi(id_aux);
        insertInfraction(infr_aux, tickets);
        if ( ERRNO == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            return ERROR;
        }
        token = strtok(NULL, "\n");
    }
}

void processBufferTicketsCHI(char buffer[], ticketsADT tickets) {
    tTicket ticket_aux;
    char id_aux[MAX_LONG_INT];
    char year_aux[MAX_LONG_INT];
    char month_aux[MAX_LONG_INT];
    char agency_aux[MAX_AGENCY_CHI];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^-]-%[^-]-%*[^;];%[^;];%[^;];%[^;];%*[^\n]\n", year_aux, month_aux, ticket_aux.patente, agency_aux, id_aux);
        ticket_aux.id = atoi(id_aux);
        ticket_aux.year = atoi(year_aux);
        ticket_aux.month = (char)atoi(month_aux);
        ticket_aux.agency = agency_aux;
        insertTicket(ticket_aux, tickets);
        if ( ERRNO == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            return ERROR;
        }
        token = strtok(NULL, "\n");
    }
}

void processBufferTicketsNYC(char buffer[], ticketsADT tickets) {
    tTicket ticket_aux;
    char year_aux[MAX_LONG_INT];
    char month_aux[MAX_LONG_INT];
    char id_aux[MAX_LONG_INT];
    char agency_aux[MAX_INFRACTION_NYC];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^;];%[^-]-%[^-]-%*[^;];%[^;];%*[^;];[^\n]\n", ticket_aux.patente, year_aux, month_aux, id_aux, agency_aux);
        ticket_aux.id = atoi(id_aux);
        ticket_aux.year = atoi(year_aux);
        ticket_aux.month = (char) atoi(month_aux);
        ticket_aux.agency = agency_aux;
        insertTicket(ticket_aux, tickets);
        if ( ERRNO == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            return ERROR;
        }
        token = strtok(NULL, "\n");
    }
}



// Función optimizada, en vez de leer línea por línea, traigo de a chunks de memoria del archivo ya que es 
// muy costoso la lectura de archivos desde el disco duro. Proceso el bloque en una funcion auxiliar
void loadInfractionsWithBlocks(ticketsADT tickets, FILE *file_infr) {
    fscanf(file_infr, "%*[^\n]\n"); // Sacamos los nombre de los campos de la primer línea
    size_t bytesRead; // Cantidad de bytes leidos por fread, si es menor al pedido significa que llegue al final del archivo
    char buffer[BUFFER_SIZE + 1]; // Iré guardando el boque traido del archivo acá
    size_t temp_length = 0; // Longitud restante luego de truncar el bloque hasta el ultimo \n
    char *buffer_pointer = buffer; // Puntero desde el cual guardará el bloque fread
    while ( (bytesRead = fread(buffer_pointer, 1, BUFFER_SIZE - temp_length, file_infr)) > 0 ) {
        if ( bytesRead == BUFFER_SIZE - temp_length ) { // Chequeo no estar al final del archivo
            buffer[bytesRead + temp_length] = '\0'; // Le pongo el 0 final
            char* lastLine = strrchr(buffer, '\n'); // Será el puntero al ultimo caracter de mi buffer, elimino la linea
            size_t buffer_length = lastLine - buffer;
            buffer[buffer_length] = '\0'; // Piso el \n por \0, strtok me reconoce el final
            processBufferInfractions(buffer, tickets);
            buffer_pointer = buffer + buffer_length + 1;
            strcpy(buffer, buffer_pointer);
            temp_length = strlen(buffer);
            buffer_pointer = buffer + temp_length;
        }
        else { // Si estoy al final del archivo, agrego un \n al final de la linea final y pongo el 0 final
            buffer[bytesRead + temp_length] = '\n';
            buffer[bytesRead + temp_length + 1] = '\0';
            processBufferInfractions(buffer, tickets);
        }
    }
}

static FILE *createQueryFile(char* name) {
    FILE *res = fopen(name, "w");
    if ( res == NULL ) {
        fprintf(stderr, "Error al crear el archivo: %s\nPuede provenir por falta de memoria.\n", name);
        exit(1);
    }
    return res;
}

void query1(ticketsADT tickets) {
    FILE *query1 = createQueryFile("./query1.csv");
    fprintf(query1, "infraction;tickets\n"); // Primera linea
    toBeginByAmount(tickets);
    while ( hasNextByAmount(tickets) ) {
        tInfractionByAmount aux = nextByAmount(tickets);
        fprintf(query1, "%s;%d\n", aux.description, aux.amount);
    }
    fclose(query1);
}

void query2(ticketsADT tickets) {
    FILE *query2 = createQueryFile("./query2.csv");
    fprintf(query2, "issuingAgency;infraction;tickets\n");
    toBeginByAgency(tickets);
    while ( hasNextByAmount(tickets) ) {
        tAgency aux = nextByAgency(tickets);
        fprintf(query2, "%s;%s;%d\n", aux.name, aux.infractionDesc, aux.amount);
    }
    fclose(query2);
}

void query3(ticketsADT tickets) {
    FILE *query3 = createQueryFile("./query3.csv");
    fprintf(query3, "infraction;plate;tickets\n");
    toBeginPlateByAlpha(tickets);
    while ( hasNextPlateByAlpha(tickets) ) {
        tInfractionPlateByAlpha aux = nextPlateByAlpha(tickets);
        fprintf(query3, "%s;%s;%d\n", aux.description, aux.plate, aux.amount);
    }
    fclose(query3);
}

void query4(ticketsADT tickets) {
    FILE *query4 = createQueryFile("./query4.csv");
    fprintf(query4, "year;ticketsTop1Month;ticketsTop2Month;ticketsTop3Month\n");
    int dim = getYears(tickets);
    tYear *resp = getTop3Month(tickets);
    static char *months[MAX_MONTHS] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    for (int i = 0; i < dim; i++) {
        fprintf(query4, "%d;%s;%s;%s\n", resp[i].year, months[resp[i].top[0]], months[resp[i].top[1]], months[resp[i].top[2]]);
    }
    fclose(query4);
    free(resp);
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