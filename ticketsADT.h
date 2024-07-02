#ifndef __ticketsADT__h
#define __ticketsADT__h

#include <stdlib.h>

typedef struct ticketsCDT * ticketsADT;

typedef struct ticket {
  size_t id;
  char *agency;
  size_t year;
  char month;
  char plate[10];
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

/* Crea una nueva estructura donde se van a guardar los tickets.
** Inicialmente esta vacia (no hay tickets).
** Si no se pudo crear retorna NULL.
** Almacena el rango de anios (beginYear y endYear inclusive) para los cuales se van a querer obtener los tres meses con mayores cantidades de multas (por anio).
** Si beginYear es 0, significa que quiere las tres multas mas populares de cada anio registrado. 
*/
ticketsADT newTickets(size_t beginYear, size_t endYear);

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
