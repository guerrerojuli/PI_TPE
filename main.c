#include "main.h"
#include "queries.h"
#include "loadFile.h"
#include "ticketsADT/ticketsADT.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*
** Comprueba que todos los caracteres del string #s sean digitos.
** Retorna 1 si lo son, 0 si no lo son.
*/
static int isNumber(char *s);

/*
** Define #beginYear y #endYear segun los argumentos recibidos.
** Define #beginYear = -1 si los parametros son inválidos.
*/
static void getYearRange(int argc, char *argv[], int *beginYear, int *endYear);

int main(int argc, char *argv[]) {
  errno = 0;
  int beginYear, endYear;
  if (argc < 3 || argc > 5) {
    fprintf(stderr, "Error: cantidad de parámetros inválida\n");
    return ERROR;
  }

  getYearRange(argc, argv, &beginYear, &endYear);
  if (beginYear == -1) {
    fprintf(stderr, "Error en los años pasados como parametros\n");
    return ERROR;
  }
  // Creo mi ADT, en el cual guardaré los datos
  ticketsADT tickets = newTickets(beginYear, endYear, MAX_INFRACTION, MAX_AGENCY, PLATE_LENGTH);
  if ( tickets == NULL ) {
    fprintf(stderr, "Error por falta de memoria\n");
    return ERROR;
  }

  // Primera parte: Leo argv[2], el cual será el archivo csv donde guardo
  // infracciones con su respectivo id Formarto: id;descripcion
  FILE *fileInfr = fopen(argv[2], "r");
  if (fileInfr == NULL) {
    openFileError(argv[2]);
    return ERROR;
  }
  loadWithBlocks(tickets, fileInfr, processBufferInfractions);
  if (errno == ENOMEM) {
    fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
    freeTickets(tickets);
    return ERROR;
  }
  fclose(fileInfr);

  // Segunda parte: Leo argv[1], el cual será el archivo csv donde guardo multas
  // Formarto: issueDate;plateRedacted;unitDescription;infrId;fineLevel1Amount
  FILE *fileTickets = fopen(argv[1], "r");
  if (fileInfr == NULL) {
    openFileError(argv[1]);
    return ERROR;
  }
  errno = 0;
  loadWithBlocks(tickets, fileTickets, processBufferTickets);
  if (errno == ENOMEM) {
      fprintf(stderr, "Error al cargar datos del archivo de tickets por falta de memoria.\n");
      freeTickets(tickets);
      return ERROR;
  }
  fclose(fileTickets);

  int res = 0;
  res = query1(tickets);
  if (!res) {
    createFileError("./query1.csv");
    freeTickets(tickets);
    return ERROR;
  }
  res = query2(tickets);
  if (!res) {
    createFileError("./query2.csv");
    freeTickets(tickets);
    return ERROR;
  }
  res = query3(tickets);
  if (!res) {
    createFileError("./query3.csv");
    freeTickets(tickets);
    return ERROR;
  }
  res = query4(tickets);
  if (!res) {
    createFileError("./query4.csv");
    freeTickets(tickets);
    return ERROR;
  } 

  freeTickets(tickets);
  return 0;
}

static void getYearRange(int argc, char *argv[], int *beginYear, int *endYear) {
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  int currentYear = local->tm_year + 1900;
  if (argc == 3) {
    *beginYear = START_YEAR;
    *endYear = currentYear;
    return;
  }
  if (!isNumber(argv[3]) || (*beginYear = atoi(argv[3])) < 0) {
    *beginYear = -1;
    *endYear = -1;
    return;
  }
  if (argc == 4) {
    if (*beginYear < START_YEAR) {
      *beginYear = START_YEAR;
    }
    if (*beginYear > currentYear) {
      *endYear = *beginYear;
      return;
    }
    *endYear = currentYear;
    return;
  }
  // argc = 5
  if (!isNumber(argv[4]) || *beginYear > (*endYear = atoi(argv[4])) || *endYear < 0) {
    *beginYear = -1;
    *endYear = -1;
    return;
  }
  if (*beginYear < START_YEAR) {
    *beginYear = START_YEAR;
  }
  if (*endYear < START_YEAR) {
    *endYear = START_YEAR;
  }
  if (*beginYear > currentYear) {
    *endYear = *beginYear;
    return;
  }
  if (*endYear > currentYear) {
    *endYear = currentYear;
  }
}

static int isNumber(char *s) {
  while (*s) {
    if (!isdigit(*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}
