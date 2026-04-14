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
Instrucciones para compilar y ejecutar el programa:

1. Abrir la terminal y ubicarse en la carpeta donde se encuentra el archivo main.cpp.

2. Compilar el programa con el siguiente comando:
   g++ main.cpp -o liga -Wall

   Este comando utiliza el compilador g++ para generar un archivo ejecutable llamado "liga".

3. Ejecutar el programa con el comando:
   liga
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
