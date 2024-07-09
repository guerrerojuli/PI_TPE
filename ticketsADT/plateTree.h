#ifndef __plateTree__h
#define __plateTree__h

#include "ticketsADT_internal.h"

/*
** Inserta una nueva matrícula en el árbol manteniendo le balanceo.
** 
** plateTree - El árbol AVL donde se insertará la matrícula.
** plate - La matrícula a insertar.
** plateLength - La longitud del string de la matrícula.
** maxPlate - Un puntero al string de la matrícula con la mayor cantidad de infracciones.
** maxPlateAmount - Un puntero a la cantidad máxima de infracciones asociadas a una matrícula.
** retorna el árbol actualizado con la nueva matrícula insertada.
*/
tPlateNode* insertToPlateTree(tPlateNode* node, const char *plate, size_t plateLength, char **maxPlate, size_t *maxPlateAmount);

/*Libera los recursos utilizados por el árbol.*/
void freeTree(tPlateNode *node);

#endif
