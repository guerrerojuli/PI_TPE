#ifndef __loadFile__h
#define __loadFile__h

#include "ticketsADT/ticketsADT.h"
#include <stdio.h>

void loadWithBlocks(ticketsADT tickets, FILE *file_infr, void (*processBuffer)(char *, ticketsADT));

void processBufferInfractions(char buffer[], ticketsADT tickets);

void processBufferTickets(char *buffer, ticketsADT tickets);

#endif

