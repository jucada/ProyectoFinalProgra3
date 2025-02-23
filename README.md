# Programación III: Proyecto Final (2025-0)

## Integrantes
- Ian Rodriguez Peres
- Juan Carlos Paredes Alva

# Plataforma de Streaming

## Descripción

Este proyecto consiste en el desarrollo de una plataforma de streaming que permite la búsqueda y visualización de sinopsis de películas. Utiliza estructuras de datos avanzadas para facilitar la búsqueda rápida de títulos dentro de una base de datos de películas en formato CSV.

## Características

- **Carga de base de datos**: El programa lee y procesa un archivo CSV que contiene información sobre películas.
- **Búsqueda eficiente**: Se utiliza una estructura de Árbol para almacenar los títulos y sinopsis de las películas, permitiendo búsquedas rápidas y eficientes.
- **Búsqueda flexible**:
  - Por palabra clave en el título o sinopsis.
  - Por frases completas.
  - Por fragmentos de palabras.
  - Por resultados.
- **Favoritos y recomendaciones**:
  - Posibilidad de marcar películas como "Ver más tarde".
  - Recomendaciones basadas en películas a las que el usuario les dio "Like".

## Tecnologías y herramientas utilizadas

- **Lenguaje**: C++
- **Compilación**: CMake
- **Estructuras de datos**: Árbol de búsqueda optimizado para consultas eficientes.
- **IDE recomendado**: CLion

## Instalación y ejecución

1. Clonar el repositorio:
   ```bash
   git clone https://github.com/usuario/proyecto-streaming.git
   cd proyecto-streaming

2. Ejecutar el programa usando CLion
   
## Uso
Al iniciar el programa, se cargarán las películas marcadas como "Ver más tarde" y recomendaciones basadas en "Likes".
Se puede realizar una búsqueda escribiendo una palabra clave, frase o etiqueta.
Se mostrarán los cinco resultados más relevantes con opción de ver más.
Al seleccionar una película, se podrá ver su sinopsis y marcarla como "Like" o "Ver más tarde".
Base de datos
El archivo CSV con la base de datos de películas puede descargarse desde el siguiente [enlace](https://drive.google.com/file/d/1iusSIbmXJW_OUBm6X7Ik593rgCGPCPza/view?usp=sharing)

## Autores
- ianjrodriguez21
- jucada2
  
## Licencia
Este proyecto está bajo la licencia MIT. Consulta el archivo LICENSE para más detalles.
