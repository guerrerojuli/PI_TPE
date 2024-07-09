#include <stdio.h>
#include <stdlib.h>
#include "ticketsADT/ticketsADT.h"
#include "main.h"

int query1(ticketsADT tickets) {
    FILE *query1 = fopen("./query1.csv", "w");
    if (query1 == NULL) {
      return 0;
    }
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
    return 1;
}

int query2(ticketsADT tickets) {
    FILE *query2 = fopen("./query2.csv", "w");
    if (query2 == NULL) {
      return 0;
    }
    fprintf(query2, "issuingAgency;infraction;tickets\n");
    toBeginAgency(tickets);
    tAgency aux;

    int max = LINEAS * MAX_LINE_QUERY2;
    char buffer[max];
    int newBytes = 0, countBytes = 0;

    for (int i = 1; hasNextAgency(tickets); i++) {
        aux = nextAgency(tickets);
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
    return 1;
}

int query3(ticketsADT tickets) {
    FILE *query3 = fopen("./query3.csv", "w");
    if (query3 == NULL) {
      return 0;
    }
    fprintf(query3, "infraction;plate;tickets\n");
    toBeginPlateByAlpha(tickets);
    tInfractionPlateByAlpha aux;

    int max = LINEAS * MAX_LINE_QUERY3;
    char buffer[max];
    int newBytes = 0, countBytes = 0;
    
    for (int i = 1; hasNextPlateByAlpha(tickets); i++) {
        aux = nextPlateByAlpha(tickets);
        if (aux.amount != 0) {
          newBytes = snprintf(buffer + countBytes, max - countBytes, "%s;%s;%ld\n", aux.description, aux.plate, aux.amount);
          countBytes += newBytes;
          if (i % LINEAS == 0){
              fwrite(buffer, 1, countBytes, query3);
              countBytes = 0;
          }
        }
    }
    if (countBytes > 0) { // copio lo que me quedo en el buffer
        fwrite(buffer, 1, countBytes, query3);
    }
    fclose(query3);
    return 1;
}

int query4(ticketsADT tickets) {
    FILE *query4 = fopen("./query4.csv", "w");
    if (query4 == NULL) {
      return 0;
    }
    fprintf(query4, "year;ticketsTop1Month;ticketsTop2Month;ticketsTop3Month\n");
    size_t dim;
    tYear *resp = getTop3Month(tickets, &dim);
    static char *months[MAX_MONTHS] = {"Empty", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    
    int max = LINEAS * MAX_LINE_QUERY4;
    char buffer[max];
    int newBytes = 0, countBytes = 0;

    for (int i = 0, j = 1; i < dim; i++, j++) {
        newBytes = snprintf(buffer + countBytes, max - countBytes,  "%ld;%s;%s;%s\n", resp[i].year, months[(int) resp[i].top[0]], months[(int) resp[i].top[1]], months[(int) resp[i].top[2]]);
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
    return 1;
}
