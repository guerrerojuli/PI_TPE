#include "ticketsADT.h"
#include <errno.h>
#include <string.h>

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

  if (tickets->infractionsDim <= infraction.id){

    errno=0;
    tickets->infractions=realloc(tickets->infractions, (infraction.id + 1) * sizeof(tickets->infractions[0]));
    if (tickets->infractions == NULL || errno == ENOMEM){
      return 0;
    }

    int i=tickets->infractionsDim;
    for (; i<=infraction.id ; i++){
      tickets->infractions[i].description=NULL;
    }

    tickets->infractionsDim=infraction.id + 1;
  }

  /*Si el id ya tiene vinculado una descripcion retorno 0*/
  if (tickets->infractions[infraction.id].description != NULL){
    return 0; 
  }

  /*como ya se que el id no tiene vinculado una descripcion, agrego la descripcion y seteo todo en NUll o 0, retorno 1*/
  tInfractionNode * ticket=&tickets->infractions[infraction.id];
  ticket->description=infraction.description;
  ticket->infractionAmount=0;
  ticket->nextByAlpha=NULL;
  ticket->nextByAmount=NULL;
  for (int i=0 ; i< N_LETTER + N_NUMS ; i++){
    ticket->plateList[i]=NULL;
  }
  return 1;

}


/*Funcion auxiliar para sumar una multa a la infraccion en el arreglo de infracciones de la agencia y actualizar, en caso de ser necesario, el maxId*/
static tAgencyList
addTicketToAgencyAux(tAgencyList agencyNode, size_t id){

  agencyNode->inf[id]++;

  /*Me fijo si ahora la infraccion que agregue es la que mayor multas tiene*/
  if (agencyNode->inf[id] > agencyNode->inf[agencyNode->maxId]){
    agencyNode->maxId=id;
  }

  return agencyNode;
}

/*Funcion auxiliar para crear un nodo de una agencia que es la primera vez que registra una infraccion*/
static tAgencyList 
newAgencyNode(tAgencyList agencyNode, char * agency, size_t id, size_t infractionsDim){

  errno=0;
  tAgencyList newAgency=malloc(sizeof(*newAgency));
  if (newAgency == NULL || errno == ENOMEM){
    return agencyNode; //Ante un error retorna la misma lista sin hacer cambios
  }

  newAgency->name=agency;
  newAgency->nextAgency=agencyNode;
  newAgency->maxId=id;
  newAgency->inf=calloc(infractionsDim, sizeof(newAgency->inf[0]));
  newAgency->inf[id]++;
  
  return newAgency;
}

/*Funcion auxiliar para a una agencia agregarle una infraccion, en caso de que la agencia todavia no tenga ninguna infraccion agrega la agencia a la lista*/
static tAgencyList 
addTicketToAgency(tAgencyList agencyNode, char * agency, size_t id, size_t infractionsDim){
  int c;

  if (agencyNode == NULL || (c=strcmp(agencyNode->name, agency) > 0)){
    return newAgencyNode(agencyNode, agency, id, infractionsDim);
  }

  if (!c){
    return addTicketToAgencyAux(agencyNode, id);
  }

  agencyNode->nextAgency=addTicketToAgency(agencyNode->nextAgency, agency, id, infractionsDim);
  return agencyNode;
}




static void addTicketToYears(size_t (*years)[N_MONTH] , size_t year, size_t month);

static void addTicketToInfraction(tInfractionNode * infractions, size_t id, char * plate);




int insertTicket(tTicket ticket, ticketsADT tickets){

  if (ticket.id >= tickets->infractionsDim){ //esto si se hace en varias funciones conviene hacerlo en una macro 
    return 0; 
  }

  tickets->agencies=addTicketToAgency(tickets->agencies, ticket.agency, ticket.id, tickets->infractionsDim);
  
  addTicketToYears(tickets->years, ticket.year, ticket.month);

  addTicketToInfraction(tickets->infractions, ticket.id, ticket.plate);

}