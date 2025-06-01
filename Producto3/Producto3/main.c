#define _CRT_SECURE_NO_WARNINGS                         // Evita advertencias por funciones inseguras como scanf, strcpy, etc.
#include <stdio.h>                                      // Librería estándar de entrada/salida
#include <stdlib.h>                                     // Librería para system(), exit(), etc.
#include <string.h>                                     // Librería para manejo de cadenas

#include "adaptador.h"                                  // Módulo para obtener el adaptador de red
#include "dns_checker.h"                                // Módulo para verificar y elegir el mejor DNS
#include "configurador.h"                               // Módulo para cambiar el DNS del sistema
#include "util.h"                                       // Funciones de utilidad como ping y tracert

int main() {
    system("chcp 65001 > nul");                         // Cambia la consola a UTF-8 para evitar caracteres raros

    char rutaArchivo[256];                              // Ruta al archivo dnsips.txt
    char adaptadorNombre[128];                          // Nombre del adaptador seleccionado por el usuario

    // 1. Pedir ruta al archivo con IPs DNS
    printf("Ingrese la ruta del archivo dnsips.txt: ");
    fgets(rutaArchivo, sizeof(rutaArchivo), stdin);     // Leer ruta del archivo ingresado por el usuario
    rutaArchivo[strcspn(rutaArchivo, "\n")] = '\0';     // Eliminar salto de línea al final

    // 2. Mostrar el contenido del archivo dnsips.txt
    mostrarContenidoArchivo(rutaArchivo);               // Llama a función que imprime el contenido

    // 3. Mostrar adaptadores disponibles y guardar el elegido
    obtenerConfiguracionRed();                          // Ejecuta netsh para mostrar interfaces, guarda en adaptador.txt

    // 4. Leer nombre real del adaptador desde adaptador.txt
    FILE* f = fopen("adaptador.txt", "r");              // Abrir archivo generado con el nombre del adaptador
    if (f) {
        if (fgets(adaptadorNombre, sizeof(adaptadorNombre), f)) {   // Leer primera línea (nombre del adaptador)
            adaptadorNombre[strcspn(adaptadorNombre, "\r\n")] = '\0'; // Limpiar retorno de carro

            // Eliminar parte inicial como "Adaptador de ..." si aún queda
            char* ptr = strstr(adaptadorNombre, "de ");
            if (ptr) {
                ptr += 3;
                strcpy(adaptadorNombre, ptr);           // Copiar solo el nombre limpio
            }

            // Eliminar ":" al final si existe
            char* fin = strrchr(adaptadorNombre, ':');
            if (fin) *fin = '\0';
        }
        fclose(f);                                      // Cerrar archivo
    }
    else {
        printf("Error al leer el archivo adaptador.txt\n");
        return 1;                                       // Salir si no se puede leer
    }

    // 5. Mostrar configuración del adaptador antes del cambio
    printf("\nConfiguración actual del adaptador (%s):\n", adaptadorNombre);
    char comandoMostrar[256];
    snprintf(comandoMostrar, sizeof(comandoMostrar),
        "netsh interface ip show config name=\"%s\"", adaptadorNombre);
    system(comandoMostrar);                             // Ejecuta comando netsh para mostrar config actual

    // 6. Obtener DNS configurados actualmente
    char dnsActuales[5][64];                            // Arreglo para guardar DNS actuales
    int cantidadDNS = obtenerDNSActuales("adaptador.txt", dnsActuales); // Llama función que extrae DNS actuales

    // 7. Leer el archivo dnsips.txt y guardar solo los DNS accesibles
    verificarDNSValidos(rutaArchivo, "dns_temp.txt");   // Guarda en archivo temporal solo los que responden

    // 8. Determinar el DNS más rápido (menor latencia y menor número de saltos)
    char dnsRapido[64];                                 // Aquí se guarda el mejor DNS seleccionado
    determinarDNSMasRapido("dns_temp.txt", dnsActuales, cantidadDNS, dnsRapido);

    // 9. Si no está configurado el mejor DNS, se cambia
    if (!dnsYaConfigurado(dnsRapido, dnsActuales, cantidadDNS)) {
        printf("\nSe requiere ejecutar como administrador para cambiar la configuración de red.\n");
        cambiarDNSAdaptador(adaptadorNombre, dnsRapido); // Ejecuta el cambio real con netsh
        printf("DNS cambiado a: %s\n", dnsRapido);
    }
    else {
        printf("Ya está configurado el DNS más rápido (%s).\n", dnsRapido);
    }

    // 10. Mostrar la configuración del adaptador después del cambio
    printf("\nConfiguración del adaptador después del cambio (%s):\n", adaptadorNombre);
    system(comandoMostrar);                             // Mostrar configuración final del adaptador

    return 0;                                           // Fin del programa
}


