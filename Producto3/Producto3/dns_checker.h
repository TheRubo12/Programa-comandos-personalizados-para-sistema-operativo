#ifndef DNS_CHECKER_H
#define DNS_CHECKER_H

void verificarDNSValidos(const char* archivoEntrada, const char* archivoTemporal);
void determinarDNSMasRapido(const char* archivoTemporal, char dnsActuales[][64], int cantidad, char* dnsRapido);

#endif
