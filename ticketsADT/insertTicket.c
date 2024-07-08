#include "plateTree.h"
#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static tAgencyList addTicketToAgency(tAgencyList agencyNode, const char *agency, size_t id, size_t infractionsDim, size_t maxLongAgencyName);

static void addTicketToYears(size_t (*years)[N_MONTH], size_t year, size_t month, size_t minYear, size_t maxYear);

static void addTicketToInfraction(tInfractionNode *infractions, size_t id, const char *plate, size_t plateLength);

static int getPlateIndex(const char* plate);

void insertTicket(tTicket ticket, ticketsADT tickets) {
  if (ticket.id >= tickets->infractionsDim) {
    return;
  }

  errno = 0;
  tickets->agencies = addTicketToAgency(tickets->agencies, ticket.agency, ticket.id, tickets->infractionsDim, tickets->agencyLength);
  if (errno == ENOMEM) {
    return;
  }

  addTicketToYears(tickets->years, ticket.year, ticket.month, tickets->beginYear, tickets->endYear);

  errno = 0;
  addTicketToInfraction(tickets->infractions, ticket.id, ticket.plate, tickets->plateLength);
  // if (errno == ENOMEM) {
  //   return;
  // }
}

/* Funcion auxiliar para a una agencia agregarle una infraccion, en caso de que
 * la agencia todavia no tenga ninguna infraccion agrega la agencia a la lista
 * modifica errno si falla
 * */
static tAgencyList addTicketToAgency(tAgencyList agencyNode, const char *agency, size_t id, size_t infractionsDim, size_t maxLongAgencyName) {
  int cmp;
  if (agencyNode == NULL || (cmp = strcmp(agencyNode->name, agency)) > 0) {

    errno = 0;
    tAgencyList newAgency = malloc(sizeof(*newAgency));
    if (newAgency == NULL || errno == ENOMEM) {
      return agencyNode; // Ante un error retorna la misma lista sin hacer cambios
    }

    newAgency->name = copyOfStr(agency, maxLongAgencyName);
    if (newAgency->name == NULL) {
      free(newAgency);
      return agencyNode; // retorna la lista sin hacer cambios
    }
    newAgency->nextAgency = agencyNode;
    newAgency->maxId = id;

    errno=0;
    newAgency->inf = calloc(infractionsDim, sizeof(*newAgency->inf));
    if (newAgency->inf == NULL || errno == ENOMEM){
      free(newAgency);
      return agencyNode;
    }

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

/* Funcion auxiliar para a un anio y un mes agregarle una infraccion
 * si el anio no esta entre el rango de anios solicitado no se agrega
 */
static void addTicketToYears(size_t (*years)[N_MONTH], size_t year, size_t month, size_t minYear, size_t maxYear) {
  if (year < minYear || year > maxYear) {
    return;
  }
  years[year - minYear][month - 1]++;
}

static void addTicketToInfraction(tInfractionNode *infractions, size_t id, const char *plate, size_t plateLength) {
  infractions[id].infractionAmount++;
  int index = getPlateIndex(plate);
  errno = 0;
  infractions[id].plateTree[index] = insertToPlateTree(infractions[id].plateTree[index], plate, plateLength, &infractions[id].maxPlate, &infractions[id].maxPlateAmount); 
}

static int getPlateIndex(const char* plate) {
  if (isalpha(*plate)) {
    return toupper(*plate) - 'A';
  }
  if (isdigit(*plate)) {
    return *plate - '0' + N_LETTER;
  }
  return N_LETTER + N_NUMS;
}

