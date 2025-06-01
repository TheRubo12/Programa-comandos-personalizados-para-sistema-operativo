# Programa-comandos-personalizados-para-sistema-operativo

Descripción general
Este proyecto ha sido desarrollado en el contexto académico y profesional para automatizar tareas administrativas de red en sistemas Windows utilizando el lenguaje C. A través de cinco productos progresivos, se ha logrado construir una aplicación modular y funcional que interactúa con comandos del sistema, genera informes XML y los transforma a HTML utilizando XSLT.

El enfoque principal ha sido la programación estructurada, el uso de archivos, la integración con comandos de red y la transformación de datos para su análisis.

Productos desarrollados
Producto 1: Acceso al sistema de archivos
Objetivo: Familiarizarse con el acceso y manipulación de archivos en C.
Tareas clave:

Lectura y escritura de archivos .txt.

Captura y análisis de la salida de comandos del sistema.

Producto 2: Gestión de interfaces de red
Objetivo: Automatizar la identificación de adaptadores de red y almacenar la selección del usuario.
Tareas clave:

Uso de netsh y ipconfig.

Registro de la interfaz seleccionada en un archivo temporal.

Producto 3: Automatización del análisis de red
Objetivo: Medir la latencia de servidores DNS y analizar la ruta de red.
Tareas clave:

Uso de ping y tracert.

Procesamiento de salida y cálculo de métricas.

Registro en archivos intermedios.

Producto 4: Generación de archivos XML
Objetivo: Consolidar los datos recogidos y generar un archivo XML con toda la configuración de red.
Tareas clave:

Modularización total del código (6 archivos .c/.h).

Generación de salida.xml con estructura estándar XML.

Manejo de errores y limpieza de datos.

Producto 5: Transformación a HTML con XSLT
Objetivo: Visualizar los datos del XML en un formato amigable para el usuario.
Tareas clave:

Creación de hoja de estilo transformacion.xslt.

Generación automática del archivo salida.html.

Automatización desde C y desde PowerShell.

Tecnologías y herramientas
Lenguaje C (ANSI C)

Windows CMD / PowerShell

Comandos netsh, ping, tracert, ipconfig

XML y XSLT
