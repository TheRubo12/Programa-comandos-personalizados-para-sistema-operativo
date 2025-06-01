#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include "fecha.h"

// Función que inserta la fecha y hora actual en el archivo producto2.txt
void insertarFechaHora() {
    FILE* file = fopen("producto2.txt", "a");
    if (file == NULL) {
        printf("Error al abrir el archivo producto2.txt\n");
        return;
    }

    time_t t;
    struct tm* tm_info;
    char buffer[26];

    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", tm_info);

    fprintf(file, "Fecha y hora: %s\n", buffer);
    fclose(file);

    // Informa al usuario que la operación fue exitosa
    printf("Fecha y hora insertadas en producto2.txt\n");
}
