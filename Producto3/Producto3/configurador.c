#define _CRT_SECURE_NO_WARNINGS                         // Permite usar funciones como sprintf sin advertencias
#include <stdio.h>                                      // Para printf, sprintf
#include <stdlib.h>                                     // Para system()
#include "configurador.h"                               // Prototipo de la función

/**
 * Cambia el servidor DNS de un adaptador de red utilizando el comando netsh.
 * El nombre del adaptador debe ser exacto (por ejemplo: "Wi-Fi", "Ethernet").
 */
void cambiarDNSAdaptador(const char* nombreAdaptador, const char* dns) {
    char comando[256];                                  // Comando a ejecutar con netsh

    // Construye el comando netsh para establecer el DNS como estático
    snprintf(comando, sizeof(comando),
        "netsh interface ip set dns name=\"%s\" static %s", nombreAdaptador, dns);

    // Ejecuta el comando en la consola
    system(comando);
}

