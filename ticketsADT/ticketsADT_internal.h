#ifndef __ticketsADT_internal__h
#define __ticketsADT_internal__h

#include <stddef.h>
 
#define N_MONTH 12
#define N_LETTER 'Z' - 'A' + 1
#define N_NUMS '9' - '0' + 1

typedef struct plateNode {
  char *plate;
  size_t infractionAmount;
  size_t height;
  struct plateNode *left;
  struct plateNode *right;
} tPlateNode;

typedef struct plateNode *tPlateTree;

typedef struct infractionNode {
  char *description;
  size_t infractionAmount;
  tPlateTree plateTree;
  struct infractionNode *nextByAmount;
  struct infractionNode *nextByAlpha;
} tInfractionNode;

typedef struct agencyNode {
  char *name;
  size_t *inf; // Vector de infracciones donde se almacena la cantidad ordenados por id
  size_t maxId;
  struct agencyNode *nextAgency;
} tAgencyNode;

typedef struct agencyNode *tAgencyList;

struct ticketsCDT {
  size_t beginYear;
  size_t endYear;
  size_t infractionsDim;
  size_t plateLength;
  size_t descLength;
  size_t agencyLength;
  tInfractionNode *infractions; // Vector de infracciones ordenados por id;
  tInfractionNode *firstByAmount;
  tInfractionNode *currentByAmount;
  tInfractionNode *firstByAlpha;
  tInfractionNode *currentByAlpha;
  size_t (*years)[N_MONTH];
  tAgencyList agencies;
  tAgencyNode *currentAgency;
};

#endif
