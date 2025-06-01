#define _CRT_SECURE_NO_WARNINGS                         // Para evitar advertencias por uso de funciones estándar
#include <stdio.h>                                      // Librería de entrada/salida
#include <stdlib.h>                                     // Para system(), exit(), etc.
#include <string.h>                                     // Para manejo de cadenas
#include "dns_checker.h"                                // Prototipos de este módulo
#include "util.h"                                       // Funciones de utilidad (ping, tracert, etc.)

/**
 * Lee IPs desde un archivo, hace ping a cada una y si responde,
 * la escribe en un archivo temporal para posteriores comparaciones.
 */
void verificarDNSValidos(const char* archivoEntrada, const char* archivoTemporal) {
    FILE* fin = fopen(archivoEntrada, "r");             // Archivo con lista original de DNS
    FILE* fout = fopen(archivoTemporal, "w");           // Archivo donde se escribirán solo los DNS válidos
    char linea[100];

    while (fgets(linea, sizeof(linea), fin)) {          // Leer línea por línea
        linea[strcspn(linea, "\r\n")] = '\0';            // Eliminar salto de línea
        char comando[150];
        snprintf(comando, sizeof(comando), "ping -n 1 -w 100 %s > nul", linea);  // Comando para probar conexión
        if (system(comando) == 0) {                      // Si el ping devuelve 0, la IP responde
            fprintf(fout, "%s\n", linea);                // Guardar IP válida en archivo temporal
        }
    }

    fclose(fin);                                         // Cerrar archivo original
    fclose(fout);                                        // Cerrar archivo temporal
}

/**
 * Determina el mejor servidor DNS según latencia.
 * Si dos DNS empatan en latencia, se elige el de menor cantidad de saltos (hops).
 */
void determinarDNSMasRapido(const char* archivoTemporal, char dnsActuales[][64], int cantidad, char* dnsRapido) {
    FILE* f = fopen(archivoTemporal, "r");              // Abrir archivo con DNS accesibles
    char linea[64];
    char mejor[64] = "";
    int mejorTiempo = 9999;                             // Iniciar con tiempo muy alto
    int mejorSaltos = 30;                               // Y muchos saltos

    while (fgets(linea, sizeof(linea), f)) {            // Leer cada IP accesible
        linea[strcspn(linea, "\r\n")] = '\0';            // Eliminar salto de línea

        int latencia = medirLatencia(linea);            // Medir tiempo de respuesta
        int saltos = contarSaltos(linea);               // Contar cantidad de saltos

        // Elegir la mejor IP:
        // 1. Si es más rápida
        // 2. Si empata en latencia pero tiene menos saltos
        if (latencia < mejorTiempo || (latencia == mejorTiempo && saltos < mejorSaltos)) {
            mejorTiempo = latencia;
            mejorSaltos = saltos;
            strcpy(mejor, linea);                       // Guardar la mejor IP hasta ahora
        }
    }

    fclose(f);                                          // Cerrar archivo
    strcpy(dnsRapido, mejor);                           // Copiar IP ganadora al parámetro de salida
}
