#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ticketsADT.h"
#define MAX_MONTHS 12 // podrian utilizarse los 6 primeros
#define MAX_LINE_LENGTH 256
#define START_YEAR 1900
#define SUCCESS 0
#define ERROR 1

// Archivo en el cual guardamos las funciones auxiliares utilizadas en el main de CHICAGO y NYC análogamente
FILE *openFile(char* arg) {
    FILE *res = fopen(arg, "r");
    if ( res == NULL ) {
        fprintf(stderr, "Error al abrir archivo: %s\n", arg);
        exit(ERROR);
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
    int beginYear, endYear;
    if ( argc == 3 ) { // Si no me pasan el año de cierre, obtengo el actual con la librería time.h
        tickets = newTickets(START_YEAR, year, max_description, max_agency_name);
    }
    else if ( argc == 4 && (beginYear = atoi(argv[3]) >= 0) ) {
        tickets = newTickets(beginYear, year, max_description, max_agency_name);
    }
    else {
        if ( beginYear > (endYear = atoi(argv[4])) || endYear < 0 || beginYear < 0 ) {
            fprintf(stderr, "Error en los años pasados como parametros\n");
            exit(ERROR);
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
        // Al mandar la descripcion, el CDT por atrás ya hace la copia. En el proximo bucle, se pisa
        // buffer_line pero no interesa, ya que en el cdt vos tenes la copia
        if ( insertInfraction(infr_aux, tickets) == -1 ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            return ERROR;
        }
    }
}

FILE *createQueryFile(char* name) {
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
    int dim;
    tYear *resp = getTop3Month(tickets, &dim);
    static char *months[MAX_MONTHS] = {"Empty", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    for (int i = 0; i < dim; i++) {
        fprintf(query4, "%d;%s;%s;%s\n", resp[i].year, months[resp[i].top[0]], months[resp[i].top[1]], months[resp[i].top[2]]);
    }
    fclose(query4);
    free(resp);
}