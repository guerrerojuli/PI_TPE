#include "ticketsADT.h"
#include "ticketsADT_internal.h"

void toBeginAgency(ticketsADT tickets) {
  tickets->currentAgency = tickets->agencies;
}

int hasNextAgency(ticketsADT tickets) {
  return (tickets->currentAgency != NULL);
}

tAgency nextAgency(ticketsADT tickets) {
  tAgency agency;

  if (!hasNextAgency(tickets)) {
    agency.name = NULL;
    agency.amount = 0;
    agency.infractionDesc = 0;
    return agency;
  }

  tAgencyNode *current = tickets->currentAgency;
  agency.name = current->name;
  agency.amount = current->inf[current->maxId];
  agency.infractionDesc = tickets->infractions[current->maxId].description;
  tickets->currentAgency=current->nextAgency;
  return agency;
}
