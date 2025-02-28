# Programación III: Proyecto Final (2025-0)

## Integrantes
- Ian Rodriguez Santayana
- Juan Carlos Paredes Alva

# Plataforma de Streaming

## Descripción

Este proyecto consiste en el desarrollo de una plataforma de streaming que permite la búsqueda y visualización de sinopsis de películas. Este proyecto utiliza un árbol organizado por el nombre de las películas extraída de una base de datos .CSV. Este programa permite tener 2 usuarios por defecto con nombre y contraseña: user1 para el usuario 1 y user2 para el otro. Cada usuario tiene la capacidad de agregar las películas que le interese en una liste de ver más tarde a través de una interfaz en la terminal.

## Características

- **Carga de base de datos**: El programa lee y procesa un archivo CSV que contiene información sobre películas. Esencialmente el titulo, los géneros y el id de estos
- **Búsqueda eficiente**: Se utiliza una estructura de Árbol para almacenar los títulos y sinopsis de las películas de forma alfabética, permitiendo búsquedas rápidas y eficientes donde cada nodo del árbol es una Película. 
- **Búsqueda flexible**:
  Para implementar la búsqueda de las películas existen diversas formas de hacerlo, Estas son buscando por una palabra, por genero o por una frase, en el caso de buscar por una frase de dos o más palabras, dicha combinación de tiene que existir en el titulo o en la sinopsis de alguna película de lo contrario no la encontrara. En caso se busque por una palabra incompleta por ejemplo bar en lugar de barco no existirá ningún problema y encontrara todas las películas que incluyan la secuencia bar en el titulo o la sinopsis dando prioridad a las películas que la contengan en el título.

- **Ver Mas Tarde**:
  Cada usuario va a contar con su lista propia para agregar películas en su lista de Ver mas tarde a través de la interfaz después de buscar la película en el árbol. En caso el usuario la desee agregar se incluirá en un vector de Películas la cual es la lista personal de cada usuario.
  

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
El programa al iniciar se va a pedir que inicie sesión, este programa cuenta con 2 usuarios user1 y user2, donde dicho nombre de usuario es el mismo para su contraseña. Al iniciar sesión se va a mostrar 5 opciones enumeradas de 1 al 5.
La opción 1 del menú principal permite al usuario buscar película por alguna palabra, frase o fragmento de una palabra para buscar en el árbol de películas. Después de buscar se mostrarán las primeras 5 películas más relevantes dándole prioridad a las que incluyan la búsqueda en el titulo y puede elegir entre 1 al 5 para agregar la película a ver más tarde, en caso no la encuentre puede presionar la tecla Enter para pasar a la siguiente pagina con 5 resultados y la ultima opción es la tecla s la cual se usa para salir de la búsqueda y volver al menú principal
La opción 2 del menú principal permite buscar las películas por género, pero en lugar de buscarlas en un árbol utiliza el patrón Composite en género para que en cada genero sea un grupo de películas las cuales después se muestran.
La opción 3 del menú permite visualizar la lista personal de cada usuario, es decir user1, cuenta con su lista que no puede ser accedida por user2 y viceversa.
La opción 4 se utiliza para cerrar el programa y terminar la ejecución devolviendo 0
Por último la opción 5 permite al usuario activo cerrar la sesión y volver al menú de inicio donde pude cambiar de user1 a user2 sin terminar el programa.

## Autores
- ianjrodriguez21
- jucada2
  
## Licencia
Este proyecto está bajo la licencia MIT. Consulta el archivo LICENSE para más detalles.

