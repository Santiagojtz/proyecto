Sistema de Gestión de Liga de Fútbol

Estudiante: Santiago Jimenez



Este programa permite gestionar una liga de fútbol de forma sencilla mediante archivos de texto.

El sistema permite:

Registrar partidos
Calcular automáticamente la tabla de posiciones
Aplicar reglas de puntuación configurables (victoria, empate, derrota)
Detectar partidos duplicados
Consultar historial de enfrentamientos entre equipos
Editar resultados de partidos
Exportar la tabla de posiciones a un archivo externo

------------------------------------------------------------------------------
Proyecto Liga de Fútbol en C++
Instrucciones para compilar y ejecutar el programa
Abrir la terminal en la carpeta del proyecto.
Entrar a la carpeta donde se encuentra el código:
cd src
Compilar el programa:
g++ main.cpp -o liga
Ejecutar el programa:
./liga
Archivos necesarios

El programa utiliza los siguientes archivos:

config.txt
fechas.txt
partidos.txt
tabla.txt

Todos estos archivos se encuentran dentro de la carpeta src y deben permanecer en la misma ubicación que el ejecutable para que el programa funcione correctamente.
Descripción

El programa simula una liga de fútbol, permitiendo:

Leer configuración
Procesar partidos
Generar tabla de posiciones
   -------------------------------
   Formato de config.txt

El archivo config.txt define la configuración de la liga.

 Estructura:
nombre: nombre de la liga
victoria: puntos por victoria
empate: puntos por empate
derrota: puntos por derrota
equipo: nombre de cada equipo (uno por línea)
 Ejemplo:
nombre=Liga Colombiana
victoria=3
empate=1
derrota=0
equipo=Millonarios
equipo=Santa Fe
equipo=Atlético Nacional
equipo=América de Cali
-------------------------
Descripcion de las decisiones de diseno mas importantes:
Se utilizaron estructuras (`struct`) para representar las entidades principales del sistema: configuración de la liga, partidos y equipos. Esto permite organizar mejor la información y facilita su manejo dentro del programa.

La tabla de posiciones se calcula dinámicamente a partir de los partidos registrados, en lugar de almacenarse directamente. Esto garantiza la consistencia de los datos y evita errores por información duplicada o desactualizada.

Para ordenar la tabla, se implementó un sistema de múltiples criterios: primero por puntos, luego por diferencia de goles y finalmente por goles a favor. Esto se realiza mediante una función de comparación personalizada.

El sistema valida que no se registren partidos duplicados, verificando que no exista otro con la misma fecha y los mismos equipos (sin importar el orden). Esto evita inconsistencias en los datos.

La edición de partidos se realiza modificando los datos en memoria y luego reescribiendo completamente el archivo `partidos.txt`. De esta forma, se asegura que el archivo siempre refleje el estado actual del sistema.

El programa fue diseñado de manera modular, separando las diferentes funcionalidades en funciones independientes. Esto facilita la escalabilidad, permitiendo agregar nuevas características o mejorar el sistema en el futuro.
