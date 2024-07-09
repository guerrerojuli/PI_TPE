#ifndef __loadFile__h
#define __loadFile__h

#include "ticketsADT/ticketsADT.h"
#include <stdio.h>

/*
** Lee un archivo en bloques y procesa cada bloque.
** Esta función es más eficiente que leer línea por línea, ya que reduce el número de operaciones de E/S.
** 
** tickets - El ADT de tickets.
** file_infr - El archivo de entrada que contiene los datos a leer.
** processBuffer - Un puntero a una función que procesa el buffer leído.
*/
void loadWithBlocks(ticketsADT tickets, FILE *file_infr, void (*processBuffer)(char *, ticketsADT));

/*
** Procesa el buffer de datos para infracciones y los inserta en el ADT de tickets.
** 
** buffer - El buffer que contiene las infracciones a procesar.
** tickets - El ADT de tickets.
*/
void processBufferInfractions(char buffer[], ticketsADT tickets);

/*
** Procesa el buffer de datos para tickets y los inserta en el ADT de tickets.
** 
** buffer - El buffer que contiene los tickets a procesar.
** tickets - El ADT de tickets.
*/
void processBufferTickets(char *buffer, ticketsADT tickets);

#endif

