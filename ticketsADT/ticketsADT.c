#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include "utils.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "insertTicket.c"
#include "iteratorByAmount.c"


//FALTA AGREGAR EL LONGPLATE AL CDT
ticketsADT newTickets(size_t beginYear, size_t endYear, size_t descLength, size_t agencyLength, size_t plateLength) {
  errno = 0;
  ticketsADT tickets = calloc(1, sizeof(*tickets));
  if (tickets == NULL || errno == ENOMEM) {
    return NULL;
  }

  errno = 0;
  tickets->years = calloc((endYear - beginYear + 1), sizeof(*tickets->years));
  if (tickets->years == NULL || errno == ENOMEM) {
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
    tickets->infractions = realloc(tickets->infractions, (infraction.id + 1) * sizeof(tickets->infractions[0]));
    if (tickets->infractions == NULL || errno == ENOMEM) {
      return -1;
    }

    for (int i = tickets->infractionsDim; i <= infraction.id; i++) {
      tickets->infractions[i].description = NULL;
    }

    tickets->infractionsDim = infraction.id + 1;
  }

  /*Si el id ya tiene vinculado una descripcion retorno 0*/
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
  ticket->nextByAlpha = NULL;
  ticket->nextByAmount = NULL;
  ticket->plateTree = NULL;
  return 1;
}

typedef struct month {
  char month;
  size_t amount;
} month;


static void getTop3MonthAux(size_t * year , month * first, month * second, month * third){

  for (int i=0 ; i < N_MONTH ; i++){

    if (year[i] > first->amount){
      *third=*second;
      *second=*first;
      first->amount=year[i];
      first->month=i+1;
    }
    else if (year[i] > second->amount){
      *third=*second;
      second->amount=year[i];
      second->month=i+1;
    }
    else if (year[i] > third->amount){
      third->amount=year[i];
      third->month=i+1;
    }

  }

}

tYear * getTop3Month(ticketsADT tickets, size_t * amountYears){
  month first={0,0}, second={0,0}, third={0,0}, reset={0,0};

  int yearsRange=tickets->endYear - tickets->beginYear + 1, i=0, j=0;

  errno=0;
  tYear * arr=malloc(yearsRange * sizeof(*arr));
  if (arr == NULL || errno ==  ENOMEM){
    return NULL;
  }

  for (; i < yearsRange ; i++){

    getTop3MonthAux(tickets->years[i], &first, &second, &third);

    if (first.month || second.month || third.month){
      arr[j].year=i + tickets->beginYear;
      arr[j].top[0]=first.month;
      arr[j].top[1]=second.month;
      arr[j++].top[2]=third.month;
      first=second=third=reset;
    }
  }

  arr=realloc(arr, j * sizeof(*arr));
  if (arr == NULL || errno ==  ENOMEM){
    //ACA NOSE SI DEBERIA AGREGAR UN FREE(ARR)
    return NULL;
  }

  return arr;
}
