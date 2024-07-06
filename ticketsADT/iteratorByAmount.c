#include "ticketsADT.h"
#include "ticketsADT_internal.h"

static tInfractionNode *orderTicketsByAmountRec(tInfractionNode *infrNode, tInfractionNode *infraction) {
  if (infrNode == NULL) {
    return infraction;
  }

  if (infrNode->infractionAmount < infraction->infractionAmount) {
    infraction->nextByAmount = infrNode;
    return infraction;
  }

  infrNode->nextByAmount = orderTicketsByAmountRec(infrNode->nextByAmount, infraction);
  return infrNode;
}

static void orderTicketsByAmount(ticketsADT tickets) {
  for (int i = 0; i < tickets->infractionsDim; i++) {
    if (tickets->infractions[i].description != NULL) {
      tickets->firstByAmount = orderTicketsByAmountRec( tickets->firstByAmount, &tickets->infractions[i]);
    }
  }
}

void toBeginByAmount(ticketsADT tickets) {
  if (tickets->firstByAmount == NULL) {
    orderTicketsByAmount(tickets);
  }

  tickets->currentByAmount = tickets->firstByAmount;
}

int hasNextByAmount(ticketsADT tickets) {
  return tickets->currentByAmount != NULL;
}

tInfractionByAmount nextByAmount(ticketsADT tickets) {
  tInfractionByAmount infr;

  if (!hasNextByAmount(tickets)) {
    infr.description = NULL;
    infr.amount = 0;
    return infr;
  }

  tInfractionNode *current = tickets->currentByAmount;
  infr.description = current->description;
  infr.amount = current->infractionAmount;
  tickets->currentByAmount = current->nextByAmount;
  return infr;
}
