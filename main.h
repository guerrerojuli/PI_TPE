#ifndef __main__h
#define __main__h

#define LINEAS 50 // manejo #años #agencias #infracciones. Copio de a 50 lineas al archivo.
#define MAX_MONTHS 13      // 12 meses + 1 empty
#define MAX_LONG_INT 11 // Maxima cantidad de caracteres de un int y el cero final
#define MAX_LINE_LENGTH 256
#define MAX_LINE_QUERY4 34 //(4+1+9+1+9+1+9)
#define START_YEAR 1900
#define SUCCESS 0
#define ERROR 1
// El 0 final lo cuentan desde el backend
#define PLATE_LENGTH 10
#define BUFFER_SIZE 16384 // 16 KB

#ifdef CHI
#define MAX_INFRACTION 50
#define MAX_AGENCY 13
#define MAX_LINE_QUERY1 61 //(50+1+10)
#define MAX_LINE_QUERY2 75 //(13+1+50+1+10)
#define MAX_LINE_QUERY3 72 //(50+1+10+1+10)
#define parseToken(token, id, plate, month, year, agency) (sscanf((token), "%[^-]-%[^-]-%*[^;];%[^;];%[^;];%[^;];%*[^\n]\n", (year), (month), (plate), (agency), (id)))

#elif NYC
#define MAX_INFRACTION 30
#define MAX_AGENCY 35
#define MAX_LINE_QUERY1 41 //(30+1+10)
#define MAX_LINE_QUERY2 77 //(35+1+30+1+10)
#define MAX_LINE_QUERY3 52 //(30+1+10+1+10)
#define parseToken(token, id, plate, month, year, agency) (sscanf((token), "%[^;];%[^-]-%[^-]-%*[^;];%[^;];%*[^;];%[^\n]\n", (plate), (year), (month), (id), (agency)))

#else
#error "Debe compilar con -D$(CIUDAD)"
#define MAX_INFRACTION 0
#define MAX_AGENCY 0
#define MAX_LINE_QUERY1 0
#define MAX_LINE_QUERY2 0
#define MAX_LINE_QUERY3 0
#define parseToken(token, id, plate, month, year, agency)

#endif

#endif
