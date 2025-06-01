#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ips.h"

#define BUFFER_SIZE 1024

// Lee un archivo con IPs, muestra su contenido, realiza ping y guarda las que responden
void comprobarIPs() {
    char archivoRuta[BUFFER_SIZE];
    printf("Ingrese la ruta del archivo con las IPs (debe ir entre comillas): ");
    fgets(archivoRuta, BUFFER_SIZE, stdin); // Lee la ruta del archivo
    archivoRuta[strcspn(archivoRuta, "\n")] = 0; // Elimina el salto de línea

    // Validar que la ruta esté entre comillas
    size_t len = strlen(archivoRuta);
    if (len < 2 || archivoRuta[0] != '\"' || archivoRuta[len - 1] != '\"') {
        printf("La ruta debe ir entre comillas dobles. Ejemplo: \"C:\ruta\archivo.txt\"\n");
        return;
    }

    // Eliminar comillas inicial y final
    memmove(archivoRuta, archivoRuta + 1, len - 2);
    archivoRuta[len - 2] = '\0';

    FILE* file = fopen(archivoRuta, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo: %s\n", archivoRuta);
        return;
    }

    char ip[BUFFER_SIZE];

    // Mostrar el contenido del archivo por pantalla antes de lanzar pings
    printf("\nContenido del archivo:\n");
    while (fgets(ip, BUFFER_SIZE, file)) {
        printf("%s", ip);
        if (ip[strlen(ip) - 1] != '\n') printf("\n");
    }
    rewind(file); // Volver al inicio

    // Abre archivo de salida en modo añadir, donde se guardarán las IPs que responden
    FILE* outputFile = fopen("producto2.txt", "a");
    if (outputFile == NULL) {
        printf("Error al abrir producto2.txt\n");
        fclose(file);
        return;
    }

    fprintf(outputFile, "\nResultados del ping:\n");
   
    // Procesa cada IP, lanza un ping y guarda las que responden
    while (fgets(ip, BUFFER_SIZE, file)) {
        ip[strcspn(ip, "\n")] = 0;
        char command[BUFFER_SIZE];
        snprintf(command, BUFFER_SIZE, "ping -n 1 %s > nul", ip);
        // Ejecuta el ping y evalúa si hubo respuesta
        if (system(command) == 0) {
            printf("Respuesta positiva de: %s\n", ip);
            fprintf(outputFile, "%s\n", ip);
        }
        else {
            printf("Sin respuesta de: %s\n", ip);
        }
    }
    
    // Cierra los archivos abiertos
    fclose(file);
    fclose(outputFile);
    printf("Resultados almacenados en producto2.txt\n");
}

