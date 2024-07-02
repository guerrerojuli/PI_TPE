#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ticketsADT.h"
#define MAX_LINE_LENGTH 256 // Preguntar si es necesario que sea exactamente la longitud de la fila de tickets

int main(int argc, char *argv[]) {
    if ( argc < 3 || argc > 5 ) {
        // Salida por error de parámetros. Debo chequear potenciales archivos que rompar el programa
    }

    // Chequeo los argumentos para crear mi ADT
    ticketsADT ticketsNYC = createTicketADT(argc, argv);

    // Primera parte: Leo argv[2], el cual será el archivo csv donde guardo infracciones con su respectivo id
    // Formarto: id;descripcion
    FILE *file_infr = fopen(argv[2], "r");
    if ( file_infr == NULL ) { 
        // Salida por error, no hay archivo
        perror("Error al abrir el archivo");
        return 1;
    }
    
    tInfraction infr_aux;// Estructura donde iré guardando los datos de cada infracción a medida que leo el archivo
    char aux_line[MAX_LINE_LENGTH];
    char* delimiters = ";\n"; // Delimitadores para strtok
    while ( fgets(aux_line, sizeof(aux_line), file_infr) != NULL ) {
        char *token = strtok(aux_line, delimiters);
        infr_aux.id = atoi(token);
        infr_aux.description = strtok(NULL, delimiters);
        insertInfraction(infr_aux, ticketsNYC, MAX_LINE_LENGTH);
    }
    // le pasamos al cdt el puntero para que haga la copia
    fclose(file_infr);
}

// falta modularizar, se puede hacer una generica para las dos ciudades como el createTicketAdt
// loadTicket
// podriamos tener un campo en el cdt en vez de pasarle todo el tiempo el maxlinelength

ticketsADT createTicketAdt(int argc, char *argv[]) {
    time_t now;
    struct tm *local;
    time(&now);
    local = localtime(&now);
    int year = local->tm_year + 1900;
    ticketsADT tickets;
    if ( argc == 3 ) { // Si no me pasan el año de cierre, obtengo el actual con la librería time.h
        tickets = newTickets(0, year);
    }
    else if ( argc == 4 ) {
        tickets = newTickets(atoi(argv[3]), year);
    }
    else {
        int beginYear = atoi(argv[3]);
        int endYear = atoi(argv[4]);
        if ( beginYear > endYear ) {
            // Salida por error de los parámetros.
        }
        tickets = newTickets(beginYear, endYear);
    }
    return tickets;
}