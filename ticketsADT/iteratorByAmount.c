#include "ticketsADT.h"
#include "ticketsADT_internal.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void toBeginByAmount(ticketsADT tickets){
    tickets->currentByAmount=tickets->firstByAmount;
}

int hasNextByAmount(ticketsADT tickets){
    return tickets->currentByAmount != NULL;
}

tInfractionByAmount nextByAmount(ticketsADT tickets){
    if (!hasNextByAmount(tickets)){
        exit(1);
    }
    
    tInfractionByAmount infr;
    infr.description=tickets->currentByAmount->description;
    infr.amount=tickets->currentByAmount->infractionAmount;
    tickets->currentByAmount=tickets->currentByAmount->nextByAmount;
    return infr;
}