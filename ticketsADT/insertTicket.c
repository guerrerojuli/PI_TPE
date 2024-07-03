#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static tAgencyList addTicketToAgency(tAgencyList agencyNode, char *agency, size_t id, size_t infractionsDim, size_t maxLongAgencyName);

static void addTicketToYears(size_t (*years)[N_MONTH], size_t year, size_t month);

static void addTicketToInfraction(tInfractionNode *infractions, size_t id, char *plate);

int insertTicket(tTicket ticket, ticketsADT tickets) {
  if (ticket.id >= tickets->infractionsDim) {
    return 0;
  }

  errno = 0;
  tickets->agencies =
      addTicketToAgency(tickets->agencies, ticket.agency, ticket.id, tickets->infractionsDim, tickets->agencyLength);
  if (errno == ENOMEM) {
    return -1;
  }

  addTicketToYears(tickets->years, ticket.year, ticket.month);

  addTicketToInfraction(tickets->infractions, ticket.id, ticket.plate);

  return 1;
}

/* Funcion auxiliar para a una agencia agregarle una infraccion, en caso de que
 * la agencia todavia no tenga ninguna infraccion agrega la agencia a la lista
 * modifica errno si falla
 * */
static tAgencyList addTicketToAgency(tAgencyList agencyNode, char *agency, size_t id, size_t infractionsDim, size_t maxLongAgencyName) {
  int cmp;

  if (agencyNode == NULL || (cmp = strcmp(agencyNode->name, agency) > 0)) {
    errno = 0;
    tAgencyList newAgency = malloc(sizeof(*newAgency));
    if (newAgency == NULL || errno == ENOMEM) {
      return agencyNode; // Ante un error retorna la misma lista sin hacer cambios
    }

    newAgency->name = copyString(agency, maxLongAgencyName);
    if (newAgency->name == NULL) {
      free(newAgency);
      return agencyNode; // retorna la lista sin hacer cambios
    }
    newAgency->nextAgency = agencyNode;
    newAgency->maxId = id;
    newAgency->inf = calloc(infractionsDim, sizeof(*newAgency->inf));
    newAgency->inf[id]++;
    return newAgency;
  } else if (cmp < 0) {
    agencyNode->nextAgency = addTicketToAgency( agencyNode->nextAgency, agency, id, infractionsDim, maxLongAgencyName);
  } else {
    agencyNode->inf[id]++;

    /*Me fijo si ahora la infraccion que agregue es la que mayor multas tiene*/
    if (agencyNode->inf[id] > agencyNode->inf[agencyNode->maxId]) {
      agencyNode->maxId = id;
    }
  }

  return agencyNode;
}
