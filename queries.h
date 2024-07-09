#ifndef __queries__h
#define __queries__h

#include "ticketsADT/ticketsADT.h"

/*
** Genera un archivo CSV llamado "query1.csv" que contiene las infracciones y la cantidad de multas
** correspondientes, ordenado por la cantidad de multas de manera descendente.
** tickets es el ADT de tickets.
** Retorna 1 si el archivo se generó correctamente, 0 en caso de error.
*/
int query1(ticketsADT tickets);

/*
** Genera un archivo CSV llamado "query2.csv" que contiene el nombre de la agencia emisora y por cada agencia emisora la infraccion mas popular y la
** cantidad de multas correspondientes (las agencias emisoras estan ordenadas en orden alfabetico).
** tickets es el ADT de tickets.
** Retorna 1 si el archivo se generó correctamente, 0 en caso de error.
*/
int query2(ticketsADT tickets);

/**
** Genera un archivo CSV llamado "query3.csv" que contiene la descripcion de cada infraccion y por cada infraccion la patente con mas multas y la cantidad de
** multas correspondientes, ordenadas alfabéticamente por infraccion.
** tickets es el ADT de tickets.
** Retorna 1 si el archivo se generó correctamente, 0 en caso de error.
*/
int query3(ticketsADT tickets);

/**
** Genera un archivo CSV llamado "query4.csv" que contiene los años y los tres meses con mayor cantidad
** de multas para cada año.
** tickets es el ADT de tickets.
** Retorna 1 si el archivo se generó correctamente, 0 en caso de error.
*/
int query4(ticketsADT tickets);

#endif

