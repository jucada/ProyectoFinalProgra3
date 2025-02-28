#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <regex>
#include <vector>
#include <thread>
#include <mutex>
#include "clases.h"

using namespace std;

mutex mtx;

int numHilos = 4;

bool esIDValido(const string& texto) {
    return regex_match(texto, regex("^tt\\d{7}$"));
}

void procesarParteCSV(const string& archivoEntrada, streampos inicio, streampos fin, map<string, Pelicula*>& peliculas) {
    ifstream archivo(archivoEntrada);
    if (!archivo.is_open()) return;

    archivo.seekg(inicio);

    string linea, id, titulo, sinopsis;
    set<string> generos;
    bool leyendoSinopsis = false;

    while (archivo.tellg() < fin && getline(archivo, linea)) {
        stringstream ss(linea);
        string primerCampo;
        getline(ss, primerCampo, ',');

        if (esIDValido(primerCampo)) {
            if (!id.empty()) {
                lock_guard<mutex> lock(mtx);
                Pelicula* pelicula = new Pelicula(id, titulo, sinopsis);
                for (const auto& genero : generos) {
                    pelicula->agregarGenero(genero);
                }
                peliculas[id] = pelicula;
            }
            id = primerCampo;
            getline(ss, titulo, ',');
            getline(ss, sinopsis);
            generos.clear();
            leyendoSinopsis = true;
        } else if (leyendoSinopsis) {
            sinopsis += (sinopsis.empty() ? "" : "\n") + linea;
        }

        if (sinopsis.find(".\",") != string::npos) {
            size_t pos = sinopsis.find(".\",");
            sinopsis = sinopsis.substr(0, pos);


            size_t pos2 = linea.find(".\",");
            string generosStr = linea.substr(pos2 + 3);
            if (!generosStr.empty() && generosStr[0] == '"') {
                size_t endQuote = generosStr.find("\",");
                if (endQuote != string::npos) {
                    generosStr = generosStr.substr(1, endQuote - 1);
                }
            } else {
                size_t firstComma = generosStr.find(",");
                if (firstComma != string::npos) {
                    generosStr = generosStr.substr(0, firstComma);
                }
            }
            stringstream ss(generosStr);
            string genero;
            while (getline(ss, genero, ',')) {
                genero.erase(0, genero.find_first_not_of(" "));
                genero.erase(genero.find_last_not_of(" ") + 1);

                if (!genero.empty()) {
                    generos.insert(genero);
                }
            }

            leyendoSinopsis = false;
        }
    }

    if (!id.empty()) {
        lock_guard<mutex> lock(mtx);
        Pelicula* pelicula = new Pelicula(id, titulo, sinopsis);
        for (const auto& genero : generos) {
            pelicula->agregarGenero(genero);
        }
        peliculas[id] = pelicula;
    }

    archivo.close();
}

map<string, Pelicula*> leerCSV(const string& archivoEntrada) {
    map<string, Pelicula*> peliculas;
    ifstream archivo(archivoEntrada, ios::ate | ios::binary);

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivoEntrada << endl;
        return peliculas;
    }

    streampos tamanoArchivo = archivo.tellg();
    archivo.close();

    streampos tamanoParte = tamanoArchivo / numHilos;

    vector<thread> hilos;
    vector<map<string, Pelicula*>> resultados(numHilos);

    auto inicioTiempo = chrono::high_resolution_clock::now();

    for (int i = 0; i < numHilos; i++) {
        streampos inicio = i * tamanoParte;
        streampos fin = (i == numHilos - 1) ? tamanoArchivo : static_cast<std::streampos>((i + 1) * tamanoParte);

        hilos.emplace_back(procesarParteCSV, archivoEntrada, inicio, fin, ref(resultados[i]));
    }

    for (auto& hilo : hilos) {
        hilo.join();
    }

    for (auto& parcial : resultados) {
        lock_guard<mutex> lock(mtx);
        peliculas.insert(parcial.begin(), parcial.end());
    }
    auto finTiempo = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(finTiempo - inicioTiempo).count();

    cout  << " Hilos: "<< numHilos << " Tiempo: " << duracion << " ms" << endl;
    cout << "Total de peliculas procesadas: " << peliculas.size() << endl;

    return peliculas;
}

void imprimirPeliculas(const map<string, Pelicula*>& peliculas) {
    for (const auto& [id, pelicula] : peliculas) {
        pelicula->mostrar();
    }
}

#endif //PROCESAMIENTO_H
