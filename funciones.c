#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "./ticketsADT.h"
#define MAX_LINE_LENGTH 256

// Archivo en el cual guardamos las funciones auxiliares utilizadas en el main de CHICAGO y NYC análogamente
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

void loadInfractions(ticketsADT tickets, FILE *file_infr, char* delimiters, char buffer_line[]) {
    tInfraction infr_aux; // Estructura donde iré guardando los datos de cada infracción a medida que leo el archivo
    fgets(buffer_line, MAX_LINE_LENGTH, file_infr); // Sacamos los nombre de los campos de la primer línea
    while ( fgets(buffer_line, MAX_LINE_LENGTH, file_infr) != NULL ) {
        infr_aux.id = atoi(strtok(buffer_line, delimiters));
        infr_aux.description = strtok(NULL, delimiters);
        insertInfraction(infr_aux, tickets);
        // Al mandar la descripcion, el CDT por atrás ya hace la copia. En el proximo bucle, se pisa
        // buffer_line pero no interesa, ya que en el cdt vos tenes la copia
    }
}

void query1(ticketsADT tickets) {
    FILE *query1 = fopen("./query1.csv", "w");
    if ( query1 == NULL ) {
        // Salida por error
    }
    fprintf(query1, "infraction;tickets\n"); // Primera linea
    toBeginByAmount(tickets);
    while ( hasNextByAmount(tickets) ) {
        tInfractionByAmount aux = nextByAmount(tickets);
        fprintf(query1, "%s;%d\n", aux.description, aux.amount); // Hay que chequear errores?
    }
    fclose(query1);
}

void query2(ticketsADT tickets) {
    FILE *query2 = fopen("./query2.csv", "w");
    if ( query2 == NULL ) {
        // Salida por error
    }
    fprintf(query2, "issuingAgency;infraction;tickets\n");
    toBeginByAgency(tickets);
    while ( hasNextByAmount(tickets) ) {
        tAgency aux = nextByAgency(tickets);
        fprintf(query2, "%s;%s;%d\n", aux.name, aux.infractionDesc, aux.amount);
    }
    fclose(query2);
}

void query3(ticketsADT tickets) {
    FILE *query3 = fopen("./query3.csv", "w");
    if ( query3 == NULL ) {
        // Salida por error
    }
    fprintf(query3, "infraction;plate;tickets\n");
    toBeginPlateByAlpha(tickets);
    while ( hasNextPlateByAlpha(tickets) ) {
        tInfractionPlateByAlpha aux = nextPlateByAlpha(tickets);
        fprintf(query3, "%s;%s;%d\n", aux.description, aux.plate, aux.amount);
    }
    fclose(query3);
}

void query4(ticketsADT tickets) {
    FILE *query4 = fopen("./query4.csv", "w");
    if ( query4 == NULL ) {
        // Salida por error
    }
    fprintf(query4, "year;ticketsTop1Month;ticketsTop2Month;ticketsTop3Month\n");
    // NO ESTÁ TERMINADA
}