#include <stdio.h>
#include <stdlib.h>
#include "xslt.h"

int aplicarTransformacionXSLT(const char* xml, const char* xslt, const char* salidaHtml) {
    char comando[512];

    snprintf(comando, sizeof(comando),
        "powershell -Command \""
        "$xml = New-Object System.Xml.XmlDocument; "
        "$xml.Load('%s'); "
        "$xslt = New-Object System.Xml.Xsl.XslCompiledTransform; "
        "$xslt.Load('%s'); "
        "$writer = New-Object System.IO.StreamWriter('%s'); "
        "$xslt.Transform($xml, $null, $writer); "
        "$writer.Close()\"",
        xml, xslt, salidaHtml);

    printf("Ejecutando transformación con PowerShell:\n%s\n", comando);

    int resultado = system(comando);
    if (resultado != 0) {
        printf("Error al ejecutar PowerShell. Código de salida: %d\n", resultado);
    }
    else {
        printf("Transformación XSLT completada con éxito.\n");
    }

    return resultado == 0;
}
