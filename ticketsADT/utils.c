#include "utils.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

char *copyOfStr(const char *string, size_t strLength) {
  errno = 0;
  char *newName = malloc(strLength + 1);
  if (newName == NULL || errno == ENOMEM) {
    return NULL;
  }
  strncpy(newName, string, strLength + 1);
  return newName;
}

int my_strcasecmp(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    int c1 = tolower((unsigned char)*s1);
    int c2 = tolower((unsigned char)*s2);
    if (c1 != c2) {
      return c1 - c2;
    }
    s1++;
    s2++;
  }
  return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}
