#include "plateTree.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>


/**
** Devuelve la altura de un nodo en el árbol AVL.
**
** node - El nodo cuyo altura se quiere obtener.
** retorna la altura del nodo, o 0 si el nodo es NULL.
*/
static int height(tPlateNode *node);

/*
** Calcula el factor de balance de un nodo en el árbol .
**
** node - El nodo cuyo factor de balance se quiere obtener.
** retorna el factor de balance del nodo.
*/
static int getBalance(tPlateNode *node);

/*
** Realiza una rotación a la derecha en el subárbol con raíz en el nodo especificado.
**
** node - La raíz del subárbol a rotar.
** Retorna la nueva raíz del subárbol después de la rotación.
*/
static tPlateNode *rotateRight(tPlateNode *node);

/*
** Realiza una rotación a la izquierda en el subárbol con raíz en el nodo especificado.
**
** node - La raíz del subárbol a rotar.
** Retorna la nueva raíz del subárbol después de la rotación.
*/
static tPlateNode *rotateLeft(tPlateNode *node);


tPlateTree insertToPlateTree(tPlateTree plateTree, const char *plate, size_t plateLength, char **maxPlate, size_t *maxPlateAmount) {
  if (plateTree == NULL) {
    errno = 0;
    tPlateNode *node = malloc(sizeof(*node));
    if (node == NULL || errno == ENOMEM) {
      return plateTree;
    }
    node->plate = copyOfStr(plate, plateLength);
    if (node->plate == NULL) {
      free(node);
      return plateTree; // errno queda seteado
    }
    node->height = 1;
    node->infractionAmount = 1;
    node->left = node->right = NULL;
    if (*maxPlate == NULL) {
      *maxPlate = node->plate;
      *maxPlateAmount = node->infractionAmount;
    }
    return node;
  }

  int cmp;
  if ((cmp = my_strcasecmp(plateTree->plate, plate)) > 0) {
    plateTree->left = insertToPlateTree(plateTree->left, plate, plateLength, maxPlate, maxPlateAmount);
  } else if (cmp < 0) {
    plateTree->right = insertToPlateTree(plateTree->right, plate, plateLength, maxPlate, maxPlateAmount);
  } else {
    plateTree->infractionAmount++;
    if (*maxPlateAmount < plateTree->infractionAmount) {
      *maxPlateAmount = plateTree->infractionAmount;
      *maxPlate = plateTree->plate;
    }
    return plateTree;
  }

  plateTree->height = 1 + MAX(height(plateTree->left), height(plateTree->right));

  // Obtener el factor de balance de este ancestro nodo para verificar si se
  // desbalanceó
  int balance = getBalance(plateTree);

  
  if (balance > 1){
    int cmpRight=my_strcasecmp(plateTree->left->plate, plate);

    if (cmpRight == 0){
      return plateTree;
    }

    if (cmpRight < 0 && plateTree->left->right != NULL ) {
      plateTree->left = rotateLeft(plateTree->left);
    }

    return rotateRight(plateTree);
  }

  
  if (balance < -1){
    int cmpLeft=my_strcasecmp(plateTree->right->plate, plate);

    if (cmpLeft == 0){
      return plateTree;
    }

    if (cmpLeft > 0 && plateTree->right->left != NULL){
      plateTree->right = rotateRight(plateTree->right);
    }

    return rotateLeft(plateTree);

  }

  // Retornar el nodo (sin cambio)
  return plateTree;
}

static int height(tPlateNode *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

static int getBalance(tPlateNode *node) {
  if (node == NULL) {
    return 0;
  }
  return height(node->left) - height(node->right);
}

static tPlateNode *rotateRight(tPlateNode *node) {
  tPlateNode *left = node->left;
  tPlateNode *subRight = left->right;

  left->right = node;
  node->left = subRight;

  node->height = MAX(height(node->left), height(node->right)) + 1;
  left->height = MAX(height(left->left), height(left->right)) + 1;

  // retorno la nueva raiz
  return left;
}

static tPlateNode *rotateLeft(tPlateNode *node) {
  tPlateNode *right = node->right;
  tPlateNode *subLeft = right->left;

  right->left = node;
  node->right = subLeft;

  node->height = MAX(height(node->left), height(node->right)) + 1;
  right->height = MAX(height(right->left), height(right->right)) + 1;

  // retorno la nueva raiz
  return right;
}

void freeTree(tPlateNode *node) {
  if (node == NULL) {
    return;
  }
  free(node->plate);
  freeTree(node->left);
  freeTree(node->right);
  free(node);
}
