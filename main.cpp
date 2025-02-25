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

    while (true) { // Bucle externo para permitir múltiples inicios de sesión
        while (!sesion->InicioSesion()) {
            cout << "Usuario o password incorrecto. Intente de nuevo." << endl;
        }

        BusquedaArbol arbol;
        vector<Genero*> generos;

        string archivoCSV = "mpst_full_data.csv";
        map<string, Pelicula*> peliculas = leerCSV("mpst_full_data.csv");

        for (const auto& [id, pelicula] : peliculas) {
            arbol.insertar(pelicula);
        }

        bool sesionActiva = true; // Controla si la sesión sigue activa

        while (sesionActiva) { // Bucle interno para el menú del usuario logueado
            int opcion;
            cout << "\nSeleccione una opcion de busqueda:" << endl;
            cout << "1 - Buscar por palabra en titulo o sinopsis" << endl;
            cout << "2 - Buscar por genero" << endl;
            cout << "3 - Ver lista de 'Ver mas tarde'" << endl;
            cout << "4 - Salir" << endl;
            cout << "5 - Cerrar sesion" << endl;
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
            } else if (opcion == 3) {
                sesion->mostrarVerMasTarde();
            } else if (opcion == 4) {
                return 0; // Cierra completamente el programa
            } else if (opcion == 5) {
                sesion->cerrarSesion();
                sesionActiva = false; // Salimos del bucle interno y volvemos a la pantalla de inicio de sesión
            } else {
                cout << "Opcion no valida." << endl;
            }
        }
    }
}
