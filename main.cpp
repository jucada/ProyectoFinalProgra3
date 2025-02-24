#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "clases.h"

using namespace std;


Singleton* Singleton::instance = nullptr;

int main() {
    Singleton* sesion = Singleton::getInstance();
    while (!sesion->InicioSesion()) {
        cout<<"Usario o password incorrecto. Intente de nuevo"<<endl;
    }

    BusquedaArbol arbol;
    vector<Genero*> generos;
    Genero* accion = new Genero("Accion");
    Genero* terror = new Genero("Terror");

    Pelicula* p1 = new Pelicula("1", "Barco Fantasma", "Un barco sin tripulacion");
    Pelicula* p2 = new Pelicula("2", "El Exorcista", "Una historia de posesion demoniaca");

    accion->agregar(p1);
    terror->agregar(p2);

    arbol.insertar(p1);
    arbol.insertar(p2);

    generos.push_back(accion);
    generos.push_back(terror);

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