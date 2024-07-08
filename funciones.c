#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include "ticketsADT/ticketsADT.h"
#include "main.h"

// Archivo en el cual guardamos las funciones auxiliares utilizadas en el main de CHICAGO y NYC análogamente
FILE *openFile(char* arg) {
    FILE *res = fopen(arg, "r");
    if ( res == NULL ) {
        fprintf(stderr, "Error al abrir archivo: %s\n", arg);
        exit(ERROR);
    }
    return res;
}

static int isNumber(char *s) {
  while (*s){
    if ( !isdigit(*s) ) {
        return 0;
    }
    s++;
  }
  return 1;
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
  if (!isNumber(argv[3]) || (*beginYear = atoi(argv[3])) < 0){
    *beginYear = -1;
    *endYear = -1;
    return;
  }
  if (argc == 4) {
    if (*beginYear < START_YEAR) {
        *beginYear = START_YEAR;
    }
    if (*beginYear > currentYear){
        *endYear = *beginYear;
        return;
    }
    *endYear = currentYear;
    return;
  }
  // argc = 5
  if (!isNumber(argv[4]) || *beginYear > (*endYear = atoi(argv[4])) || *endYear < 0){
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
  if (*beginYear > currentYear){
      *endYear = *beginYear;
      return;
  }
  if (*endYear > currentYear) {
    *endYear = currentYear;
  }
}

void processBufferInfractions(char buffer[], ticketsADT tickets) {
    tInfraction infr_aux; // Estructura donde iré guardando los datos de cada infracción
    char descAux[51];
    char id_aux[MAX_LONG_INT];
    char* token = strtok(buffer, "\n");
    while ( token != NULL ) {
        sscanf(token, "%[^;];%[^\n]\n", id_aux, descAux);
        infr_aux.id = atoi(id_aux);
        infr_aux.description = descAux;
        errno = 0;
        insertInfraction(infr_aux, tickets);
        if ( errno == ENOMEM ) {
            fprintf(stderr, "Error al cargar datos del archivo de infracciones por falta de memoria.\n");
            exit(ERROR);
        }
        token = strtok(NULL, "\n");
    }
}

void processBufferTickets(char *buffer, ticketsADT tickets) {
  tTicket ticket;
  char id_aux[MAX_LONG_INT];
  char month_aux[MAX_LONG_INT];
  char year_aux[MAX_LONG_INT];
  char agency_aux[MAX_AGENCY + 1];
  char* token = strtok(buffer, "\n");
  while ( token != NULL ) {
    parseToken(token, id_aux, ticket.plate, month_aux, year_aux, agency_aux);
    ticket.id = atoi(id_aux);
    ticket.year = atoi(year_aux);
    ticket.month = (char) atoi(month_aux);
    ticket.agency = agency_aux;
    errno = 0;
    insertTicket(ticket, tickets);
    if ( errno == ENOMEM ) {
        fprintf(stderr, "Error al cargar datos del archivo de tickets por falta de memoria.\n");
        exit(ERROR);
    }
    token = strtok(NULL, "\n");
  }
}

// Función optimizada, en vez de leer línea por línea, traigo de a chunks de memoria del archivo ya que es 
// muy costoso la lectura de archivos desde el disco duro. Proceso el bloque en una funcion auxiliar.
// La funcion es analoga para infracciones y tickets, por eso le pasamos un puntero a funcion
void loadWithBlocks(ticketsADT tickets, FILE *file_infr, void (*processBuffer)(char *, ticketsADT) ) {
    fscanf(file_infr, "%*[^\n]\n"); // Sacamos los nombre de los campos de la primer línea
    size_t bytesRead; // Cantidad de bytes leidos por fread, si es menor al pedido significa que llegue al final del archivo
    char buffer[BUFFER_SIZE + 1]; // Iré guardando el boque traido del archivo acá
    size_t temp_length = 0; // Longitud restante luego de truncar el bloque hasta el ultimo \n
    char *buffer_pointer = buffer; // Puntero desde el cual guardará el bloque fread
    while ( (bytesRead = fread(buffer_pointer, 1, BUFFER_SIZE - temp_length, file_infr)) > 0 ) {
        if ( bytesRead == BUFFER_SIZE - temp_length ) { // Chequeo no estar al final del archivo
            buffer[bytesRead + temp_length] = '\0'; // Le pongo el 0 final
            char* lastLine = strrchr(buffer, '\n'); // Será el puntero al ultimo caracter de mi buffer, elimino la linea
            size_t buffer_length = lastLine - buffer;
            buffer[buffer_length] = '\0'; // Piso el \n por \0, strtok me reconoce el final
            processBuffer(buffer, tickets);
            buffer_pointer = buffer + buffer_length + 1;
            strcpy(buffer, buffer_pointer);
            temp_length = strlen(buffer);
            buffer_pointer = buffer + temp_length;
        }
        else { // Si estoy al final del archivo, agrego un \n al final de la linea final y pongo el 0 final
            buffer[bytesRead + temp_length] = '\n';
            buffer[bytesRead + temp_length + 1] = '\0';
            processBuffer(buffer, tickets);
        }
    }
}
