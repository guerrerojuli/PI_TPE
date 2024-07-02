typedef unsigned int size_t;

typedef struct ticketsCDT *ticketsADT;

typedef struct ticket {
  size_t id;
  char *agency;
  size_t year;
  char month;
  char patente[10];
} tTicket;

typedef struct infraction {
  size_t id;
  char *description;
} tInfraction;

struct infractionAmountNode {
  char *description;
  size_t amount;
  struct infractionAmountNode *next;
};
typedef struct infractionAmountNode *infractionAmountList;

typedef struct agency {
  char *name;
  char *infractionDesc;
  size_t amount;
} tAgency;

struct infractionPlateNode {
  char *description;
  char *plate;
  size_t amount;
  struct infractionPlateNode *next;
};
typedef struct infractionPlateNode *infractionPlateList;

typedef struct year {
  size_t year;
  size_t top[3];
} tYear;

ticketsADT newTickets(size_t beginYear, size_t endYear);

int insertInfraction(tInfraction infraction, ticketsADT tickets);

int insertTicket(tTicket ticket, ticketsADT tickets);

infractionAmountList getInfractionsByAmount(ticketsADT tickets);

tAgency *popularInfractionByAgency(ticketsADT tickets);

infractionPlateList getInfractionPlate(ticketsADT tickets);

tYear *getTop3Month(ticketsADT tickets);

void freeTickets(ticketsADT tickets);
