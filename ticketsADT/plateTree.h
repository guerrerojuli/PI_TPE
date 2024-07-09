#ifndef __plateTree__h
#define __plateTree__h

#include "ticketsADT_internal.h"

#define MAX(a, b) (((a) < (b)) ? (b) : (a))

/*
** Inserta una nueva matrícula en el árbol y mantiene el árbol balanceado.
** 
** plateTree - El árbol AVL donde se insertará la matrícula.
** plate - La matrícula a insertar.
** plateLength - La longitud del string de la matrícula.
** maxPlate - Un puntero al string de la matrícula con la mayor cantidad de infracciones.
** maxPlateAmount - Un puntero a la cantidad máxima de infracciones asociadas a una matrícula.
** retorna el árbol actualizado con la nueva matrícula insertada.
*/
tPlateNode* insertToPlateTree(tPlateNode* node, const char *plate, size_t plateLength, char **maxPlate, size_t *maxPlateAmount);

/**
** Devuelve la altura de un nodo en el árbol AVL.
**
** node - El nodo cuyo altura se quiere obtener.
** retorna la altura del nodo, o 0 si el nodo es NULL.
*/
int height(tPlateNode *node);

/*
** Calcula el factor de balance de un nodo en el árbol .
**
** node - El nodo cuyo factor de balance se quiere obtener.
** retorna el factor de balance del nodo.
*/
int getBalance(tPlateNode *node);

/*
** Realiza una rotación a la derecha en el subárbol con raíz en el nodo especificado.
**
** node - La raíz del subárbol a rotar.
** Retorna la nueva raíz del subárbol después de la rotación.
*/
tPlateNode *rotateRight(tPlateNode *node);

/*
** Realiza una rotación a la izquierda en el subárbol con raíz en el nodo especificado.
**
** node - La raíz del subárbol a rotar.
** Retorna la nueva raíz del subárbol después de la rotación.
*/
tPlateNode *rotateLeft(tPlateNode *node);

/*Libera los recursos utilizados por el árbol.*/
void freeTree(tPlateNode *node);

#endif
