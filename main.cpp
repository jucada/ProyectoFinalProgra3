#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;


class Componente {
public:
    virtual void agregar(Componente* pelicula) = 0;
    virtual void mostrar() const = 0;
    virtual bool contienePalabra(const string& palabra) const = 0;
    virtual int contarPalabra(const string& palabra) const = 0;
};

class Pelicula : public Componente {
private:
    int id;
    string titulo;
    string sinopsis;
    set<string> generos;

public:
    Pelicula(int id, string titulo, string sinopsis) {
        this->id = id;
        this->titulo = titulo;
        this->sinopsis = sinopsis;
    }

    void mostrar() const override {
        cout << "Titulo: " << titulo << endl;
        cout << "Sinopsis: " << sinopsis << endl;
        cout << "----------------------------------------------" << endl;
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

    void agregarGenero(string genero) {
        generos.insert(genero);
    }

    void agregar(Componente *pelicula) override{};
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
            cout<<"Error no es pelicula"<<endl;
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

        int pos = nodo->pelicula->getTitulo().find(palabra,0);
        while (pos != -1) {
            vecesTitulo++;
            pos = nodo->pelicula->getTitulo().find(palabra,pos+palabra.length());
        }

        prioridad = (vecesTitulo * 10) + vecesSinopsis;

        if (prioridad > 0) {
            resultados.push_back({prioridad,nodo->pelicula});
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
        vector<pair<int, Pelicula*>> resultados;
        buscarNodo(raiz, palabra, resultados);

        sort(resultados.begin(), resultados.end(), [](const pair<int, Pelicula*>& a, const pair<int, Pelicula*>& b) {
            return a.first > b.first;
        });

        cout << "Resultados de busqueda:" << endl;
        for (const auto& resultado : resultados) {
            resultado.second->mostrar();
        }
    }
};


int main() {
    BusquedaArbol arbol;
    Genero* accion = new Genero("Accion");

    Pelicula* p1 = new Pelicula(1, "Barco Fantasma", "Un barco sin tripulacion");
    p1->agregarGenero("Accion");

    accion->agregar(p1);
    arbol.insertar(p1);

    cout << "\nIngrese una palabra para buscar en titulo o sinopsis: ";
    string consulta;
    cin >> consulta;
    arbol.buscar(consulta);


    return 0;
}