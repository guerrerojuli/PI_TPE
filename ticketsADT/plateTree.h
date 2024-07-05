#ifndef __plateTree__h
#define __plateTree__h

#include "ticketsADT_internal.h"

#define MAX(a, b) (((a) < (b)) ? (b) : (a))

tPlateNode* insertToPlateTree(tPlateNode* node, char *plate, size_t plateLength);

int height(tPlateNode *node);

int getBalance(tPlateNode *node);

tPlateNode *rotateRight(tPlateNode *node);

tPlateNode *rotateLeft(tPlateNode *node);

void freeTree(tPlateNode *node);

#endif
