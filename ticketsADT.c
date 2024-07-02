#include "ticketsADT.h"
#include <errno.h>

#define N_MONTH 12
#define N_LETTER 'Z' - 'A' + 1
#define N_NUMS '9' - '0' + 1

struct plateNode {
  char *plate;
  size_t infractionAmout;
  struct plateNode *next;
};

typedef struct plateNode *tPlateList;

typedef struct infractionNode {
  char *description;
  size_t infractionAmount;
  tPlateList plateList[N_LETTER + N_NUMS];
  struct infractionNode *nextByAmount;
  struct infractionNode *nextByAlpha;
} tInfractionNode;

struct agencyNode {
  char *name;
  size_t *inf; // Vector de infracciones donde se almacena la cantidad ordenados por id
  size_t maxId;
  struct agencyNode *nextAgency;
};

typedef struct agencyNode *  tAgencyList;

struct ticketsCDT {
  size_t beginYear;
  size_t endYear;
  size_t infractionsDim;
  tInfractionNode *infractions; // Vector de infracciones ordenados por id;
  tInfractionNode *firstByAmount;
  tInfractionNode *currentByAmount;
  tInfractionNode *firstByAlpha;
  tInfractionNode *currentByAlpha;
  size_t (*years)[N_MONTH]; 
  tAgencyList agencies;
};

ticketsADT newTickets(size_t beginYear, size_t endYear) {
  errno = 0;
  if (beginYear > endYear) {
    return NULL;
  }

  ticketsADT tickets = calloc(1, sizeof(*tickets));
  if (tickets==NULL || errno == ENOMEM) {
    return NULL;
  }

  tickets->beginYear = beginYear;
  tickets->endYear = endYear;
  return tickets;
}



int insertInfraction(tInfraction infraction, ticketsADT tickets){

  if (tickets->infractionsDim < infraction.id){

    errno=0;

    tickets->infractions=realloc(tickets->infractions, infraction.id * sizeof(tickets->infractions[0]));

    if (tickets->infractions == NULL || errno == ENOMEM){
      return 0;
    }

    int i=tickets->infractionsDim;
    for (; i<infraction.id ; i++){
      tickets->infractions[i].description=NULL;
    }

    tickets->infractionsDim=infraction.id;
  }

  if (tickets->infractions[infraction.id - 1].description != NULL){
    return 0; 
  }

  tInfractionNode * ticket=&tickets->infractions[infraction.id - 1];
  ticket->description=infraction.description;
  ticket->infractionAmount=0;
  ticket->nextByAlpha=NULL;
  ticket->nextByAmount=NULL;
  for (int i=0 ; i< N_LETTER + N_NUMS ; i++){
    ticket->plateList[i]=NULL;
  }

  return 1; 
}



static tAgencyList addTicketToAgency(tAgencyList agencyNode, char * agency, size_t id);

static void addTicketToYears(size_t (*years)[N_MONTH] , size_t year, size_t month);

static void addTicketToInfraction(tInfractionNode * infractions, size_t id, char * plate);

int insertTicket(tTicket ticket, ticketsADT tickets){

  if (ticket.id > tickets->infractionsDim){ //esto si se hace en varias funciones conviene hacerlo en una macro 
    return 0; 
  }

  tickets->agencies=addTicketToAgency(tickets->agencies, ticket.agency, ticket.id);
  
  addTicketToYears(tickets->years, ticket.year, ticket.month);

  addTicketToInfraction(tickets->infractions, ticket.id, ticket.plate);

}