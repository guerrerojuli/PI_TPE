
# Proyecto Final Programación Imperativa

Repositorio con todos los archivos utilizados para el procesamiento de los archivos de multas de estacionamiento de distintas ciudades.


## Orden de los archivos

En la carpeta [ticketsADT](https://github.com/aguegea/TP_01/tree/main/ticketsADT) se encuentra toda la lógica utilizada en el manejo de los datos extraídos por el front de los achivos csv.

El archivo [loadFile.c](https://github.com/aguegea/TP_01/blob/main/loadFile.c) contiene las funciones utilizadas para leer de a bloques de los archivos recibidos, las culaes están explicadas en [loadFile.h](https://github.com/aguegea/TP_01/blob/main/loadFile.h).

El archivo [queries.c](https://github.com/aguegea/TP_01/blob/main/queries.c) contiene las funciones llamadas desde el main para generar los csv, las cuales están explicadas en [queries.h](https://github.com/aguegea/TP_01/blob/main/queries.h).

El archivo [main.c](https://github.com/aguegea/TP_01/blob/main/main.c) es el que recibe los archivos y llama a las funciones para procesar los datos, y luego a los queries para generar los archivos csv que se entregan al usuario. Las macros utilizadas en el programa están definidas en [main.h](https://github.com/aguegea/TP_01/blob/main/main.h).

El archivo [Makefile](https://github.com/aguegea/TP_01/blob/main/Makefile) es el encargado de compilar el programa y devolver al usuario el ejecutable.

## Implementación

Para generar el ejecutable, desde la consola debes escribir:
```
    $> make CITY
```
Donde CITY es el nombre de la ciudad (CHI, NYC, ...).

Luego, para eliminar los archivos objeto generados al compilar se debe correr el siguiente comando:
```
    $> make clean
```

Se creará un ejecutable con el nombre parkingTicketsCITY, el cual debe ejecutarse de la siguiente manera:
```
    $> parkingTicketCITY  tickets.csv  infractions.csv  beginYear  endYear
```
Donde beginYear y endYear son parámetros numéricos opcionales, mediante los cuales se puede consultar en un rango de años específico los meses más multados.

## Autores

- [@aguegea](https://github.com/aguegea)
- [@nicanovotny](https://github.com/nicanovotny)
- [@maguiGPorzio](https://github.com/maguiGPorzio)
- [@guerrerojuli](https://github.com/guerrerojuli)

