#include "plateTree.h"
#include "utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define MAX(a, b) (((a) < (b)) ? (b) : (a))

tPlateTree insertToPlateTree(tPlateTree plateTree, char *plate, size_t plateLength) {
  if (plateTree == NULL) {
    errno = 0;
    tPlateNode *node = calloc(1, sizeof(*node));
    if (node == NULL || errno == ENOMEM) {
      return plateTree;
    }
    node->plate = copyOfStr(plate, plateLength);
    if (node->plate == NULL) {
      free(node);
      return plateTree; // errno queda seteado
    }
    node->height = 1;
    return node;
  }

  int cmp;
  if ((cmp = strcmp(plateTree->plate, plate)) > 0) {
    plateTree->left = insertToPlateTree(plateTree->left, plate, plateLength);
  } else if (cmp < 0) {
    plateTree->right = insertToPlateTree(plateTree->right, plate, plateLength);
  } else {
    plateTree->infractionAmount++;
    return plateTree;
  }

  plateTree->height =
      1 + MAX(height(plateTree->left), height(plateTree->right));

  // Obtener el factor de balance de este ancestro nodo para verificar si se
  // desbalanceó
  int balance = getBalance(plateTree);

  // Si este nodo se desbalanceó, hay 4 casos

  // Caso Izquierda Izquierda
  int cmpLeft;
  if (balance > 1 && (cmpLeft = strcmp(plateTree->left->plate, plate) > 0)) {
    return rotateRight(plateTree);
  }

  // Caso Izquierda Derecha
  if (balance > 1 && cmpLeft < 0) {
    plateTree->left = rotateLeft(plateTree->left);
    return rotateRight(plateTree);
  }

  // Caso Derecha Derecha
  int cmpRight;
  if (balance < -1 && (cmpRight = strcmp(plateTree->right->plate, plate)) < 0)
    return rotateLeft(plateTree);

  // Caso Derecha Izquierda
  if (balance < -1 && cmpRight > 0) {
    plateTree->right = rotateRight(plateTree->right);
    return rotateLeft(plateTree);
  }

  // Retornar el nodo (sin cambio)
  return plateTree;
}

int height(tPlateNode *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

int getBalance(tPlateNode *node) {
  if (node == NULL) {
    return 0;
  }
  return height(node->left) - height(node->right);
}

tPlateNode *rotateRight(tPlateNode *node) {
  tPlateNode *left = node->left;
  tPlateNode *subRight = left->right;

  left->right = node;
  node->left = subRight;

  node->height = MAX(height(node->left), height(node->right)) + 1;
  left->height = MAX(height(left->left), height(left->right)) + 1;

  // retorno la nueva raiz
  return left;
}

tPlateNode *rotateLeft(tPlateNode *node) {
  tPlateNode *right = node->right;
  tPlateNode *subLeft = right->left;

  right->left = node;
  node->right = subLeft;

  node->height = MAX(height(node->left), height(node->right)) + 1;
  right->height = MAX(height(right->left), height(right->right)) + 1;

  // retorno la nueva raiz
  return right;
}

// Función para liberar la memoria del árbol
void freeTree(tPlateNode *node) {
  if (node == NULL) {
    return;
  }
  free(node->plate);
  freeTree(node->left);
  freeTree(node->right);
  free(node);
}
