#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "clases.h"
#include "procesamiento.h"

using namespace std;


Singleton* Singleton::instance = nullptr;

int main() {
    Singleton* sesion = Singleton::getInstance();
    while (!sesion->InicioSesion()) {
        cout<<"Usario o password incorrecto. Intente de nuevo"<<endl;
    }

    BusquedaArbol arbol;
    vector<Genero*> generos;


    string archivoCSV = "mpst_full_data.csv";
    map<string, Pelicula*> peliculas = leerCSV("mpst_full_data.csv");

    for (const auto& [id, pelicula] : peliculas) {
        arbol.insertar(pelicula);
    }


    int opcion;
    cout << "\nSeleccione una opcion de busqueda:" << endl;
    cout << "1 - Buscar por palabra en titulo o sinopsis" << endl;
    cout << "2 - Buscar por genero" << endl;
    cin >> opcion;
    cin.ignore();

    if (opcion == 1) {
        cout << "\nIngrese una palabra para buscar en titulo o sinopsis: ";
        string consulta;
        getline(cin, consulta);
        arbol.buscar(consulta);
    } else if (opcion == 2) {
        cout << "\nIngrese el genero a buscar: ";
        string genero;
        getline(cin, genero);
        buscarGenero(generos, genero);
    } else {
        cout << "Opcion no valida." << endl;
    }

    return 0;
}