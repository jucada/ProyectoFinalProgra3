#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <regex>
#include "clases.h"

using namespace std;


bool esIDValido(const string& texto) {
    return regex_match(texto, regex("^tt\\d{7}$"));
}

map<string, Pelicula> leerCSV(const string& archivoEntrada) {
    map<string, Pelicula> peliculas;
    ifstream archivo(archivoEntrada);

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivoEntrada << endl;
        return peliculas;
    }

    string linea;
    string id, titulo, genero, sinopsis, tags, split, synopsis_source;
    bool leyendoSinopsis = false;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string primerCampo;
        getline(ss, primerCampo, ',');

        // Si encontramos un ID válido, significa que es una nueva película
        if (esIDValido(primerCampo)) {
            // Guardamos la película anterior si existe
            if (!id.empty()) {
                peliculas[id] = Pelicula(id, titulo, sinopsis);
            }

            // Iniciamos una nueva película
            id = primerCampo;
            getline(ss, titulo, ',');

            // Iniciar sinopsis vacía, se llenará en el siguiente paso
            getline(ss, sinopsis);;
            leyendoSinopsis = true;
        } else if (leyendoSinopsis) {
            // Si NO es un ID, entonces es parte de la sinopsis o datos adicionales
            sinopsis += (sinopsis.empty() ? "" : "\n") + linea;
        }

        // Detectar si hemos llegado al final de la sinopsis y hay más datos en la línea
        if (sinopsis.find(".\",") != string::npos) {
            size_t pos = sinopsis.find(".\",");
            sinopsis = sinopsis.substr(0, pos);  // Tomar solo la parte de la sinopsis

            // Leer los demás campos después de la sinopsis
            ss.ignore(); // Ignorar la coma después de las comillas
            getline(ss, tags, ',');
            getline(ss, split, ',');
            getline(ss, synopsis_source, ',');

            leyendoSinopsis = false;  // Terminamos la sinopsis
        }
    }

    // Guardar la última película si existe
    if (!id.empty()) {
        peliculas[id] = Pelicula(id, titulo, sinopsis);
    }

    archivo.close();
    return peliculas;
}

// Función para imprimir las películas almacenadas
void imprimirPeliculas(const map<string, Pelicula>& peliculas) {
    for (const auto& [id, pelicula] : peliculas) {
        pelicula.mostrar();
    }
}



int main() {
    string archivoCSV = "mpst_full_data.csv";
    map<string, Pelicula> peliculas = leerCSV("mpst_full_data.csv");

    // Mostrar películas almacenadas
    cout << peliculas.size();
    return 0;
}
