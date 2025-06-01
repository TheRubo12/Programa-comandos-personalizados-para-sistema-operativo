#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "fecha.h"
#include "ips.h"
#include "adaptador.h"

// Muestra el menú principal y llama a la función correspondiente
void menu() {
    int opcion;
    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Insertar fecha y hora en producto2.txt\n");
        printf("2. Comprobar IPs desde archivo\n");
        printf("3. Obtener configuración de red\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");

        scanf("%d", &opcion); // Lee opción del usuario
        while (getchar() != '\n'); // Limpia buffer

        // Ejecuta la función según la opción
        switch (opcion) {
        case 1:
            insertarFechaHora();
            break;
        case 2:
            comprobarIPs();
            break;
        case 3:
            obtenerConfiguracionRed();
            break;
        case 4:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opción inválida, intente nuevamente.\n");
        }
    } while (opcion != 4);
}

int main() {
    SetConsoleOutputCP(65001); // Habilita UTF-8 para mostrar caracteres correctamente
    menu(); // Llama al menú principal
    return 0;
}
