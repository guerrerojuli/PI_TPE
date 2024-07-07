#ifndef __ticketsADT__h
#define __ticketsADT__h

#include <stddef.h>

typedef struct ticketsCDT * ticketsADT;

typedef struct ticket {
  size_t id;
  char *agency;
  size_t year;
  char month;
  char plate[11];
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
  char top[3];
} tYear;

/* Crea una nueva estructura donde se van a guardar los tickets.
** Inicialmente esta vacia (no hay tickets).
** Si no se pudo crear retorna NULL.
*/
ticketsADT newTickets(size_t beginYear, size_t endYear,size_t descLength, size_t agencyLength, size_t longPlate);

/* Vincula el id con la descripcion de la infraccion. 
** Retorna 0 si el id ya estaba vinculado a una descripcion (no lo inserta).
** Setea errno si falla la asignacion de memoria y retorna -1.
** Retorna 1 si lo puedo vincular (lo inserta).
*/
int insertInfraction(tInfraction infraction, ticketsADT tickets);

/* Inserta un ticket
** Setea errno si falla
*/
void insertTicket(tTicket ticket, ticketsADT tickets);

/* Ordena la lista de infracciones en orden descendiente por cantidad de multas
** Setea current en first que se va a usar para iterar en la lista
*/
void toBeginByAmount(ticketsADT tickets);

/* Verifica si hay más infracciones en la lista ordenada por cantidad.
** Retorna 1 si hay más infracciones, 0 en caso contrario.
*/
int hasNextByAmount(ticketsADT tickets);

/* Retorna la siguiente infracción en la lista ordenada por cantidad.
** La estructura retornada incluye la descripción y la cantidad de multas.
*/
tInfractionByAmount nextByAmount(ticketsADT tickets);

/* Inicializa el iterador para recorrer las agencias.
** Setea currentAgency en la primera agencia de la lista.
*/
void toBeginAgency(ticketsADT tickets);

/* Verifica si hay más agencias en la lista.
** Retorna 1 si hay más agencias, 0 en caso contrario.
*/
int hasNextAgency(ticketsADT tickets);

/* Retorna la siguiente agencia en la lista.
** La estructura retornada incluye el nombre de la agencia, 
** la descripción de la infracción más común y la cantidad de dicha infracción.
*/
tAgency nextAgency(ticketsADT tickets);

/* Inicializa el iterador para recorrer las infracciones por orden alfabético.
** Setea currentByAlpha en la primera infracción de la lista.
*/
void toBeginPlateByAlpha(ticketsADT tickets);

/* Verifica si hay más infracciones en la lista ordenada alfabéticamente.
** Retorna 1 si hay más infracciones, 0 en caso contrario.
*/
int hasNextPlateByAlpha(ticketsADT tickets);

/* Retorna la siguiente infracción en la lista ordenada alfabéticamente.
** La estructura retornada incluye la descripción de la infracción, 
** la placa del vehículo y la cantidad de dicha infracción.
*/
tInfractionPlateByAlpha nextPlateByAlpha(ticketsADT tickets);

/* Devuelve un arreglo (de tYear) en orden creciente (por anio) que en cada posicion tiene el anio 
** con los tres meses con más multas (si menos de tres meses tienen multas devuelve 
** en lugar de un número de un mes el número cero).
** amountYears es un parámetro de entrada/salida donde se va a guardar la cantidad 
** de años para los cuales se registraron los tres meses con más multas.
*/
tYear * getTop3Month(ticketsADT tickets, size_t * amountYears);

/*Libera los recursos utilizados para registrar las infracciones*/
void freeTickets(ticketsADT tickets);

#endif