#ifndef __ticketsADT__h
#define __ticketsADT__h

#include <stdlib.h>

typedef struct ticketsCDT *ticketsADT;

typedef struct ticket {
  size_t id;
  char *agency;
  size_t year;
  char month;
  char patente[10];
} tTicket;

typedef struct infraction {
  size_t id;
  char *description;
} tInfraction;

typedef struct infractionByAmount {
  char *description;
  size_t amount;
} tInfractionByAmount;

typedef struct agency {
  char *name;
  char *infractionDesc;
  size_t amount;
} tAgency;

typedef struct infractionPlateByAlpha {
  char *description;
  char *plate;
  size_t amount;
} tInfractionPlateByAlpha;

typedef struct year {
  size_t year;
  size_t top[3];
} tYear;




ticketsADT newTickets(size_t beginYear, size_t endYear);

int insertInfraction(tInfraction infraction, ticketsADT tickets);

int insertTicket(tTicket ticket, ticketsADT tickets);

void toBeginByAmount(ticketsADT tickets);

int hasNextByAmount(ticketsADT tickets);

tInfraction nextByAmount(ticketsADT tickets);

void toBeginAgeny(ticketsADT tickets);

int hasNextAgency(ticketsADT tickets);

tAgency nextAgency(ticketsADT tickets);

void toBeginPlateByAlpha(ticketsADT tickets);

int hasNextPlateByAlpha(ticketsADT tickets);

tInfractionPlateByAlpha nextPlateByAlpha(ticketsADT tickets);

tYear *getTop3Month(ticketsADT tickets);

void freeTickets(ticketsADT tickets);

#endif
