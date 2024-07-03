#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include "utils.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "insertTicket.c"

ticketsADT newTickets(size_t beginYear, size_t endYear, size_t descLength, size_t agencyLength) {
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
  ticket->description = copyString(infraction.description, tickets->descLength);
  if (ticket->description == NULL) {
    return -1;
  }
  ticket->infractionAmount = 0;
  ticket->nextByAlpha = NULL;
  ticket->nextByAmount = NULL;
  for (int i = 0; i < N_LETTER + N_NUMS; i++) {
    ticket->plateList[i] = NULL;
  }
  return 1;
}

