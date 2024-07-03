#include "ticketsADT.h"

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

typedef struct agencyNode *tAgencyList;

struct ticketsCDT {
  size_t beginYear;
  size_t endYear;
  size_t descLength;
  size_t infractionsDim;
  tInfractionNode *infractions; // Vector de infracciones ordenados por id;
  tInfractionNode *firstByAmount;
  tInfractionNode *currentByAmount;
  tInfractionNode *firstByAlpha;
  tInfractionNode *currentByAlpha;
  size_t (*years)[N_MONTH]; 
  size_t agenciesDim;
  tAgencyList agencies;
};

ticketsADT newTickets(size_t beginYear, size_t endYear) {
  if (beginYear > endYear) {
    return NULL;
  }
  ticketsADT tickets = calloc(1, sizeof(*tickets));
  tickets->beginYear = beginYear;
  tickets->endYear = endYear;
  return tickets;
}
