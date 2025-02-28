#ifndef CLASES_H
#define CLASES_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <fstream>

using namespace std;

class Observador {
public:
    virtual void actualizar(const string& mensaje) = 0;
};

class Sujeto {
protected:
    vector<Observador*> observadores;
public:
    void agregarObservador(Observador* obs) {
        observadores.push_back(obs);
    }
    void removerObservador(Observador* obs) {
        observadores.erase(remove(observadores.begin(), observadores.end(), obs), observadores.end());
    }
    void notificarObservadores(const string& mensaje) {
        for (Observador* obs : observadores) {
            obs->actualizar(mensaje);
        }
    }
};

class UsuarioObservador : public Observador {
private:
    string nombre;
public:
    UsuarioObservador(const string& nombre) : nombre(nombre) {}
    void actualizar(const string& mensaje) override {
        cout << "Notificacion para " << nombre << ": " << mensaje << endl;
    }
};


class Componente {
public:
    virtual void agregar(Componente* pelicula) = 0;
    virtual void mostrar() const = 0;
    virtual bool contienePalabra(const string& palabra) const = 0;
    virtual int contarPalabra(const string& palabra) const = 0;
};

class Pelicula : public Componente {
private:
    string id;
    string titulo;
    string sinopsis;
    set<string> generos;

public:
    Pelicula() = default;
    Pelicula(string id, string titulo, string sinopsis) {
        this->id = id;
        this->titulo = titulo;
        this->sinopsis = sinopsis;
    }

    void mostrar() const override {
        cout << "Titulo: " << titulo << endl;
        cout << "ID: " << id << endl;
        cout << "Generos: ";
        for (set<string>::iterator it = generos.begin(); it != generos.end(); it++) {
            cout << *it << " ";
        }
        cout << "\n----------------------------------------------" << endl;
    }

    bool contienePalabra(const string& palabra) const override {
        return (titulo.find(palabra) != string::npos || sinopsis.find(palabra) != string::npos);
    }

    int contarPalabra(const string& palabra) const override {
        int contador = 0;
        size_t pos = sinopsis.find(palabra, 0);
        while (pos != string::npos) {
            contador++;
            pos = sinopsis.find(palabra, pos + palabra.length());
        }
        return contador;
    }

    string getTitulo() const {
        return titulo;
    }

    string getID() const {
        return id;
    }

    void agregarGenero(string genero) {
        generos.insert(genero);
    }

    bool tieneGenero(const string& genero) const {
        return generos.find(genero) != generos.end();
    }

    void agregar(Componente *pelicula) override { }
};

class Genero : public Componente {
private:
    string nombre;
    vector<Pelicula*> peliculas;

public:
    Genero(string nombre) {
        this->nombre = nombre;
    }

    void agregar(Componente* pelicula) override {
        Pelicula* peli = dynamic_cast<Pelicula*>(pelicula);
        if (peli) {
            peliculas.push_back(peli);
        } else {
            cout << "Error no es pelicula" << endl;
        }
    }

    void mostrar() const override {
        cout << "Genero: " << nombre << endl;
        cout << "--------------------------------------------" << endl;
        for (const auto& pelicula : peliculas) {
            pelicula->mostrar();
        }
    }

    bool contienePalabra(const string& palabra) const override {
        for (const auto& pelicula : peliculas) {
            if (pelicula->contienePalabra(palabra)) {
                return true;
            }
        }
        return false;
    }

    int contarPalabra(const string& palabra) const override {
        int total = 0;
        for (const auto& pelicula : peliculas) {
            total += pelicula->contarPalabra(palabra);
        }
        return total;
    }

    string getNombre() const {
        return nombre;
    }
};


class Singleton : public Sujeto {
private:
    static Singleton* instance;
    map<string, string> usuarios;
    map<string, set<Pelicula*>> listaVerMasTarde;
    string usuarioActual;

