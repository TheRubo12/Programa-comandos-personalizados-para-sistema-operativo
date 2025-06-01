#define _CRT_SECURE_NO_WARNINGS                         // Permite usar funciones est�ndar como fopen, scanf, etc.
#include <stdio.h>                                      // Librer�a de entrada/salida
#include <stdlib.h>                                     // Para system(), malloc(), etc.
#include <string.h>                                     // Manejo de cadenas
#include "util.h"                                       // Prototipos del m�dulo

/**
 * Muestra el contenido de un archivo por consola.
 */
void mostrarContenidoArchivo(const char* ruta) {
    FILE* f = fopen(ruta, "r");                         // Abre el archivo indicado
    if (!f) return;
    char linea[256];

    while (fgets(linea, sizeof(linea), f)) {            // Lee l�nea por l�nea
        printf("%s", linea);                            // Imprime cada l�nea
    }
    fclose(f);                                          // Cierra el archivo
}

/**
 * Ejecuta ping a una IP y devuelve la latencia en milisegundos.
 * Si no se puede medir, devuelve 9999.
 */
int medirLatencia(const char* ip) {
    char comando[128];
    snprintf(comando, sizeof(comando), "ping -n 1 -w 100 %s > temp_ping.txt", ip);
    system(comando);                                     // Ejecuta el comando y guarda la salida

    FILE* f = fopen("temp_ping.txt", "r");              // Abrir salida del ping
    char linea[256];
    int ms = 9999;                                       // Valor por defecto (muy alto)

    while (fgets(linea, sizeof(linea), f)) {            // Leer salida del ping
        if (strstr(linea, "tiempo") || strstr(linea, "Tiempo")) {
            int ok = sscanf(linea, "%*[^=]=%dms", &ms);  // Extrae valor num�rico (ignora lo anterior)
            (void)ok;                                    // Evita advertencia si no se usa 'ok'
            break;
        }
    }

    fclose(f);                                          // Cierra archivo
    remove("temp_ping.txt");                            // Limpia archivo temporal

    return ms;                                          // Devuelve la latencia medida
}

/**
 * Ejecuta tracert (rastreo de ruta) a una IP y cuenta la cantidad de saltos (hops).
 */
int contarSaltos(const char* ip) {
    char comando[128];
    snprintf(comando, sizeof(comando), "tracert -h 30 -d %s > temp_tracert.txt", ip);
    system(comando);                                     // Ejecutar tracert

    FILE* f = fopen("temp_tracert.txt", "r");
    char linea[256];
    int saltos = 0;

    while (fgets(linea, sizeof(linea), f)) {            // Cuenta l�neas que contienen latencia
        if (strstr(linea, " ms") || strstr(linea, "ms")) {
            saltos++;                                   // Cada l�nea con tiempo = un salto
        }
    }

    fclose(f);
    remove("temp_tracert.txt");                         // Limpia archivo temporal

    return saltos;                                      // Devuelve el n�mero de saltos
}

/**
 * Verifica si una IP ya est� configurada como DNS.
 */
int dnsYaConfigurado(const char* dns, char actuales[][64], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        if (strcmp(dns, actuales[i]) == 0)              // Compara con cada DNS actual
            return 1;                                   // Ya est� configurado
    }
    return 0;                                           // No est� configurado
}
