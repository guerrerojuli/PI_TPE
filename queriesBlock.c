#include "ticketsADT.h"
#include "funciones.c"

void query1(ticketsADT tickets) {
    FILE *query1 = createQueryFile("./query1.csv");
    fprintf(query1, "infraction;tickets\n"); // Primera linea
    toBeginByAmount(tickets);
    tInfractionByAmount aux;

    char buffer[MAX];
    int newBytes = 0, countBytes = 0;

    for (int i = 1; hasNextByAmount(tickets); i++){
        
        aux = nextByAmount(tickets);
        newBytes = snprintf(buffer + countBytes, MAX - countBytes, "%s;%d\n", aux.description, aux.amount);
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

    char buffer[MAX];
    int newBytes = 0, countBytes = 0;

    for (int i = 1; hasNextAgency(tickets); i++) {
        aux = nextByAgency(tickets);
        newBytes = snprintf(buffer + countBytes, MAX - countBytes, "%s;%s;%d\n", aux.name, aux.infractionDesc, aux.amount);
        countBytes += newBytes;
        if (i % LINEAS == 0){
            fwrite(buffer, 1, countBytes, query2);
            countBytes = 0;
        }
    }
    if (countBytes > 0) { // copio lo que me quedo en el buffer
        fwrite(buffer, 1, countBytes, query1);
    }
    fclose(query2);
}

void query3(ticketsADT tickets) {
    FILE *query3 = createQueryFile("./query3.csv");
    fprintf(query3, "infraction;plate;tickets\n");
    toBeginPlateByAlpha(tickets);
    tInfractionPlateByAlpha aux;

    char buffer[MAX];
    int newBytes = 0, countBytes = 0;
    
    for (int i = 1; hasNextPlateByAlpha(tickets); i++) {
        aux = nextPlateByAlpha(tickets);
        newBytes = snprintf(buffer + countBytes, MAX - countBytes, "%s;%s;%d\n", aux.description, aux.plate, aux.amount);
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
    
    char buffer[MAX];
    int newBytes = 0, countBytes = 0;

    for (int i = 0, j = 1; i < dim; i++, j++) {
        newBytes = snprintf(buffer + countBytes, MAX - countBytes,  "%d;%s;%s;%s\n", resp[i].year, months[resp[i].top[0]], months[resp[i].top[1]], months[resp[i].top[2]]);
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
