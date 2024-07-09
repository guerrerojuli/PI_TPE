#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include <string.h>

/* Ordena el vector de infracciones alfabeticamente creando una lista */
static void orderTicketsByAlpha (ticketsADT tickets);
static tInfractionNode *orderTicketsByAlphaRec(tInfractionNode *infrNode, tInfractionNode *infraction);

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
  infr.amount = current->maxPlateAmount;
  tickets->currentByAlpha = current->nextByAlpha;
  return infr;
}

static tInfractionNode *orderTicketsByAlphaRec(tInfractionNode *infrNode, tInfractionNode *infraction) {
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

      if (tickets->infractions[i].description != NULL){
        tickets->firstByAlpha = orderTicketsByAlphaRec(tickets->firstByAlpha, &tickets->infractions[i]);
      }

    }
}