    Singleton() {
        usuarios["user1"] = "user1";
        usuarios["user2"] = "user2";
    }

public:
    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }

    bool InicioSesion() {
        string user, cont;
        cout << "Inicio sesion" << endl;
        cout << "Usuario: " << endl;
        cin >> user;
        cout << "Password: " << endl;
        cin >> cont;
        if (usuarios.find(user) != usuarios.end() && usuarios[user] == cont) {
            usuarioActual = user;
            cout << "Inicio de sesion exitoso. Bienvenido, " << usuarioActual << "!\n";
            return true;
        } else {
            cout << "Usuario o contraseña incorrectos.\n";
            return false;
        }
    }

    void cerrarSesion() {
        cout << "Cerrando sesion." << endl;
        usuarioActual.clear();
    }

    void guardarListaVerMasTarde() {
        if (usuarioActual.empty()) return;
        ofstream archivo(usuarioActual + ".txt");
        if (!archivo) {
            cout << "Error al guardar la lista de 'Ver más tarde'." << endl;
            return;
        }
        for (const auto& pelicula : listaVerMasTarde[usuarioActual]) {
            archivo << pelicula->getID() << endl;
        }
        archivo.close();
    }

    void cargarListaVerMasTarde(const map<string, Pelicula*>& peliculasDisponibles) {
        if (usuarioActual.empty()) return;
        ifstream archivo(usuarioActual + ".txt");
        if (!archivo) {
            return;
        }
        string titulo;
        while (getline(archivo, titulo)) {
            auto it = peliculasDisponibles.find(titulo);
            if (it != peliculasDisponibles.end()) {
                listaVerMasTarde[usuarioActual].insert(it->second);
            }
        }
        archivo.close();
    }

    void agregarVerMasTarde(Pelicula* pelicula) {
        if (listaVerMasTarde[usuarioActual].insert(pelicula).second) {
            cout << "Pelicula agregada a ver mas tarde: " << pelicula->getTitulo() << endl;
            guardarListaVerMasTarde();
            notificarObservadores("Se agregó la película '" + pelicula->getTitulo() + "' a la lista de 'Ver mas tarde' para el usuario " + usuarioActual);
        } else {
            cout << "Esta pelicula ya esta en tu lista" << endl;
        }
    }

    void mostrarVerMasTarde(const map<string, Pelicula*>& peliculasDisponibles) {
        cargarListaVerMasTarde(peliculasDisponibles);
        if (listaVerMasTarde[usuarioActual].empty()) {
            cout << "No tienes peliculas en tu lista de Ver mas tarde.\n";
            return;
        }
        cout << "\nTu lista de Ver mas tarde:\n";
        int i = 1;
        for (const auto& pelicula : listaVerMasTarde[usuarioActual]) {
            cout << i++ << ". " << pelicula->getTitulo() << endl;
        }
    }
};

struct Nodo {
    Pelicula* pelicula;
    Nodo *izq, *der;

    Nodo(Pelicula* pel) {
        this->pelicula = pel;
        izq = nullptr;
        der = nullptr;
    }
};

class BusquedaArbol {
private:
    Nodo* raiz;

    Nodo* insertarNodo(Nodo* nodo, Pelicula* pelicula) {
        if (nodo == nullptr) {
            return new Nodo(pelicula);
        }
        if (pelicula->getTitulo() < nodo->pelicula->getTitulo()) {
            nodo->izq = insertarNodo(nodo->izq, pelicula);
        } else {
            nodo->der = insertarNodo(nodo->der, pelicula);
        }
        return nodo;
    }

    void buscarNodo(Nodo* nodo, const string& palabra, vector<pair<int, Pelicula*>>& resultados) {
        if (nodo == nullptr) {
            return;
        }
        int prioridad = 0;
        int vecesTitulo = 0;
        int vecesSinopsis = nodo->pelicula->contarPalabra(palabra);
        size_t pos = nodo->pelicula->getTitulo().find(palabra, 0);
        while (pos != string::npos) {
            vecesTitulo++;
            pos = nodo->pelicula->getTitulo().find(palabra, pos + palabra.length());
        }
        prioridad = (vecesTitulo * 10) + vecesSinopsis;
        if (prioridad > 0) {
            resultados.push_back({prioridad, nodo->pelicula});
        }
        buscarNodo(nodo->izq, palabra, resultados);
        buscarNodo(nodo->der, palabra, resultados);
    }

public:
    BusquedaArbol() { raiz = nullptr; }

