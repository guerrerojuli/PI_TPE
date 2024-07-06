#ifndef __utils__h
#define __utils__h

#include <stddef.h>

#define MAX(a, b) (((a) < (b)) ? (b) : (a))

/* Retorna una copia de un string
 * Retorna NULL si falla
 */
char *copyOfStr(const char *string, size_t strLength);

#endif
