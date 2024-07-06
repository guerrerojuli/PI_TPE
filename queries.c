#include <stdio.h>
#include <stdlib.h>
#include "ticketsADT.h"
#define LINEAS 50 // manejo #años #agencias #infracciones. Copio de a 50 lineas al archivo.
#define MAX_LINE_QUERY4 34 //(4+1+9+1+9+1+9)
#define MAX_MONTHS 13 // 12 meses + 1 empty

#ifdef CHI
        #define MAX_LINE_QUERY1 61 //(50+1+10)
        #define MAX_LINE_QUERY2 75 //(13+1+50+1+10)
        #define MAX_LINE_QUERY3 72 //(50+1+10+1+10)
#elif NYC
        #define MAX_LINE_QUERY1 41 //(30+1+10)
        #define MAX_LINE_QUERY2 77 //(35+1+30+1+10)
        #define MAX_LINE_QUERY3 52 //(30+1+10+1+10)
#endif

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
    tInfractionByAmount aux;

    int max = LINEAS * MAX_LINE_QUERY1;
    char buffer[max];
    int newBytes = 0, countBytes = 0;

    for (int i = 1; hasNextByAmount(tickets); i++){
        
        aux = nextByAmount(tickets);
        newBytes = snprintf(buffer + countBytes, max - countBytes, "%s;%ld\n", aux.description, aux.amount);
        countBytes += newBytes;
        if (i % LINEAS == 0){
            fwrite(buffer, 1, countBytes, query1);
            countBytes = 0;
        }
    }

    if (countBytes > 0) { // copio lo que me quedo en el buffer
        fwrite(buffer, 1, countBytes, query1);
    }

    fclose(query1);
}


void query2(ticketsADT tickets) {
    FILE *query2 = createQueryFile("./query2.csv");
    fprintf(query2, "issuingAgency;infraction;tickets\n");
    toBeginByAgency(tickets);
    tAgency aux;

    int max = LINEAS * MAX_LINE_QUERY2;
    char buffer[max];
    int newBytes = 0, countBytes = 0;

    for (int i = 1; hasNextAgency(tickets); i++) {
        aux = nextByAgency(tickets);
        newBytes = snprintf(buffer + countBytes, max - countBytes, "%s;%s;%ld\n", aux.name, aux.infractionDesc, aux.amount);
        countBytes += newBytes;
        if (i % LINEAS == 0){
            fwrite(buffer, 1, countBytes, query2);
            countBytes = 0;
        }
    }
    if (countBytes > 0) { // copio lo que me quedo en el buffer
        fwrite(buffer, 1, countBytes, query2);
    }
    fclose(query2);
}

void query3(ticketsADT tickets) {
    FILE *query3 = createQueryFile("./query3.csv");
    fprintf(query3, "infraction;plate;tickets\n");
    toBeginPlateByAlpha(tickets);
    tInfractionPlateByAlpha aux;

    int max = LINEAS * MAX_LINE_QUERY3;
    char buffer[max];
    int newBytes = 0, countBytes = 0;
    
    for (int i = 1; hasNextPlateByAlpha(tickets); i++) {
        aux = nextPlateByAlpha(tickets);
        newBytes = snprintf(buffer + countBytes, max - countBytes, "%s;%s;%ld\n", aux.description, aux.plate, aux.amount);
        countBytes += newBytes;
        if (i % LINEAS == 0){
            fwrite(buffer, 1, countBytes, query3);
            countBytes = 0;
        }
    }
    if (countBytes > 0) { // copio lo que me quedo en el buffer
        fwrite(buffer, 1, countBytes, query3);
    }
    fclose(query3);
}

void query4(ticketsADT tickets) {
    FILE *query4 = createQueryFile("./query4.csv");
    fprintf(query4, "year;ticketsTop1Month;ticketsTop2Month;ticketsTop3Month\n");
    int dim;
    tYear *resp = getTop3Month(tickets, &dim);
    static char *months[MAX_MONTHS] = {"Empty", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    
    int max = LINEAS * MAX_LINE_QUERY4;
    char buffer[max];
    int newBytes = 0, countBytes = 0;

    for (int i = 0, j = 1; i < dim; i++, j++) {
        newBytes = snprintf(buffer + countBytes, max - countBytes,  "%ld;%s;%s;%s\n", resp[i].year, months[resp[i].top[0]], months[resp[i].top[1]], months[resp[i].top[2]]);
        countBytes += newBytes;
        if (j % LINEAS == 0){
            fwrite(buffer, 1, countBytes, query4);
            countBytes = 0;
        }
    }
    if (countBytes > 0) { // copio lo que me quedo en el buffer
        fwrite(buffer, 1, countBytes, query4);
    }
    fclose(query4);
    free(resp);
}