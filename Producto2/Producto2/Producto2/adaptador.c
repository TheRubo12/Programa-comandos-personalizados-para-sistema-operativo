#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adaptador.h"

#define BUFFER_SIZE 1024

// Función que muestra los adaptadores de red del sistema, permite al usuario seleccionar uno, y guarda su configuración en adaptador.txt
void obtenerConfiguracionRed() {
    system("ipconfig /all > temp_red.txt");

    FILE* file = fopen("temp_red.txt", "r");
    if (file == NULL) {
        printf("Error al obtener la configuración de red.\n");
        return;
    }

    char line[BUFFER_SIZE];
    char adaptadores[20][BUFFER_SIZE];
    int totalAdaptadores = 0;

    // Recorre el archivo buscando líneas que contengan "Adaptador" o "adapter"
    while (fgets(line, BUFFER_SIZE, file)) {
        if (strstr(line, "Adaptador") || strstr(line, "adapter")) {
            line[strcspn(line, "\n")] = 0;
            strcpy(adaptadores[totalAdaptadores], line);
            totalAdaptadores++;
        }
    }

    // Si no se encontró ningún adaptador, se informa y termina la función
    if (totalAdaptadores == 0) {
        printf("No se encontraron adaptadores de red.\n");
        fclose(file);
        return;
    }

    // Muestra los adaptadores encontrados con una lista numerada
    printf("\nSe han encontrado los siguientes adaptadores de red:\n");
    for (int i = 0; i < totalAdaptadores; i++) {
        printf("%d - %s\n", i + 1, adaptadores[i]);
    }

    // Permite al usuario seleccionar un adaptador de la lista
    int seleccion = 0;
    do {
        printf("\nSeleccione un adaptador (1 - %d): ", totalAdaptadores);
        scanf("%d", &seleccion);
        while (getchar() != '\n');
    } while (seleccion < 1 || seleccion > totalAdaptadores);

    const char* adaptadorSeleccionado = adaptadores[seleccion - 1];

    rewind(file);
    FILE* output = fopen("adaptador.txt", "w");
    if (output == NULL) {
        printf("Error al crear adaptador.txt\n");
        fclose(file);
        return;
    }

    int guardar = 0;
    while (fgets(line, BUFFER_SIZE, file)) {
        if (strstr(line, adaptadorSeleccionado)) {
            guardar = 1;
        }
        else if (guardar && (strstr(line, "Adaptador") || strstr(line, "adapter"))) {
            break;
        }

        if (guardar) {
            fputs(line, output);
        }
    }

    fclose(file);
    fclose(output);
    remove("temp_red.txt");

    printf("\nInformación del adaptador guardada en adaptador.txt\n");
}