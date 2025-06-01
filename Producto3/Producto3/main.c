#define _CRT_SECURE_NO_WARNINGS                         // Evita advertencias por funciones inseguras como scanf, strcpy, etc.
#include <stdio.h>                                      // Librer�a est�ndar de entrada/salida
#include <stdlib.h>                                     // Librer�a para system(), exit(), etc.
#include <string.h>                                     // Librer�a para manejo de cadenas

#include "adaptador.h"                                  // M�dulo para obtener el adaptador de red
#include "dns_checker.h"                                // M�dulo para verificar y elegir el mejor DNS
#include "configurador.h"                               // M�dulo para cambiar el DNS del sistema
#include "util.h"                                       // Funciones de utilidad como ping y tracert

int main() {
    system("chcp 65001 > nul");                         // Cambia la consola a UTF-8 para evitar caracteres raros

    char rutaArchivo[256];                              // Ruta al archivo dnsips.txt
    char adaptadorNombre[128];                          // Nombre del adaptador seleccionado por el usuario

    // 1. Pedir ruta al archivo con IPs DNS
    printf("Ingrese la ruta del archivo dnsips.txt: ");
    fgets(rutaArchivo, sizeof(rutaArchivo), stdin);     // Leer ruta del archivo ingresado por el usuario
    rutaArchivo[strcspn(rutaArchivo, "\n")] = '\0';     // Eliminar salto de l�nea al final

    // 2. Mostrar el contenido del archivo dnsips.txt
    mostrarContenidoArchivo(rutaArchivo);               // Llama a funci�n que imprime el contenido

    // 3. Mostrar adaptadores disponibles y guardar el elegido
    obtenerConfiguracionRed();                          // Ejecuta netsh para mostrar interfaces, guarda en adaptador.txt

    // 4. Leer nombre real del adaptador desde adaptador.txt
    FILE* f = fopen("adaptador.txt", "r");              // Abrir archivo generado con el nombre del adaptador
    if (f) {
        if (fgets(adaptadorNombre, sizeof(adaptadorNombre), f)) {   // Leer primera l�nea (nombre del adaptador)
            adaptadorNombre[strcspn(adaptadorNombre, "\r\n")] = '\0'; // Limpiar retorno de carro

            // Eliminar parte inicial como "Adaptador de ..." si a�n queda
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

    // 5. Mostrar configuraci�n del adaptador antes del cambio
    printf("\nConfiguraci�n actual del adaptador (%s):\n", adaptadorNombre);
    char comandoMostrar[256];
    snprintf(comandoMostrar, sizeof(comandoMostrar),
        "netsh interface ip show config name=\"%s\"", adaptadorNombre);
    system(comandoMostrar);                             // Ejecuta comando netsh para mostrar config actual

    // 6. Obtener DNS configurados actualmente
    char dnsActuales[5][64];                            // Arreglo para guardar DNS actuales
    int cantidadDNS = obtenerDNSActuales("adaptador.txt", dnsActuales); // Llama funci�n que extrae DNS actuales

    // 7. Leer el archivo dnsips.txt y guardar solo los DNS accesibles
    verificarDNSValidos(rutaArchivo, "dns_temp.txt");   // Guarda en archivo temporal solo los que responden

    // 8. Determinar el DNS m�s r�pido (menor latencia y menor n�mero de saltos)
    char dnsRapido[64];                                 // Aqu� se guarda el mejor DNS seleccionado
    determinarDNSMasRapido("dns_temp.txt", dnsActuales, cantidadDNS, dnsRapido);

    // 9. Si no est� configurado el mejor DNS, se cambia
    if (!dnsYaConfigurado(dnsRapido, dnsActuales, cantidadDNS)) {
        printf("\nSe requiere ejecutar como administrador para cambiar la configuraci�n de red.\n");
        cambiarDNSAdaptador(adaptadorNombre, dnsRapido); // Ejecuta el cambio real con netsh
        printf("DNS cambiado a: %s\n", dnsRapido);
    }
    else {
        printf("Ya est� configurado el DNS m�s r�pido (%s).\n", dnsRapido);
    }

    // 10. Mostrar la configuraci�n del adaptador despu�s del cambio
    printf("\nConfiguraci�n del adaptador despu�s del cambio (%s):\n", adaptadorNombre);
    system(comandoMostrar);                             // Mostrar configuraci�n final del adaptador

    return 0;                                           // Fin del programa
}


