#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>


using namespace std;

class Singleton {
private:
    static Singleton* instance;
    string usuario;
    string password;
    Singleton() {
        usuario = "user";
        password = "user";
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
        return (user == usuario && cont == password);
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

    string getNombre() const {
        return nombre;
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

void buscarGenero(const vector<Genero*>&generos, const string& nom) {
    bool exists = false;
    for (const auto& genero : generos) {
        if (genero->getNombre() == nom) {
            exists = true;
            genero->mostrar();
            break;
        }
    }
}


vector<string> splitCSV(const string& line) {
    vector<string> fields;
    bool inQuotes = false;
    string field;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field.push_back(c);
        }
    }
    fields.push_back(field);
    return fields;
}

vector<string> splitTags(const string& tagsStr) {
    vector<string> tags;
    stringstream ss(tagsStr);
    string tag;
    while(getline(ss, tag, ',')) {
        tags.push_back(tag);
    }
    return tags;
}

// Función para cargar películas desde el archivo CSV.
// Se asume que el archivo tiene la siguiente estructura en el encabezado:
// imdb_id,title,plot_synopsis,tags,split,synopsis_source
void cargarPeliculasDesdeCSV(const string& nombreArchivo, vector<Pelicula*>& peliculas) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    bool primeraLinea = true;
    while(getline(archivo, linea)) {
        if (primeraLinea) {
            primeraLinea = false;
            continue;
        }

        vector<string> campos = splitCSV(linea);
        // Verificar que se hayan leído al menos 6 campos.
        if (campos.size() < 6) {
            cout << "Línea con formato incorrecto: " << linea << endl;
            continue;
        }

        // Convertir el imdb_id en un entero (ignorando el prefijo "tt" si existe).
        int id;
        if (campos[0].substr(0, 2) == "tt") {
            id = stoi(campos[0].substr(2));
        } else {
            id = stoi(campos[0]);
        }

        string titulo = campos[1];
        string sinopsis = campos[2];
        string tagsStr = campos[3]; // Ejemplo: "cult, horror, gothic, murder, atmospheric"
        // Los campos 'split' y 'synopsis_source' se pueden ignorar o utilizar según tus necesidades.

        // Crear la película con id, título y sinopsis.
        Pelicula* nuevaPelicula = new Pelicula(id, titulo, sinopsis);

        // Separar y agregar cada tag como género.
        vector<string> tags = splitTags(tagsStr);
        for (const auto& tag : tags) {
            nuevaPelicula->agregarGenero(tag);
        }

        peliculas.push_back(nuevaPelicula);
    }

    archivo.close();
}

Singleton* Singleton::instance = nullptr;

int main() {
    Singleton* sesion = Singleton::getInstance();
    while (!sesion->InicioSesion()) {
        cout<<"Usario o password incorrecto. Intente de nuevo"<<endl;
    }



    // Vector para almacenar las películas cargadas desde el CSV.
    vector<Pelicula*> peliculas;
    cargarPeliculasDesdeCSV("mpst_full_data.csv", peliculas);
    vector<Genero*> generos;
    BusquedaArbol arbol;
    for (auto pelicula : peliculas) {
        arbol.insertar(pelicula);
        cout << pelicula->getTitulo() << endl;
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