    void insertar(Pelicula* pelicula) {
        raiz = insertarNodo(raiz, pelicula);
    }

    void buscar(string palabra) {
        Singleton* sesion = Singleton::getInstance();
        vector<pair<int, Pelicula*>> resultados;
        buscarNodo(raiz, palabra, resultados);
        sort(resultados.begin(), resultados.end(), [](const pair<int, Pelicula*>& a, const pair<int, Pelicula*>& b) {
            return a.first > b.first;
        });
        cout << "Resultados de busqueda:" << endl;
        if (resultados.empty()) {
            cout << "No se encontraron resultados para: " << palabra << endl;
            return;
        }
        int totalResultados = resultados.size();
        int pagina = 0;
        while (pagina * 5 < totalResultados) {
            cout << "\nPagina " << (pagina + 1) << " de " << ((totalResultados + 4) / 5) << ":\n";
            for (int i = pagina * 5; i < min((pagina + 1) * 5, totalResultados); i++) {
                cout << (i % 5) + 1 << ". "; // Numeración de 1 a 5 en la página actual
                resultados[i].second->mostrar();
            }
            while (true) {
                cout << "Ingrese el numero (1-5) para agregar a 'Ver mas tarde', 'a' para avanzar o 's' para salir: ";
                string opcion;
                getline(cin, opcion);
                if (opcion == "a") {
                    break;
                } else if (opcion == "s") {
                    return;
                } else if (opcion >= "1" && opcion <= "5") {
                    int index = stoi(opcion) - 1;
                    int actualIndex = pagina * 5 + index;
                    if (actualIndex < totalResultados) {
                        sesion->agregarVerMasTarde(resultados[actualIndex].second);
                    } else {
                        cout << "Numero invalido.\n";
                    }
                } else {
                    cout << "Opcion no valida.\n";
                }
            }
            pagina++;
        }
    }
};

void buscarGenero(const map<string, Pelicula*>& peliculas, const string& genero) {
    Singleton* sesion = Singleton::getInstance();
    vector<Pelicula*> resultados;
    // Buscar películas que contengan el género
    for (const auto& [id, pelicula] : peliculas) {
        if (pelicula->tieneGenero(genero)) {
            resultados.push_back(pelicula);
        }
    }
    cout << "Resultados de busqueda:" << endl;
    if (resultados.empty()) {
        cout << "No se encontraron películas con el género '" << genero << "'.\n";
        return;
    }
    int totalResultados = resultados.size();
    int pagina = 0;
    while (pagina * 5 < totalResultados) {
        cout << "\nPeliculas del genero '" << genero << "' (Pagina " << (pagina + 1) << " de " << ((totalResultados + 4) / 5) << "):\n";
        for (int i = pagina * 5; i < min((pagina + 1) * 5, totalResultados); i++) {
            cout << (i % 5) + 1 << ". "; // Numeración de 1 a 5 en la página actual
            resultados[i]->mostrar();
        }
        while (true) {
            cout << "Ingrese el numero (1-5) para agregar a 'Ver mas tarde', 'a' para avanzar o 's' para salir: ";
            string opcion;
            getline(cin, opcion);
            if (opcion == "a") {
                break;
            } else if (opcion == "s") {
                return;
            } else if (opcion >= "1" && opcion <= "5") {
                int index = stoi(opcion) - 1;
                int actualIndex = pagina * 5 + index;
                if (actualIndex < totalResultados) {
                    sesion->agregarVerMasTarde(resultados[actualIndex]);
                } else {
                    cout << "Numero invalido.\n";
                }
            } else {
                cout << "Opcion no valida.\n";
            }
        }
        pagina++;
    }
}

#endif //CLASES_H
