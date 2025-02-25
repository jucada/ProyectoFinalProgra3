//
// Created by dalev on 24/02/2025.
//

#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include <iostream>
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

map<string, Pelicula*> leerCSV(const string& archivoEntrada) {
    map<string, Pelicula*> peliculas;
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

        if (esIDValido(primerCampo)) {
            if (!id.empty()) {
                peliculas[id] = new Pelicula(id, titulo, sinopsis);
            }

            id = primerCampo;
            getline(ss, titulo, ',');

            getline(ss, sinopsis);;
            leyendoSinopsis = true;
        } else if (leyendoSinopsis) {
            sinopsis += (sinopsis.empty() ? "" : "\n") + linea;
        }

        if (sinopsis.find(".\",") != string::npos) {
            size_t pos = sinopsis.find(".\",");
            sinopsis = sinopsis.substr(0, pos);

            ss.ignore();
            getline(ss, tags, ',');
            getline(ss, split, ',');
            getline(ss, synopsis_source, ',');

            leyendoSinopsis = false;
        }
    }


    if (!id.empty()) {
        peliculas[id] = new Pelicula(id, titulo, sinopsis);
    }

    archivo.close();
    return peliculas;
}

void imprimirPeliculas(const map<string, Pelicula>& peliculas) {
    for (const auto& [id, pelicula] : peliculas) {
        pelicula.mostrar();
    }
}




#endif //PROCESAMIENTO_H
