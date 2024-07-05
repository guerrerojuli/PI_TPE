#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include <string.h>

static tInfractionNode *orderTicketsByAlphaRec(tInfractionNode *infrNode, tInfractionNode *infraction) {
  if (infraction->description == NULL) {
    return infrNode;
  }

  if (infrNode == NULL) {
    return infraction;
  }

  if (strcmp(infrNode->description, infraction->description) > 0) {
    infraction->nextByAlpha = infrNode;
    return infraction;
  }

  infrNode->nextByAlpha = orderTicketsByAlphaRec(infrNode->nextByAlpha, infraction);
  return infrNode;
}

static void orderTicketsByAlpha (ticketsADT tickets){
    for (int i=0 ; i < tickets->infractionsDim ; i++){
        tickets->firstByAmount=orderTicketsByAlphaRec(tickets->firstByAmount, &tickets->infractions[i]);
    }
}

void toBeginPlateByAlpha(ticketsADT tickets) {
  if (tickets->firstByAlpha == NULL) {
    orderTicketsByAlpha(tickets);
  }
  tickets->currentByAlpha = tickets->firstByAlpha;
}

int hasNextPlateByAlpha(ticketsADT tickets) {
  return (tickets->currentByAlpha != NULL);
}

tInfractionPlateByAlpha nextPlateByAlpha(ticketsADT tickets) {
  tInfractionPlateByAlpha infr;
  if (!hasNextPlateByAlpha(tickets)) {
    infr.description = NULL;
    infr.plate = NULL;
    infr.amount = 0;
    return infr;
  }
  tInfractionNode *current = tickets->currentByAlpha;
  infr.description = current->description;
  infr.plate = current->maxPlate;
  infr.amount = current->plateAmount;
  tickets->currentByAlpha = current->nextByAlpha;
  return infr;
}
