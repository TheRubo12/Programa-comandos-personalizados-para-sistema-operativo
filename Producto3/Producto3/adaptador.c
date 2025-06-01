#define _CRT_SECURE_NO_WARNINGS                         // Para evitar advertencias por funciones como scanf, strcpy
#include <stdio.h>                                      // Librería estándar de entrada/salida
#include <stdlib.h>                                     // Librería para system, exit, etc.
#include <string.h>                                     // Para funciones de cadenas
#include "adaptador.h"                                  // Prototipos de las funciones

#define MAX_ADAPTADORES 20                              // Máximo número de adaptadores que vamos a manejar
#define BUFFER_SIZE 256                                 // Tamaño para líneas leídas

/**
 * Esta función obtiene una lista de adaptadores de red disponibles
 * usando 'netsh interface show interface' y permite al usuario seleccionar uno.
 * El nombre seleccionado se guarda en 'adaptador.txt'
 */
void obtenerConfiguracionRed() {
    system("netsh interface show interface > interfaces.txt");  // Ejecutar comando y guardar salida

    FILE* file = fopen("interfaces.txt", "r");          // Abrir archivo generado por el comando netsh
    if (!file) {
        printf("Error al obtener las interfaces.\n");
        return;
    }

    char line[BUFFER_SIZE];
    char adaptadores[MAX_ADAPTADORES][BUFFER_SIZE];
    int total = 0;

    // Saltar cabecera del comando
    while (fgets(line, BUFFER_SIZE, file)) {
        if (strstr(line, "Nombre interfaz")) {
            break;
        }
    }

    // Leer nombres de interfaces desde cada línea
    while (fgets(line, BUFFER_SIZE, file) && total < MAX_ADAPTADORES) {
        line[strcspn(line, "\r\n")] = '\0';              // Eliminar salto de línea

        // Buscar la última columna (nombre de la interfaz)
        char* nombre = strrchr(line, ' ');
        if (nombre && strlen(nombre) > 1) {
            nombre++;                                    // Saltar el espacio
            strcpy(adaptadores[total++], nombre);        // Guardar el nombre limpio
        }
    }

    fclose(file);                                       // Cerrar archivo temporal
    remove("interfaces.txt");                           // Eliminar el archivo temporal

    if (total == 0) {
        printf("No se encontraron adaptadores.\n");
        return;
    }

    // Mostrar adaptadores encontrados
    printf("\nAdaptadores disponibles:\n");
    for (int i = 0; i < total; i++) {
        printf("%d - %s\n", i + 1, adaptadores[i]);     // Mostrar cada adaptador numerado
    }

    // Permitir al usuario seleccionar un adaptador
    int opcion = 0;
    int result = 0;
    do {
        printf("\nSeleccione un adaptador (1 - %d): ", total);
        result = scanf("%d", &opcion);                  // Leer selección del usuario
        while (getchar() != '\n');                      // Limpiar buffer de entrada
    } while (result != 1 || opcion < 1 || opcion > total);

    // Guardar el nombre del adaptador seleccionado en archivo
    FILE* out = fopen("adaptador.txt", "w");
    if (out) {
        fprintf(out, "%s\n", adaptadores[opcion - 1]);  // Escribir nombre exacto del adaptador
        fclose(out);
    }

    printf("\nNombre del adaptador seleccionado guardado: %s\n", adaptadores[opcion - 1]);
}

/**
 * Lee los DNS actuales del archivo adaptador.txt y los guarda en un arreglo.
 */
int obtenerDNSActuales(const char* archivo, char dnsActuales[][64]) {
    FILE* f = fopen(archivo, "r");                      // Abrir archivo con configuración del adaptador
    if (!f) return 0;

    char linea[256];
    int contador = 0;

    // Buscar líneas con 'DNS' y extraer dirección IP
    while (fgets(linea, sizeof(linea), f)) {
        if (strstr(linea, "DNS")) {
            char* ptr = strchr(linea, ':');             // Buscar el ':' para separar campo de valor
            if (ptr) {
                ptr++;                                  // Moverse después del ':'
                while (*ptr == ' ') ptr++;              // Saltar espacios
                strcpy(dnsActuales[contador++], ptr);   // Copiar IP al arreglo
                dnsActuales[contador - 1][strcspn(dnsActuales[contador - 1], "\r\n")] = '\0'; // Limpiar salto
            }
        }
    }

    fclose(f);                                          // Cerrar archivo
    return contador;                                    // Devolver cuántos DNS se encontraron
}
