#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include "ticketsADT/ticketsADT.h"
#include "main.h"

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

void processBufferInfractions(char buffer[], ticketsADT tickets) {
    tInfraction infr_aux; // Estructura donde iré guardando los datos de cada infracción
    char descAux[51];
    char id_aux[MAX_LONG_INT];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^;];%[^\n]\n", id_aux, descAux);
        infr_aux.id = atoi(id_aux);
        infr_aux.description = descAux;
        errno = 0;
        insertInfraction(infr_aux, tickets);
        if ( errno == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            exit(ERROR);
        }
        token = strtok(NULL, "\n");
    }
}

void processBufferTicketsCHI(char buffer[], ticketsADT tickets) {
    tTicket ticket_aux;
    char id_aux[MAX_LONG_INT];
    char year_aux[MAX_LONG_INT];
    char month_aux[MAX_LONG_INT];
    char agency_aux[MAX_AGENCY_CHI + 1];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^-]-%[^-]-%*[^;];%[^;];%[^;];%[^;];%*[^\n]\n", year_aux, month_aux, ticket_aux.plate, agency_aux, id_aux);
        ticket_aux.id = atoi(id_aux);
        ticket_aux.year = atoi(year_aux);
        ticket_aux.month = (char)atoi(month_aux);
        ticket_aux.agency = agency_aux;
        errno = 0;
        insertTicket(ticket_aux, tickets);
        if ( errno == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de tickets por falta de memoria.\n");
            exit(ERROR);
        }
        token = strtok(NULL, "\n");
    }
}

void processBufferTicketsNYC(char buffer[], ticketsADT tickets) {
    tTicket ticket_aux;
    char year_aux[MAX_LONG_INT];
    char month_aux[MAX_LONG_INT];
    char id_aux[MAX_LONG_INT];
    char agency_aux[MAX_AGENCY_NYC + 1];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^;];%[^-]-%[^-]-%*[^;];%[^;];%*[^;];%[^\n]\n", ticket_aux.plate, year_aux, month_aux, id_aux, agency_aux);
        ticket_aux.id = atoi(id_aux);
        ticket_aux.year = atoi(year_aux);
        ticket_aux.month = (char) atoi(month_aux);
        ticket_aux.agency = agency_aux;
        errno = 0;
        insertTicket(ticket_aux, tickets);
        if ( errno == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de tickets por falta de memoria.\n");
            exit(ERROR);
        }
        token = strtok(NULL, "\n");
    }
}

// Función optimizada, en vez de leer línea por línea, traigo de a chunks de memoria del archivo ya que es 
// muy costoso la lectura de archivos desde el disco duro. Proceso el bloque en una funcion auxiliar.
// La funcion es analoga para infracciones y tickets, por eso le pasamos un puntero a funcion
void loadWithBlocks(ticketsADT tickets, FILE *file_infr, void (*fn)(char[], ticketsADT) ) {
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
            fn(buffer, tickets);
            buffer_pointer = buffer + buffer_length + 1;
            strcpy(buffer, buffer_pointer);
            temp_length = strlen(buffer);
            buffer_pointer = buffer + temp_length;
        }
        else { // Si estoy al final del archivo, agrego un \n al final de la linea final y pongo el 0 final
            buffer[bytesRead + temp_length] = '\n';
            buffer[bytesRead + temp_length + 1] = '\0';
            fn(buffer, tickets);
        }
    }
}
