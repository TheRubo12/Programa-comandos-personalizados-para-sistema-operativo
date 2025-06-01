#ifndef UTIL_H
#define UTIL_H

void mostrarContenidoArchivo(const char* ruta);
int medirLatencia(const char* ip);
int contarSaltos(const char* ip);
int dnsYaConfigurado(const char* dns, char actuales[][64], int cantidad);

#endif

