#ifndef __ticketsADT__h
#define __ticketsADT__h

#include <stddef.h>

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
*/
ticketsADT newTickets(size_t beginYear, size_t endYear,size_t descLength, size_t agencyLength, size_t longPlate);

/* Vincula el id con la descripcion de la infraccion. 
** Retorna 0 si el id ya estaba vinculado a una descripcion (no lo inserta).
** Retorna -1 si falla la asignacion de memoria.
** Retorna 1 si lo puedo vincular (lo inserta).
*/
int insertInfraction(tInfraction infraction, ticketsADT tickets);

/* Inserta un ticket. 
** Retorna 0 si el id de la infraccion no existe y no lo inserta.
** Retorna -1 si falla.
** Retorna 1 si lo pudo insertar.
*/
int insertTicket(tTicket ticket, ticketsADT tickets);

void toBeginByAmount(ticketsADT tickets);

int hasNextByAmount(ticketsADT tickets);

tInfractionByAmount nextByAmount(ticketsADT tickets);

void toBeginAgency(ticketsADT tickets);

int hasNextAgency(ticketsADT tickets);

tAgency nextAgency(ticketsADT tickets);

void toBeginPlateByAlpha(ticketsADT tickets);

int hasNextPlateByAlpha(ticketsADT tickets);

tInfractionPlateByAlpha nextPlateByAlpha(ticketsADT tickets);

/* Devuelve un arreglo (de tYear) en orden creciente que en cada posicion tiene el anio con los tres meses con mas multas (si menos de tres meses tienen multas devuelve en lugar de un numero de un mes el numero cero)
** amountYears es un parametro de entrada/salida donde se va a guardar la cantidad de anios para los cuales se van a registrar los tres meses con mas multas
*/
tYear * getTop3Month(ticketsADT tickets, size_t * amountYears);

void freeTickets(ticketsADT tickets);

#endif
