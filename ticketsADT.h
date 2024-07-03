#ifndef __ticketsADT__h
#define __ticketsADT__h

#include <stddef.h>
#define LONG_PATENTE 11 // Cuento el 0 final

typedef struct ticketsCDT *ticketsADT;

typedef struct ticket {
  size_t id;
  char *agency;
  size_t year;
  char month;
  char patente[LONG_PATENTE];
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

ticketsADT newTickets(size_t beginYear, size_t endYear, size_t max_description, size_t max_agency_name);

/* Vincula el id con la descripcion de la infraccion. 
** Retorna 0 si el id ya estaba vinculado a una descripcion (no lo inserta).
** Retorna 0 si falla la asignacion de memoria.
** Retorna 1 si lo puedo vincular (lo inserta).
*/
int insertInfraction(tInfraction infraction, ticketsADT tickets);

/* Inserta un ticket. 
** Retorna 0 si el id de la infraccion no existe y no lo inserta.
** Retorna 1 si lo pudo insertar.
*/
int insertTicket(tTicket ticket, ticketsADT tickets);

void toBeginByAmount(ticketsADT tickets);

int hasNextByAmount(ticketsADT tickets);

tInfractionByAmount nextByAmount(ticketsADT tickets);

void toBeginByAgency(ticketsADT tickets);

int hasNextAgency(ticketsADT tickets);

tAgency nextByAgency(ticketsADT tickets);

void toBeginPlateByAlpha(ticketsADT tickets);

int hasNextPlateByAlpha(ticketsADT tickets);

tInfractionPlateByAlpha nextPlateByAlpha(ticketsADT tickets);

tYear *getTop3Month(ticketsADT tickets);

void freeTickets(ticketsADT tickets);

#endif