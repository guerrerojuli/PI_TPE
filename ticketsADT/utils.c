#include "utils.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char *copyString(const char *string, size_t strLength) {
  errno = 0;
  char *newName = malloc(strLength + 1);
  if (newName == NULL || errno == ENOMEM) {
    return NULL;
  }
  strncpy(newName, string, strLength + 1);
  return newName;
}
