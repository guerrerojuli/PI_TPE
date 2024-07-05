#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static tInfractionNode * orderTicketsByAmountRec(tInfractionNode *infrNode, tInfractionNode *infraction){
    if (infraction->description == NULL){
        return infrNode;
    }

    if (infrNode == NULL){
        return infraction;
    }

    if (infrNode->infractionAmount < infraction->infractionAmount){
        infraction->nextByAmount=infrNode;
        return infraction;
    }

    infrNode->nextByAmount=orderTicketsByAmountRec(infrNode->nextByAmount,infraction);
    return infrNode;
}

static void orderTicketsByAmount (ticketsADT tickets){
    for (int i=0 ; i < tickets->infractionsDim ; i++){
        tickets->firstByAmount=orderTicketsByAmountRec(tickets->firstByAmount,&tickets->infractions[i]);
    }
}

void toBeginByAmount(ticketsADT tickets) {
    if (tickets->firstByAmount == NULL){
        orderTicketsByAmount(tickets);
    }

    tickets->currentByAmount = tickets->firstByAmount;
}

int hasNextByAmount(ticketsADT tickets) {
  return tickets->currentByAmount != NULL;
}

tInfractionByAmount nextByAmount(ticketsADT tickets) {
  if (!hasNextByAmount(tickets)) {
    exit(1);
  }

  tInfractionByAmount infr;
  infr.description = tickets->currentByAmount->description;
  infr.amount = tickets->currentByAmount->infractionAmount;
  tickets->currentByAmount = tickets->currentByAmount->nextByAmount;
  return infr;
}
