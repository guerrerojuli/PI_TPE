#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include "plateTree.h"
#include "utils.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "insertTicket.c"
#include "iteratorByAmount.c"
#include "agencyIterator.c"
#include "iteratorPlateByAlpha.c"

/* Para un anio coloca los valores de los tres meses con mayor cantidad de multas */
static void getMonths(size_t * year , char* firstMonth, char* secondMonth, char* thirdMonth);

/* Libera todos los recursos utilizados por la lista de agencias */
static void freeAgencies(tAgencyList agencies);

/* Libera todos los recursos utilizados por el arreglo de infracciones */
static void freeInfraction(tInfractionNode * infrNode);


ticketsADT newTickets(size_t beginYear, size_t endYear, size_t descLength, size_t agencyLength, size_t plateLength) {
  errno = 0;
  ticketsADT tickets = calloc(1, sizeof(*tickets));
  if (tickets == NULL || errno == ENOMEM) {
    return NULL;
  }

  errno = 0;
  tickets->years = calloc((endYear - beginYear + 1), sizeof(*tickets->years));
  if (tickets->years == NULL || errno == ENOMEM) {
    free(tickets);
    return NULL;
  }

  tickets->beginYear = beginYear;
  tickets->endYear = endYear;
  tickets->descLength = descLength;
  tickets->plateLength = plateLength;
  tickets->agencyLength = agencyLength;
  return tickets;
}

int insertInfraction(tInfraction infraction, ticketsADT tickets) {
  if (tickets->infractionsDim <= infraction.id) {
    errno = 0;
    tInfractionNode * aux=tickets->infractions;
    tickets->infractions = realloc(tickets->infractions, (infraction.id + 1) * sizeof(tickets->infractions[0]));
    if (tickets->infractions == NULL || errno == ENOMEM) {
      tickets->infractions=aux;
      errno=ENOMEM;
      return -1;
    }

    for (int i = tickets->infractionsDim; i <= infraction.id; i++) {
      tickets->infractions[i].description = NULL;
    }

    tickets->infractionsDim = infraction.id + 1;
  }

  /*Si el id ya tiene vinculado una descripcion para*/
  if (tickets->infractions[infraction.id].description != NULL) {
    return 0;
  }

  /*como ya se que el id no tiene vinculado una descripcion, agrego la
   * descripcion y seteo todo en NUll o 0, retorno 1*/
  tInfractionNode *ticket = &tickets->infractions[infraction.id];
  ticket->description = copyOfStr(infraction.description, tickets->descLength);
  if (ticket->description == NULL) {
    return -1;
  }
  ticket->infractionAmount = 0;
  ticket->maxPlate = NULL;
  ticket->maxPlateAmount = 0;
  ticket->nextByAlpha = NULL;
  ticket->nextByAmount = NULL;
  for (int i = 0; i < TREE_VEC_DIM; i++) {
    ticket->plateTree[i] = NULL;
  }
  return 1;
}

tYear * getTop3Month(ticketsADT tickets, size_t * amountYears){
  int j = 0, yearsRange = tickets->endYear - tickets->beginYear + 1;

  errno = 0;
  tYear *arr = malloc(yearsRange * sizeof(*arr)); 
  if (arr == NULL || errno ==  ENOMEM){
    return NULL;
  }

  for (int i = 0; i < yearsRange ; i++){
    getMonths(tickets->years[i], (arr[j].top + 0), (arr[j].top + 1), (arr[j].top + 2));
    if (j != 0 || arr[j].top[0] != 0) {
      arr[j++].year = i + tickets->beginYear;
    }

  }

  tYear * aux=arr;
  arr=realloc(arr, j * sizeof(*arr));
  if (arr == NULL || errno ==  ENOMEM){
    arr=aux;
    *amountYears=0;
    return NULL;
  }
  *amountYears = j;

  return arr;
}

static void getMonths(size_t * year , char* firstMonth, char* secondMonth, char* thirdMonth){
  size_t firstAmount = 0, secondAmount = 0, thirdAmount = 0;
  *firstMonth = *secondMonth = *thirdMonth = 0;
  for (int i=0 ; i < N_MONTH ; i++){
    if (year[i] > firstAmount){
      thirdAmount = secondAmount;
      *thirdMonth = *secondMonth;
      secondAmount = firstAmount;
      *secondMonth = *firstMonth;
      firstAmount = year[i];
      *firstMonth = i + 1;
    }
    else if (year[i] > secondAmount){
      thirdAmount = secondAmount;
      *thirdMonth = *secondMonth;
      secondAmount = year[i];
      *secondMonth = i + 1;
    }
    else if (year[i] > thirdAmount){
      thirdAmount = year[i];
      *thirdMonth= i + 1;
    }
  }
}

void freeTickets(ticketsADT tickets){
  if (tickets == NULL) {
    return;
  }
  free(tickets->years);

  for (int i=0 ; i < tickets->infractionsDim ; i++){
    freeInfraction(&tickets->infractions[i]);
  }

  free(tickets->infractions);

  freeAgencies(tickets->agencies);

  free(tickets);
}

static void freeAgencies(tAgencyList agencies) {
  if (agencies == NULL){
    return;
  }

  freeAgencies(agencies->nextAgency);

  free(agencies->inf);
  free(agencies->name);
  free(agencies);
}

static void freeInfraction(tInfractionNode * infrNode) {
  if (infrNode->description == NULL){
    return; 
  }

  free(infrNode->description);
  for (int i = 0; i < TREE_VEC_DIM; i++) {
    freeTree(infrNode->plateTree[i]);
  }
}

