#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// =====================================================
// ESTRUCTURAS
// =====================================================

struct NodoPixel {
    int fila;
    int columna;
    string color;
    NodoPixel* siguiente;
};

struct NodoCapa {
    int idCapa;
    NodoPixel* pixeles;
    NodoCapa* izquierda;
    NodoCapa* derecha;
};

struct NodoCapaImagen {
    NodoCapa* capa;
    NodoCapaImagen* siguiente;
};

struct NodoImagen {
    int idImagen;
    NodoCapaImagen* listaCapas;
    NodoImagen* siguiente;
    NodoImagen* anterior;
};

struct NodoImagenUsuario {
    NodoImagen* imagen;
    NodoImagenUsuario* siguiente;
};

struct NodoUsuario {
    string nombre;
    NodoImagenUsuario* listaImagenes;
    NodoUsuario* izquierda;
    NodoUsuario* derecha;
};

// =====================================================
// VARIABLES GLOBALES
// =====================================================

NodoCapa* raizCapas = NULL;
NodoImagen* listaImagenes = NULL;
NodoUsuario* raizUsuarios = NULL;

// =====================================================
// PROTOTIPOS
// =====================================================

int leerEntero();
string limpiarTexto(string texto);
void mostrarMenu();

void cargaMasivaCapas();
void cargaMasivaImagenes();
void cargaMasivaUsuarios();

NodoCapa* crearNodoCapa(int id);
NodoCapa* insertarCapa(NodoCapa* raiz, int id);
NodoCapa* buscarCapa(NodoCapa* raiz, int id);
void mostrarCapasInorden(NodoCapa* raiz);
void mostrarCapasPreorden(NodoCapa* raiz);
void mostrarCapasPostorden(NodoCapa* raiz);

NodoPixel* crearNodoPixel(int fila, int columna, string color);
void agregarPixelACapa(NodoCapa* capa, int fila, int columna, string color);
void mostrarPixelesDeCapa(NodoCapa* capa);
int contarPixelesCapa(NodoCapa* capa);

NodoImagen* crearNodoImagen(int id);
NodoImagen* buscarImagen(int id);
void insertarImagenCircular(NodoImagen* nueva);
void eliminarImagenCircular(int idImagen);
void agregarCapaAImagen(NodoImagen* imagen, NodoCapa* capa);
void mostrarListaImagenes();
void mostrarCapasDeImagen(NodoImagen* imagen);

NodoUsuario* crearNodoUsuario(string nombre);
NodoUsuario* insertarUsuario(NodoUsuario* raiz, string nombre);
NodoUsuario* buscarUsuario(NodoUsuario* raiz, string nombre);
NodoUsuario* encontrarMinUsuario(NodoUsuario* raiz);
NodoUsuario* eliminarUsuarioABB(NodoUsuario* raiz, string nombre);
void mostrarUsuariosInorden(NodoUsuario* raiz);
void agregarImagenAUsuario(NodoUsuario* usuario, NodoImagen* imagen);
void mostrarImagenesDeUsuario(NodoUsuario* usuario);
bool usuarioTieneImagen(NodoUsuario* usuario, int idImagen);
void quitarImagenDeListaUsuario(NodoUsuario* usuario, int idImagen);
void quitarImagenDeTodosLosUsuarios(NodoUsuario* raiz, int idImagen);

void generarImagenRecorridoLimitado();
void generarImagenPorLista();
void generarImagenPorCapa();
void generarImagenPorUsuario();

void calcularTamanoImagen(NodoImagen* imagen, int& maxFila, int& maxColumna);
void calcularTamanoCapa(NodoCapa* capa, int& maxFila, int& maxColumna);
void calcularTamanoArregloCapas(NodoCapa* capas[], int cantidad, int& maxFila, int& maxColumna);
void generarImagenHTML(NodoImagen* imagen);
void generarCapaHTML(NodoCapa* capa);
void generarImagenPorArregloCapas(NodoCapa* capas[], int cantidad, string nombreArchivo, string titulo);

void recolectarPreorden(NodoCapa* raiz, NodoCapa* capas[], int& contador, int limite);
void recolectarInorden(NodoCapa* raiz, NodoCapa* capas[], int& contador, int limite);
void recolectarPostorden(NodoCapa* raiz, NodoCapa* capas[], int& contador, int limite);

void menuCRUDUsuarios();
void modificarNombreUsuario();
void eliminarUsuarioDesdeCRUD();
void eliminarImagenDeUsuarioDesdeCRUD();

void menuCRUDImagenes();
void eliminarImagenDesdeCRUD();

void menuReportes();
void generarGraphvizArbolCapas();
void escribirArbolCapasDot(ofstream& archivo, NodoCapa* raiz);
void generarGraphvizArbolUsuarios();
void escribirArbolUsuariosDot(ofstream& archivo, NodoUsuario* raiz);
void generarGraphvizListaImagenes();
void generarGraphvizImagenCapas(int idImagen);

void probarArbolCapas();
void probarListaImagenes();
void probarArbolUsuarios();

// =====================================================
// FUNCIONES GENERALES
// =====================================================

int leerEntero() {
    int numero;
    while (!(cin >> numero)) {
        cout << "Entrada invalida. Ingrese un numero: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return numero;
}

string limpiarTexto(string texto) {
    string resultado = "";
    for (int i = 0; i < (int)texto.length(); i++) {
        char c = texto[i];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            resultado += c;
        }
    }
    return resultado;
}

int main() {
    int opcion = 0;

    do {
        mostrarMenu();
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();
        cout << endl;

        switch (opcion) {
            case 1: cargaMasivaCapas(); break;
            case 2: cargaMasivaImagenes(); break;
            case 3: cargaMasivaUsuarios(); break;
            case 4: generarImagenRecorridoLimitado(); break;
            case 5: generarImagenPorLista(); break;
            case 6: generarImagenPorCapa(); break;
            case 7: generarImagenPorUsuario(); break;
            case 8: menuCRUDUsuarios(); break;
            case 9: menuCRUDImagenes(); break;
            case 10: menuReportes(); break;
            case 11: probarArbolCapas(); break;
            case 12: probarListaImagenes(); break;
            case 13: probarArbolUsuarios(); break;
            case 14: cout << "Saliendo del programa..." << endl; break;
            default: cout << "Opcion invalida." << endl; break;
        }

        cout << endl;
    } while (opcion != 14);

    return 0;
}

void mostrarMenu() {
    cout << "========================================" << endl;
    cout << "     GENERADOR DE IMAGENES POR CAPAS    " << endl;
    cout << "========================================" << endl;
    cout << "1. Carga masiva de capas" << endl;
    cout << "2. Carga masiva de imagenes" << endl;
    cout << "3. Carga masiva de usuarios" << endl;
    cout << "4. Generar imagen por recorrido limitado" << endl;
    cout << "5. Generar imagen por lista de imagenes" << endl;
    cout << "6. Generar imagen por capa" << endl;
    cout << "7. Generar imagen por usuario" << endl;
    cout << "8. CRUD usuarios" << endl;
    cout << "9. CRUD imagenes" << endl;
    cout << "10. Graficar estado de la memoria" << endl;
    cout << "11. Probar arbol de capas" << endl;
    cout << "12. Probar lista de imagenes" << endl;
    cout << "13. Probar arbol de usuarios" << endl;
    cout << "14. Salir" << endl;
    cout << "========================================" << endl;
}

// =====================================================
// CARGA MASIVA
// =====================================================

void cargaMasivaCapas() {
    ifstream archivo("capas.cap");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo capas.cap" << endl;
        return;
    }

    string linea;
    NodoCapa* capaActual = NULL;
    int contadorCapas = 0;
    int contadorPixeles = 0;

    while (getline(archivo, linea)) {
        linea = limpiarTexto(linea);

        if (linea == "") {
            continue;
        }

        if (linea.find("{") != string::npos) {
            string idTexto = linea.substr(0, linea.find("{"));

            if (idTexto == "") {
                cout << "Advertencia: se encontro una capa sin ID antes de {" << endl;
                capaActual = NULL;
                continue;
            }

            int idCapa = stoi(idTexto);
            raizCapas = insertarCapa(raizCapas, idCapa);
            capaActual = buscarCapa(raizCapas, idCapa);
            contadorCapas++;
        }
        else if (linea.find("}") != string::npos) {
            capaActual = NULL;
        }
        else if (capaActual != NULL) {
            if (linea.find(";") != string::npos) {
                linea = linea.substr(0, linea.find(";"));
            }

            stringstream ss(linea);
            string filaTexto, columnaTexto, color;

            getline(ss, filaTexto, ',');
            getline(ss, columnaTexto, ',');
            getline(ss, color, ',');

            filaTexto = limpiarTexto(filaTexto);
            columnaTexto = limpiarTexto(columnaTexto);
            color = limpiarTexto(color);

            if (filaTexto != "" && columnaTexto != "" && color != "") {
                int fila = stoi(filaTexto);
                int columna = stoi(columnaTexto);
                agregarPixelACapa(capaActual, fila, columna, color);
                contadorPixeles++;
            }
        }
    }

    archivo.close();

    cout << "Carga masiva de capas finalizada correctamente." << endl;
    cout << "Capas procesadas: " << contadorCapas << endl;
    cout << "Pixeles procesados: " << contadorPixeles << endl;
}

void cargaMasivaImagenes() {
    ifstream archivo("imagenes.im");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo imagenes.im" << endl;
        return;
    }

    string linea;

    while (getline(archivo, linea)) {
        linea = limpiarTexto(linea);

        if (linea == "") {
            continue;
        }

        int posicionLlaveAbre = linea.find("{");
        int posicionLlaveCierra = linea.find("}");

        if (posicionLlaveAbre == string::npos || posicionLlaveCierra == string::npos) {
            continue;
        }

        string idImagenTexto = linea.substr(0, posicionLlaveAbre);

        if (idImagenTexto == "") {
            continue;
        }

        int idImagen = stoi(idImagenTexto);

        if (buscarImagen(idImagen) != NULL) {
            cout << "La imagen " << idImagen << " ya existe. No se agregara de nuevo." << endl;
            continue;
        }

        NodoImagen* nuevaImagen = crearNodoImagen(idImagen);

        string contenido = linea.substr(posicionLlaveAbre + 1, posicionLlaveCierra - posicionLlaveAbre - 1);
        stringstream ss(contenido);
        string idCapaTexto;

        while (getline(ss, idCapaTexto, ',')) {
            idCapaTexto = limpiarTexto(idCapaTexto);

            if (idCapaTexto != "") {
                int idCapa = stoi(idCapaTexto);
                NodoCapa* capaEncontrada = buscarCapa(raizCapas, idCapa);

                if (capaEncontrada != NULL) {
                    agregarCapaAImagen(nuevaImagen, capaEncontrada);
                } else {
                    cout << "Advertencia: La capa " << idCapa << " no existe. No se agrego a la imagen " << idImagen << "." << endl;
                }
            }
        }

        insertarImagenCircular(nuevaImagen);
        cout << "Imagen " << idImagen << " cargada correctamente." << endl;
    }

    archivo.close();
    cout << "Carga masiva de imagenes finalizada correctamente." << endl;
}

void cargaMasivaUsuarios() {
    ifstream archivo("usuarios.usr");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo usuarios.usr" << endl;
        return;
    }

    string linea;

    while (getline(archivo, linea)) {
        linea = limpiarTexto(linea);

        if (linea == "") {
            continue;
        }

        int posicionDosPuntos = linea.find(":");
        int posicionPuntoComa = linea.find(";");

        if (posicionDosPuntos == string::npos || posicionPuntoComa == string::npos) {
            continue;
        }

        string nombreUsuario = linea.substr(0, posicionDosPuntos);

        if (buscarUsuario(raizUsuarios, nombreUsuario) != NULL) {
            cout << "El usuario " << nombreUsuario << " ya existe. No se agregara de nuevo." << endl;
            continue;
        }

        raizUsuarios = insertarUsuario(raizUsuarios, nombreUsuario);
        NodoUsuario* usuarioActual = buscarUsuario(raizUsuarios, nombreUsuario);

        string contenido = linea.substr(posicionDosPuntos + 1, posicionPuntoComa - posicionDosPuntos - 1);
        stringstream ss(contenido);
        string idImagenTexto;

        while (getline(ss, idImagenTexto, ',')) {
            idImagenTexto = limpiarTexto(idImagenTexto);

            if (idImagenTexto != "") {
                int idImagen = stoi(idImagenTexto);
                NodoImagen* imagenEncontrada = buscarImagen(idImagen);

                if (imagenEncontrada != NULL) {
                    agregarImagenAUsuario(usuarioActual, imagenEncontrada);
                } else {
                    cout << "Advertencia: La imagen " << idImagen << " no existe. No se agrego al usuario " << nombreUsuario << "." << endl;
                }
            }
        }

        cout << "Usuario " << nombreUsuario << " cargado correctamente." << endl;
    }

    archivo.close();
    cout << "Carga masiva de usuarios finalizada correctamente." << endl;
}

// =====================================================
// ARBOL DE CAPAS
// =====================================================

NodoCapa* crearNodoCapa(int id) {
    NodoCapa* nuevo = new NodoCapa();
    nuevo->idCapa = id;
    nuevo->pixeles = NULL;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    return nuevo;
}

NodoCapa* insertarCapa(NodoCapa* raiz, int id) {
    if (raiz == NULL) {
        return crearNodoCapa(id);
    }

    if (id < raiz->idCapa) {
        raiz->izquierda = insertarCapa(raiz->izquierda, id);
    }
    else if (id > raiz->idCapa) {
        raiz->derecha = insertarCapa(raiz->derecha, id);
    }

    return raiz;
}

NodoCapa* buscarCapa(NodoCapa* raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }

    if (id == raiz->idCapa) {
        return raiz;
    }

    if (id < raiz->idCapa) {
        return buscarCapa(raiz->izquierda, id);
    } else {
        return buscarCapa(raiz->derecha, id);
    }
}

void mostrarCapasInorden(NodoCapa* raiz) {
    if (raiz != NULL) {
        mostrarCapasInorden(raiz->izquierda);
        cout << "Capa: " << raiz->idCapa << endl;
        mostrarCapasInorden(raiz->derecha);
    }
}

void mostrarCapasPreorden(NodoCapa* raiz) {
    if (raiz != NULL) {
        cout << "Capa: " << raiz->idCapa << endl;
        mostrarCapasPreorden(raiz->izquierda);
        mostrarCapasPreorden(raiz->derecha);
    }
}

void mostrarCapasPostorden(NodoCapa* raiz) {
    if (raiz != NULL) {
        mostrarCapasPostorden(raiz->izquierda);
        mostrarCapasPostorden(raiz->derecha);
        cout << "Capa: " << raiz->idCapa << endl;
    }
}

// =====================================================
// PIXELES
// =====================================================

NodoPixel* crearNodoPixel(int fila, int columna, string color) {
    NodoPixel* nuevo = new NodoPixel();
    nuevo->fila = fila;
    nuevo->columna = columna;
    nuevo->color = color;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarPixelACapa(NodoCapa* capa, int fila, int columna, string color) {
    if (capa == NULL) {
        return;
    }

    NodoPixel* nuevo = crearNodoPixel(fila, columna, color);

    // Se inserta al inicio para que archivos grandes carguen rapido.
    nuevo->siguiente = capa->pixeles;
    capa->pixeles = nuevo;
}

void mostrarPixelesDeCapa(NodoCapa* capa) {
    if (capa == NULL) {
        cout << "La capa no existe." << endl;
        return;
    }

    cout << "Pixeles de la capa " << capa->idCapa << ":" << endl;

    if (capa->pixeles == NULL) {
        cout << "Esta capa no tiene pixeles." << endl;
        return;
    }

    NodoPixel* actual = capa->pixeles;
    int contador = 0;

    while (actual != NULL) {
        cout << "Fila: " << actual->fila
             << " | Columna: " << actual->columna
             << " | Color: " << actual->color << endl;

        contador++;

        if (contador >= 200) {
            cout << "... Se muestran solo los primeros 200 pixeles para no saturar la consola." << endl;
            break;
        }

        actual = actual->siguiente;
    }
}

int contarPixelesCapa(NodoCapa* capa) {
    if (capa == NULL) {
        return 0;
    }

    int contador = 0;
    NodoPixel* actual = capa->pixeles;

    while (actual != NULL) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}

// =====================================================
// LISTA CIRCULAR DOBLE DE IMAGENES
// =====================================================

NodoImagen* crearNodoImagen(int id) {
    NodoImagen* nueva = new NodoImagen();
    nueva->idImagen = id;
    nueva->listaCapas = NULL;
    nueva->siguiente = NULL;
    nueva->anterior = NULL;
    return nueva;
}

NodoImagen* buscarImagen(int id) {
    if (listaImagenes == NULL) {
        return NULL;
    }

    NodoImagen* actual = listaImagenes;

    do {
        if (actual->idImagen == id) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != listaImagenes);

    return NULL;
}

void insertarImagenCircular(NodoImagen* nueva) {
    if (nueva == NULL) {
        return;
    }

    if (listaImagenes == NULL) {
        listaImagenes = nueva;
        nueva->siguiente = nueva;
        nueva->anterior = nueva;
    } else {
        NodoImagen* ultimo = listaImagenes->anterior;
        ultimo->siguiente = nueva;
        nueva->anterior = ultimo;
        nueva->siguiente = listaImagenes;
        listaImagenes->anterior = nueva;
    }
}

void eliminarImagenCircular(int idImagen) {
    if (listaImagenes == NULL) {
        return;
    }

    NodoImagen* actual = listaImagenes;

    do {
        if (actual->idImagen == idImagen) {
            if (actual->siguiente == actual && actual->anterior == actual) {
                listaImagenes = NULL;
            } else {
                actual->anterior->siguiente = actual->siguiente;
                actual->siguiente->anterior = actual->anterior;

                if (actual == listaImagenes) {
                    listaImagenes = actual->siguiente;
                }
            }

            delete actual;
            return;
        }

        actual = actual->siguiente;
    } while (actual != listaImagenes);
}

void agregarCapaAImagen(NodoImagen* imagen, NodoCapa* capa) {
    if (imagen == NULL || capa == NULL) {
        return;
    }

    NodoCapaImagen* nuevo = new NodoCapaImagen();
    nuevo->capa = capa;
    nuevo->siguiente = NULL;

    if (imagen->listaCapas == NULL) {
        imagen->listaCapas = nuevo;
    } else {
        NodoCapaImagen* actual = imagen->listaCapas;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }
}

void mostrarListaImagenes() {
    if (listaImagenes == NULL) {
        cout << "No hay imagenes cargadas." << endl;
        return;
    }

    NodoImagen* actual = listaImagenes;
    cout << "Imagenes cargadas:" << endl;

    do {
        cout << "Imagen: " << actual->idImagen << endl;
        actual = actual->siguiente;
    } while (actual != listaImagenes);
}

void mostrarCapasDeImagen(NodoImagen* imagen) {
    if (imagen == NULL) {
        cout << "La imagen no existe." << endl;
        return;
    }

    cout << "Capas de la imagen " << imagen->idImagen << ":" << endl;

    if (imagen->listaCapas == NULL) {
        cout << "Esta imagen no tiene capas." << endl;
        return;
    }

    NodoCapaImagen* actual = imagen->listaCapas;

    while (actual != NULL) {
        cout << "Capa: " << actual->capa->idCapa << endl;
        actual = actual->siguiente;
    }
}

// =====================================================
// ARBOL DE USUARIOS
// =====================================================

NodoUsuario* crearNodoUsuario(string nombre) {
    NodoUsuario* nuevo = new NodoUsuario();
    nuevo->nombre = nombre;
    nuevo->listaImagenes = NULL;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    return nuevo;
}

NodoUsuario* insertarUsuario(NodoUsuario* raiz, string nombre) {
    if (raiz == NULL) {
        return crearNodoUsuario(nombre);
    }

    if (nombre < raiz->nombre) {
        raiz->izquierda = insertarUsuario(raiz->izquierda, nombre);
    }
    else if (nombre > raiz->nombre) {
        raiz->derecha = insertarUsuario(raiz->derecha, nombre);
    }

    return raiz;
}

NodoUsuario* buscarUsuario(NodoUsuario* raiz, string nombre) {
    if (raiz == NULL) {
        return NULL;
    }

    if (nombre == raiz->nombre) {
        return raiz;
    }

    if (nombre < raiz->nombre) {
        return buscarUsuario(raiz->izquierda, nombre);
    } else {
        return buscarUsuario(raiz->derecha, nombre);
    }
}

NodoUsuario* encontrarMinUsuario(NodoUsuario* raiz) {
    if (raiz == NULL) {
        return NULL;
    }

    while (raiz->izquierda != NULL) {
        raiz = raiz->izquierda;
    }

    return raiz;
}

NodoUsuario* eliminarUsuarioABB(NodoUsuario* raiz, string nombre) {
    if (raiz == NULL) {
        return NULL;
    }

    if (nombre < raiz->nombre) {
        raiz->izquierda = eliminarUsuarioABB(raiz->izquierda, nombre);
    }
    else if (nombre > raiz->nombre) {
        raiz->derecha = eliminarUsuarioABB(raiz->derecha, nombre);
    }
    else {
        if (raiz->izquierda == NULL && raiz->derecha == NULL) {
            delete raiz;
            return NULL;
        }
        else if (raiz->izquierda == NULL) {
            NodoUsuario* temp = raiz->derecha;
            delete raiz;
            return temp;
        }
        else if (raiz->derecha == NULL) {
            NodoUsuario* temp = raiz->izquierda;
            delete raiz;
            return temp;
        }
        else {
            NodoUsuario* sucesor = encontrarMinUsuario(raiz->derecha);
            raiz->nombre = sucesor->nombre;
            raiz->listaImagenes = sucesor->listaImagenes;
            raiz->derecha = eliminarUsuarioABB(raiz->derecha, sucesor->nombre);
        }
    }

    return raiz;
}

void mostrarUsuariosInorden(NodoUsuario* raiz) {
    if (raiz != NULL) {
        mostrarUsuariosInorden(raiz->izquierda);
        cout << "Usuario: " << raiz->nombre << endl;
        mostrarUsuariosInorden(raiz->derecha);
    }
}

void agregarImagenAUsuario(NodoUsuario* usuario, NodoImagen* imagen) {
    if (usuario == NULL || imagen == NULL) {
        return;
    }

    NodoImagenUsuario* nuevo = new NodoImagenUsuario();
    nuevo->imagen = imagen;
    nuevo->siguiente = NULL;

    if (usuario->listaImagenes == NULL) {
        usuario->listaImagenes = nuevo;
    } else {
        NodoImagenUsuario* actual = usuario->listaImagenes;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }
}

void mostrarImagenesDeUsuario(NodoUsuario* usuario) {
    if (usuario == NULL) {
        cout << "El usuario no existe." << endl;
        return;
    }

    cout << "Imagenes del usuario " << usuario->nombre << ":" << endl;

    if (usuario->listaImagenes == NULL) {
        cout << "Este usuario no tiene imagenes." << endl;
        return;
    }

    NodoImagenUsuario* actual = usuario->listaImagenes;

    while (actual != NULL) {
        cout << "Imagen: " << actual->imagen->idImagen << endl;
        actual = actual->siguiente;
    }
}

bool usuarioTieneImagen(NodoUsuario* usuario, int idImagen) {
    if (usuario == NULL) {
        return false;
    }

    NodoImagenUsuario* actual = usuario->listaImagenes;

    while (actual != NULL) {
        if (actual->imagen->idImagen == idImagen) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

void quitarImagenDeListaUsuario(NodoUsuario* usuario, int idImagen) {
    if (usuario == NULL || usuario->listaImagenes == NULL) {
        return;
    }

    NodoImagenUsuario* actual = usuario->listaImagenes;
    NodoImagenUsuario* anterior = NULL;

    while (actual != NULL) {
        if (actual->imagen->idImagen == idImagen) {
            if (anterior == NULL) {
                usuario->listaImagenes = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            delete actual;
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    }
}

void quitarImagenDeTodosLosUsuarios(NodoUsuario* raiz, int idImagen) {
    if (raiz == NULL) {
        return;
    }

    quitarImagenDeTodosLosUsuarios(raiz->izquierda, idImagen);
    quitarImagenDeListaUsuario(raiz, idImagen);
    quitarImagenDeTodosLosUsuarios(raiz->derecha, idImagen);
}

// =====================================================
// GENERACION DE IMAGENES DINAMICAS
// =====================================================

void calcularTamanoImagen(NodoImagen* imagen, int& maxFila, int& maxColumna) {
    maxFila = 0;
    maxColumna = 0;

    if (imagen == NULL) {
        return;
    }

    NodoCapaImagen* capaActual = imagen->listaCapas;

    while (capaActual != NULL) {
        NodoPixel* pixelActual = capaActual->capa->pixeles;

        while (pixelActual != NULL) {
            if (pixelActual->fila > maxFila) {
                maxFila = pixelActual->fila;
            }

            if (pixelActual->columna > maxColumna) {
                maxColumna = pixelActual->columna;
            }

            pixelActual = pixelActual->siguiente;
        }

        capaActual = capaActual->siguiente;
    }

    if (maxFila < 10) maxFila = 10;
    if (maxColumna < 10) maxColumna = 10;
}

void calcularTamanoCapa(NodoCapa* capa, int& maxFila, int& maxColumna) {
    maxFila = 0;
    maxColumna = 0;

    if (capa == NULL) {
        return;
    }

    NodoPixel* pixelActual = capa->pixeles;

    while (pixelActual != NULL) {
        if (pixelActual->fila > maxFila) {
            maxFila = pixelActual->fila;
        }

        if (pixelActual->columna > maxColumna) {
            maxColumna = pixelActual->columna;
        }

        pixelActual = pixelActual->siguiente;
    }

    if (maxFila < 10) maxFila = 10;
    if (maxColumna < 10) maxColumna = 10;
}

void calcularTamanoArregloCapas(NodoCapa* capas[], int cantidad, int& maxFila, int& maxColumna) {
    maxFila = 0;
    maxColumna = 0;

    for (int i = 0; i < cantidad; i++) {
        NodoPixel* pixelActual = capas[i]->pixeles;

        while (pixelActual != NULL) {
            if (pixelActual->fila > maxFila) {
                maxFila = pixelActual->fila;
            }

            if (pixelActual->columna > maxColumna) {
                maxColumna = pixelActual->columna;
            }

            pixelActual = pixelActual->siguiente;
        }
    }

    if (maxFila < 10) maxFila = 10;
    if (maxColumna < 10) maxColumna = 10;
}

void generarImagenHTML(NodoImagen* imagen) {
    if (imagen == NULL) {
        cout << "No se puede generar la imagen porque no existe." << endl;
        return;
    }

    int filas;
    int columnas;

    calcularTamanoImagen(imagen, filas, columnas);

    string** matriz = new string*[filas];

    for (int i = 0; i < filas; i++) {
        matriz[i] = new string[columnas];

        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = "#ffffff";
        }
    }

    NodoCapaImagen* capaActual = imagen->listaCapas;

    while (capaActual != NULL) {
        NodoPixel* pixelActual = capaActual->capa->pixeles;

        while (pixelActual != NULL) {
            int fila = pixelActual->fila;
            int columna = pixelActual->columna;

            if (fila >= 1 && fila <= filas && columna >= 1 && columna <= columnas) {
                matriz[fila - 1][columna - 1] = pixelActual->color;
            }

            pixelActual = pixelActual->siguiente;
        }

        capaActual = capaActual->siguiente;
    }

    ofstream archivo("imagen_generada.html");

    if (!archivo.is_open()) {
        cout << "No se pudo crear imagen_generada.html" << endl;
        return;
    }

    archivo << "<!DOCTYPE html>" << endl;
    archivo << "<html><head><meta charset='UTF-8'><title>Imagen generada</title>" << endl;
    archivo << "<style>" << endl;
    archivo << "body{font-family:Arial;padding:20px;}" << endl;
    archivo << ".contenedor{overflow:auto;max-width:100%;max-height:85vh;border:1px solid #ddd;}" << endl;
    archivo << "table{border-collapse:collapse;}" << endl;
    archivo << "td{width:4px;height:4px;min-width:4px;min-height:4px;padding:0;border:0;}" << endl;
    archivo << "</style></head><body>" << endl;
    archivo << "<h1>Imagen generada</h1>" << endl;
    archivo << "<p>ID de imagen: " << imagen->idImagen << "</p>" << endl;
    archivo << "<p>Tamano: " << filas << " x " << columnas << "</p>" << endl;
    archivo << "<div class='contenedor'><table>" << endl;

    for (int i = 0; i < filas; i++) {
        archivo << "<tr>";

        for (int j = 0; j < columnas; j++) {
            archivo << "<td style='background-color:" << matriz[i][j] << ";'></td>";
        }

        archivo << "</tr>" << endl;
    }

    archivo << "</table></div></body></html>" << endl;
    archivo.close();

    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }

    delete[] matriz;

    cout << "Imagen generada correctamente." << endl;
    cout << "Archivo creado: imagen_generada.html" << endl;
}

void generarCapaHTML(NodoCapa* capa) {
    if (capa == NULL) {
        cout << "No se puede generar la capa porque no existe." << endl;
        return;
    }

    int filas;
    int columnas;

    calcularTamanoCapa(capa, filas, columnas);

    string** matriz = new string*[filas];

    for (int i = 0; i < filas; i++) {
        matriz[i] = new string[columnas];

        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = "#ffffff";
        }
    }

    NodoPixel* pixelActual = capa->pixeles;

    while (pixelActual != NULL) {
        int fila = pixelActual->fila;
        int columna = pixelActual->columna;

        if (fila >= 1 && fila <= filas && columna >= 1 && columna <= columnas) {
            matriz[fila - 1][columna - 1] = pixelActual->color;
        }

        pixelActual = pixelActual->siguiente;
    }

    ofstream archivo("capa_generada.html");

    if (!archivo.is_open()) {
        cout << "No se pudo crear capa_generada.html" << endl;
        return;
    }

    archivo << "<!DOCTYPE html>" << endl;
    archivo << "<html><head><meta charset='UTF-8'><title>Capa generada</title>" << endl;
    archivo << "<style>" << endl;
    archivo << "body{font-family:Arial;padding:20px;}" << endl;
    archivo << ".contenedor{overflow:auto;max-width:100%;max-height:85vh;border:1px solid #ddd;}" << endl;
    archivo << "table{border-collapse:collapse;}" << endl;
    archivo << "td{width:4px;height:4px;min-width:4px;min-height:4px;padding:0;border:0;}" << endl;
    archivo << "</style></head><body>" << endl;
    archivo << "<h1>Capa generada</h1>" << endl;
    archivo << "<p>ID de capa: " << capa->idCapa << "</p>" << endl;
    archivo << "<p>Tamano: " << filas << " x " << columnas << "</p>" << endl;
    archivo << "<div class='contenedor'><table>" << endl;

    for (int i = 0; i < filas; i++) {
        archivo << "<tr>";

        for (int j = 0; j < columnas; j++) {
            archivo << "<td style='background-color:" << matriz[i][j] << ";'></td>";
        }

        archivo << "</tr>" << endl;
    }

    archivo << "</table></div></body></html>" << endl;
    archivo.close();

    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }

    delete[] matriz;

    cout << "Capa generada correctamente." << endl;
    cout << "Archivo creado: capa_generada.html" << endl;
}

void generarImagenPorArregloCapas(NodoCapa* capas[], int cantidad, string nombreArchivo, string titulo) {
    int filas;
    int columnas;

    calcularTamanoArregloCapas(capas, cantidad, filas, columnas);

    string** matriz = new string*[filas];

    for (int i = 0; i < filas; i++) {
        matriz[i] = new string[columnas];

        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = "#ffffff";
        }
    }

    for (int i = 0; i < cantidad; i++) {
        NodoPixel* pixelActual = capas[i]->pixeles;

        while (pixelActual != NULL) {
            int fila = pixelActual->fila;
            int columna = pixelActual->columna;

            if (fila >= 1 && fila <= filas && columna >= 1 && columna <= columnas) {
                matriz[fila - 1][columna - 1] = pixelActual->color;
            }

            pixelActual = pixelActual->siguiente;
        }
    }

    ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open()) {
        cout << "No se pudo crear " << nombreArchivo << endl;
        return;
    }

    archivo << "<!DOCTYPE html>" << endl;
    archivo << "<html><head><meta charset='UTF-8'><title>" << titulo << "</title>" << endl;
    archivo << "<style>" << endl;
    archivo << "body{font-family:Arial;padding:20px;}" << endl;
    archivo << ".contenedor{overflow:auto;max-width:100%;max-height:85vh;border:1px solid #ddd;}" << endl;
    archivo << "table{border-collapse:collapse;}" << endl;
    archivo << "td{width:4px;height:4px;min-width:4px;min-height:4px;padding:0;border:0;}" << endl;
    archivo << "</style></head><body>" << endl;
    archivo << "<h1>" << titulo << "</h1>" << endl;
    archivo << "<p>Capas utilizadas: ";

    for (int i = 0; i < cantidad; i++) {
        archivo << capas[i]->idCapa;
        if (i < cantidad - 1) {
            archivo << ", ";
        }
    }

    archivo << "</p>" << endl;
    archivo << "<p>Tamano: " << filas << " x " << columnas << "</p>" << endl;
    archivo << "<div class='contenedor'><table>" << endl;

    for (int i = 0; i < filas; i++) {
        archivo << "<tr>";

        for (int j = 0; j < columnas; j++) {
            archivo << "<td style='background-color:" << matriz[i][j] << ";'></td>";
        }

        archivo << "</tr>" << endl;
    }

    archivo << "</table></div></body></html>" << endl;
    archivo.close();

    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }

    delete[] matriz;

    cout << "Imagen por recorrido generada correctamente." << endl;
    cout << "Archivo creado: " << nombreArchivo << endl;
}

// =====================================================
// RECORRIDOS Y GENERACION
// =====================================================

void recolectarPreorden(NodoCapa* raiz, NodoCapa* capas[], int& contador, int limite) {
    if (raiz == NULL || contador >= limite) {
        return;
    }

    capas[contador] = raiz;
    contador++;

    recolectarPreorden(raiz->izquierda, capas, contador, limite);
    recolectarPreorden(raiz->derecha, capas, contador, limite);
}

void recolectarInorden(NodoCapa* raiz, NodoCapa* capas[], int& contador, int limite) {
    if (raiz == NULL || contador >= limite) {
        return;
    }

    recolectarInorden(raiz->izquierda, capas, contador, limite);

    if (contador < limite) {
        capas[contador] = raiz;
        contador++;
    }

    recolectarInorden(raiz->derecha, capas, contador, limite);
}

void recolectarPostorden(NodoCapa* raiz, NodoCapa* capas[], int& contador, int limite) {
    if (raiz == NULL || contador >= limite) {
        return;
    }

    recolectarPostorden(raiz->izquierda, capas, contador, limite);
    recolectarPostorden(raiz->derecha, capas, contador, limite);

    if (contador < limite) {
        capas[contador] = raiz;
        contador++;
    }
}

void generarImagenRecorridoLimitado() {
    int cantidadCapas;
    int tipoRecorrido;

    cout << "Ingrese el numero de capas a utilizar: ";
    cantidadCapas = leerEntero();

    if (cantidadCapas <= 0) {
        cout << "La cantidad debe ser mayor a cero." << endl;
        return;
    }

    cout << "Seleccione el tipo de recorrido:" << endl;
    cout << "1. Preorden" << endl;
    cout << "2. Inorden" << endl;
    cout << "3. Postorden" << endl;
    cout << "Opcion: ";
    tipoRecorrido = leerEntero();

    NodoCapa* capas[1000];
    int contador = 0;

    if (tipoRecorrido == 1) {
        recolectarPreorden(raizCapas, capas, contador, cantidadCapas);
    }
    else if (tipoRecorrido == 2) {
        recolectarInorden(raizCapas, capas, contador, cantidadCapas);
    }
    else if (tipoRecorrido == 3) {
        recolectarPostorden(raizCapas, capas, contador, cantidadCapas);
    }
    else {
        cout << "Tipo de recorrido invalido." << endl;
        return;
    }

    if (contador == 0) {
        cout << "No hay capas cargadas." << endl;
        return;
    }

    cout << "Capas utilizadas:" << endl;

    for (int i = 0; i < contador; i++) {
        cout << "Capa: " << capas[i]->idCapa << endl;
    }

    generarImagenPorArregloCapas(capas, contador, "imagen_recorrido.html", "Imagen por recorrido limitado");
}

void generarImagenPorLista() {
    int idImagen;

    cout << "Ingrese el id de la imagen que desea generar: ";
    idImagen = leerEntero();

    NodoImagen* imagen = buscarImagen(idImagen);

    if (imagen == NULL) {
        cout << "La imagen " << idImagen << " no existe." << endl;
        return;
    }

    generarImagenHTML(imagen);
}

void generarImagenPorCapa() {
    int idCapa;

    cout << "Ingrese el id de la capa que desea generar: ";
    idCapa = leerEntero();

    NodoCapa* capa = buscarCapa(raizCapas, idCapa);

    if (capa == NULL) {
        cout << "La capa " << idCapa << " no existe." << endl;
        return;
    }

    generarCapaHTML(capa);
}

void generarImagenPorUsuario() {
    string nombreUsuario;

    cout << "Ingrese el nombre del usuario: ";
    cin >> nombreUsuario;

    NodoUsuario* usuario = buscarUsuario(raizUsuarios, nombreUsuario);

    if (usuario == NULL) {
        cout << "El usuario " << nombreUsuario << " no existe." << endl;
        return;
    }

    if (usuario->listaImagenes == NULL) {
        cout << "El usuario " << nombreUsuario << " no tiene imagenes." << endl;
        return;
    }

    mostrarImagenesDeUsuario(usuario);

    int idImagen;

    cout << "Ingrese el id de la imagen que desea generar: ";
    idImagen = leerEntero();

    NodoImagenUsuario* actual = usuario->listaImagenes;
    NodoImagen* imagenSeleccionada = NULL;

    while (actual != NULL) {
        if (actual->imagen->idImagen == idImagen) {
            imagenSeleccionada = actual->imagen;
            break;
        }

        actual = actual->siguiente;
    }

    if (imagenSeleccionada == NULL) {
        cout << "La imagen " << idImagen << " no pertenece al usuario " << nombreUsuario << "." << endl;
        return;
    }

    generarImagenHTML(imagenSeleccionada);
}

// =====================================================
// CRUD USUARIOS
// =====================================================

void menuCRUDUsuarios() {
    int opcion = 0;

    do {
        cout << "==============================" << endl;
        cout << "        CRUD USUARIOS         " << endl;
        cout << "==============================" << endl;
        cout << "1. Agregar usuario" << endl;
        cout << "2. Buscar usuario" << endl;
        cout << "3. Mostrar usuarios" << endl;
        cout << "4. Mostrar imagenes de un usuario" << endl;
        cout << "5. Agregar imagen a un usuario" << endl;
        cout << "6. Eliminar imagen de un usuario" << endl;
        cout << "7. Modificar nombre de usuario" << endl;
        cout << "8. Eliminar usuario" << endl;
        cout << "9. Regresar al menu principal" << endl;
        cout << "==============================" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();

        cout << endl;

        if (opcion == 1) {
            string nombre;

            cout << "Ingrese el nombre del nuevo usuario: ";
            cin >> nombre;

            if (buscarUsuario(raizUsuarios, nombre) != NULL) {
                cout << "El usuario ya existe." << endl;
            } else {
                raizUsuarios = insertarUsuario(raizUsuarios, nombre);
                cout << "Usuario agregado correctamente." << endl;
            }
        }
        else if (opcion == 2) {
            string nombre;

            cout << "Ingrese el nombre del usuario a buscar: ";
            cin >> nombre;

            if (buscarUsuario(raizUsuarios, nombre) != NULL) {
                cout << "El usuario " << nombre << " si existe." << endl;
            } else {
                cout << "El usuario " << nombre << " no existe." << endl;
            }
        }
        else if (opcion == 3) {
            cout << "Usuarios guardados en el arbol:" << endl;

            if (raizUsuarios == NULL) {
                cout << "No hay usuarios cargados." << endl;
            } else {
                mostrarUsuariosInorden(raizUsuarios);
            }
        }
        else if (opcion == 4) {
            string nombre;

            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;

            mostrarImagenesDeUsuario(buscarUsuario(raizUsuarios, nombre));
        }
        else if (opcion == 5) {
            string nombre;
            int idImagen;

            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;

            NodoUsuario* usuario = buscarUsuario(raizUsuarios, nombre);

            if (usuario == NULL) {
                cout << "El usuario no existe." << endl;
            } else {
                mostrarListaImagenes();

                cout << "Ingrese el id de la imagen que desea agregar al usuario: ";
                idImagen = leerEntero();

                NodoImagen* imagen = buscarImagen(idImagen);

                if (imagen == NULL) {
                    cout << "La imagen no existe." << endl;
                }
                else if (usuarioTieneImagen(usuario, idImagen)) {
                    cout << "El usuario ya tiene asignada esa imagen." << endl;
                }
                else {
                    agregarImagenAUsuario(usuario, imagen);
                    cout << "Imagen agregada correctamente al usuario." << endl;
                }
            }
        }
        else if (opcion == 6) {
            eliminarImagenDeUsuarioDesdeCRUD();
        }
        else if (opcion == 7) {
            modificarNombreUsuario();
        }
        else if (opcion == 8) {
            eliminarUsuarioDesdeCRUD();
        }
        else if (opcion == 9) {
            cout << "Regresando al menu principal..." << endl;
        }
        else {
            cout << "Opcion invalida." << endl;
        }

        cout << endl;
    } while (opcion != 9);
}

void modificarNombreUsuario() {
    string nombreActual;
    string nombreNuevo;

    cout << "Ingrese el nombre del usuario a modificar: ";
    cin >> nombreActual;

    NodoUsuario* usuarioActual = buscarUsuario(raizUsuarios, nombreActual);

    if (usuarioActual == NULL) {
        cout << "El usuario no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo nombre del usuario: ";
    cin >> nombreNuevo;

    if (buscarUsuario(raizUsuarios, nombreNuevo) != NULL) {
        cout << "Ya existe un usuario con ese nombre." << endl;
        return;
    }

    NodoImagenUsuario* listaGuardada = usuarioActual->listaImagenes;

    raizUsuarios = eliminarUsuarioABB(raizUsuarios, nombreActual);
    raizUsuarios = insertarUsuario(raizUsuarios, nombreNuevo);

    NodoUsuario* usuarioNuevo = buscarUsuario(raizUsuarios, nombreNuevo);

    if (usuarioNuevo != NULL) {
        usuarioNuevo->listaImagenes = listaGuardada;
    }

    cout << "Usuario modificado correctamente." << endl;
}

void eliminarUsuarioDesdeCRUD() {
    string nombre;

    cout << "Ingrese el nombre del usuario a eliminar: ";
    cin >> nombre;

    if (buscarUsuario(raizUsuarios, nombre) == NULL) {
        cout << "El usuario no existe." << endl;
        return;
    }

    raizUsuarios = eliminarUsuarioABB(raizUsuarios, nombre);
    cout << "Usuario eliminado correctamente." << endl;
}

void eliminarImagenDeUsuarioDesdeCRUD() {
    string nombre;
    int idImagen;

    cout << "Ingrese el nombre del usuario: ";
    cin >> nombre;

    NodoUsuario* usuario = buscarUsuario(raizUsuarios, nombre);

    if (usuario == NULL) {
        cout << "El usuario " << nombre << " no existe." << endl;
        return;
    }

    if (usuario->listaImagenes == NULL) {
        cout << "El usuario " << nombre << " no tiene imagenes asignadas." << endl;
        return;
    }

    mostrarImagenesDeUsuario(usuario);

    cout << "Ingrese el id de la imagen que desea eliminar del usuario: ";
    idImagen = leerEntero();

    if (!usuarioTieneImagen(usuario, idImagen)) {
        cout << "El usuario " << nombre << " no tiene asignada la imagen " << idImagen << "." << endl;
        return;
    }

    quitarImagenDeListaUsuario(usuario, idImagen);
    cout << "Imagen " << idImagen << " eliminada correctamente del usuario " << nombre << "." << endl;
}

// =====================================================
// CRUD IMAGENES
// =====================================================

void menuCRUDImagenes() {
    int opcion = 0;

    do {
        cout << "==============================" << endl;
        cout << "        CRUD IMAGENES         " << endl;
        cout << "==============================" << endl;
        cout << "1. Mostrar imagenes cargadas" << endl;
        cout << "2. Buscar imagen" << endl;
        cout << "3. Mostrar capas de una imagen" << endl;
        cout << "4. Agregar imagen nueva" << endl;
        cout << "5. Eliminar imagen" << endl;
        cout << "6. Regresar al menu principal" << endl;
        cout << "==============================" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();

        cout << endl;

        if (opcion == 1) {
            mostrarListaImagenes();
        }
        else if (opcion == 2) {
            int idImagen;

            cout << "Ingrese el id de la imagen a buscar: ";
            idImagen = leerEntero();

            if (buscarImagen(idImagen) != NULL) {
                cout << "La imagen " << idImagen << " si existe." << endl;
            } else {
                cout << "La imagen " << idImagen << " no existe." << endl;
            }
        }
        else if (opcion == 3) {
            int idImagen;

            cout << "Ingrese el id de la imagen: ";
            idImagen = leerEntero();

            mostrarCapasDeImagen(buscarImagen(idImagen));
        }
        else if (opcion == 4) {
            int idImagen;
            int cantidadCapas;

            cout << "Ingrese el id de la nueva imagen: ";
            idImagen = leerEntero();

            if (buscarImagen(idImagen) != NULL) {
                cout << "La imagen ya existe." << endl;
            } else {
                NodoImagen* nuevaImagen = crearNodoImagen(idImagen);

                cout << "Ingrese cuantas capas tendra la imagen: ";
                cantidadCapas = leerEntero();

                for (int i = 0; i < cantidadCapas; i++) {
                    int idCapa;

                    cout << "Ingrese el id de la capa " << (i + 1) << ": ";
                    idCapa = leerEntero();

                    NodoCapa* capa = buscarCapa(raizCapas, idCapa);

                    if (capa != NULL) {
                        agregarCapaAImagen(nuevaImagen, capa);
                        cout << "Capa agregada." << endl;
                    } else {
                        cout << "La capa no existe. No se agrego." << endl;
                    }
                }

                insertarImagenCircular(nuevaImagen);
                cout << "Imagen agregada correctamente." << endl;
            }
        }
        else if (opcion == 5) {
            eliminarImagenDesdeCRUD();
        }
        else if (opcion == 6) {
            cout << "Regresando al menu principal..." << endl;
        }
        else {
            cout << "Opcion invalida." << endl;
        }

        cout << endl;
    } while (opcion != 6);
}

void eliminarImagenDesdeCRUD() {
    int idImagen;

    cout << "Ingrese el id de la imagen a eliminar: ";
    idImagen = leerEntero();

    if (buscarImagen(idImagen) == NULL) {
        cout << "La imagen no existe." << endl;
        return;
    }

    quitarImagenDeTodosLosUsuarios(raizUsuarios, idImagen);
    eliminarImagenCircular(idImagen);

    cout << "Imagen " << idImagen << " eliminada correctamente." << endl;
    cout << "Tambien se elimino de los usuarios que la tenian asignada." << endl;
}

// =====================================================
// GRAPHVIZ
// =====================================================

void menuReportes() {
    int opcion = 0;

    do {
        cout << "====================================" << endl;
        cout << "     GRAFICAR ESTADO DE MEMORIA     " << endl;
        cout << "====================================" << endl;
        cout << "1. Generar arbol de capas Graphviz" << endl;
        cout << "2. Generar arbol de usuarios Graphviz" << endl;
        cout << "3. Generar lista de imagenes Graphviz" << endl;
        cout << "4. Generar imagen y sus capas Graphviz" << endl;
        cout << "5. Regresar al menu principal" << endl;
        cout << "====================================" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();

        cout << endl;

        if (opcion == 1) {
            generarGraphvizArbolCapas();
        }
        else if (opcion == 2) {
            generarGraphvizArbolUsuarios();
        }
        else if (opcion == 3) {
            generarGraphvizListaImagenes();
        }
        else if (opcion == 4) {
            int idImagen;

            cout << "Ingrese el id de la imagen: ";
            idImagen = leerEntero();

            generarGraphvizImagenCapas(idImagen);
        }
        else if (opcion == 5) {
            cout << "Regresando al menu principal..." << endl;
        }
        else {
            cout << "Opcion invalida." << endl;
        }

        cout << endl;
    } while (opcion != 5);
}

void generarGraphvizArbolCapas() {
    if (raizCapas == NULL) {
        cout << "No hay capas cargadas." << endl;
        return;
    }

    ofstream archivo("arbol_capas.dot");

    if (!archivo.is_open()) {
        cout << "No se pudo crear arbol_capas.dot" << endl;
        return;
    }

    archivo << "digraph ArbolCapas {" << endl;
    archivo << "    rankdir=TB;" << endl;
    archivo << "    node [shape=record, style=filled, fillcolor=lightblue, fontname=Arial];" << endl;

    escribirArbolCapasDot(archivo, raizCapas);

    archivo << "}" << endl;
    archivo.close();

    system("dot -Tpng arbol_capas.dot -o arbol_capas.png");
    cout << "Reporte generado: arbol_capas.png" << endl;
    system("start arbol_capas.png");
}

void escribirArbolCapasDot(ofstream& archivo, NodoCapa* raiz) {
    if (raiz == NULL) {
        return;
    }

    archivo << "    capa" << raiz->idCapa
            << " [label=\"{Capa " << raiz->idCapa
            << "|Pixeles: " << contarPixelesCapa(raiz) << "}\"];" << endl;

    if (raiz->izquierda != NULL) {
        archivo << "    capa" << raiz->idCapa
                << " -> capa" << raiz->izquierda->idCapa
                << " [label=\"izq\"];" << endl;
        escribirArbolCapasDot(archivo, raiz->izquierda);
    }

    if (raiz->derecha != NULL) {
        archivo << "    capa" << raiz->idCapa
                << " -> capa" << raiz->derecha->idCapa
                << " [label=\"der\"];" << endl;
        escribirArbolCapasDot(archivo, raiz->derecha);
    }
}

void generarGraphvizArbolUsuarios() {
    if (raizUsuarios == NULL) {
        cout << "No hay usuarios cargados." << endl;
        return;
    }

    ofstream archivo("arbol_usuarios.dot");

    if (!archivo.is_open()) {
        cout << "No se pudo crear arbol_usuarios.dot" << endl;
        return;
    }

    archivo << "digraph ArbolUsuarios {" << endl;
    archivo << "    rankdir=TB;" << endl;
    archivo << "    node [shape=record, style=filled, fillcolor=lightyellow, fontname=Arial];" << endl;

    escribirArbolUsuariosDot(archivo, raizUsuarios);

    archivo << "}" << endl;
    archivo.close();

    system("dot -Tpng arbol_usuarios.dot -o arbol_usuarios.png");
    cout << "Reporte generado: arbol_usuarios.png" << endl;
    system("start arbol_usuarios.png");
}

void escribirArbolUsuariosDot(ofstream& archivo, NodoUsuario* raiz) {
    if (raiz == NULL) {
        return;
    }

    int contadorImagenes = 0;
    NodoImagenUsuario* imagenActual = raiz->listaImagenes;

    while (imagenActual != NULL) {
        contadorImagenes++;
        imagenActual = imagenActual->siguiente;
    }

    archivo << "    usuario_" << raiz->nombre
            << " [label=\"{Usuario: " << raiz->nombre
            << "|Imagenes: " << contadorImagenes << "}\"];" << endl;

    if (raiz->izquierda != NULL) {
        archivo << "    usuario_" << raiz->nombre
                << " -> usuario_" << raiz->izquierda->nombre
                << " [label=\"izq\"];" << endl;
        escribirArbolUsuariosDot(archivo, raiz->izquierda);
    }

    if (raiz->derecha != NULL) {
        archivo << "    usuario_" << raiz->nombre
                << " -> usuario_" << raiz->derecha->nombre
                << " [label=\"der\"];" << endl;
        escribirArbolUsuariosDot(archivo, raiz->derecha);
    }
}

void generarGraphvizListaImagenes() {
    if (listaImagenes == NULL) {
        cout << "No hay imagenes cargadas." << endl;
        return;
    }

    ofstream archivo("lista_imagenes.dot");

    if (!archivo.is_open()) {
        cout << "No se pudo crear lista_imagenes.dot" << endl;
        return;
    }

    archivo << "digraph ListaImagenes {" << endl;
    archivo << "    rankdir=LR;" << endl;
    archivo << "    node [shape=record, style=filled, fillcolor=lightgreen, fontname=Arial];" << endl;

    NodoImagen* actual = listaImagenes;

    do {
        archivo << "    imagen" << actual->idImagen
                << " [label=\"{Imagen " << actual->idImagen << "|Capas: ";

        NodoCapaImagen* capaActual = actual->listaCapas;

        if (capaActual == NULL) {
            archivo << "sin capas";
        } else {
            while (capaActual != NULL) {
                archivo << capaActual->capa->idCapa;

                if (capaActual->siguiente != NULL) {
                    archivo << ", ";
                }

                capaActual = capaActual->siguiente;
            }
        }

        archivo << "}\"];" << endl;
        actual = actual->siguiente;
    } while (actual != listaImagenes);

    actual = listaImagenes;

    do {
        archivo << "    imagen" << actual->idImagen
                << " -> imagen" << actual->siguiente->idImagen
                << " [label=\"sig\"];" << endl;

        archivo << "    imagen" << actual->idImagen
                << " -> imagen" << actual->anterior->idImagen
                << " [label=\"ant\", color=red];" << endl;

        actual = actual->siguiente;
    } while (actual != listaImagenes);

    archivo << "}" << endl;
    archivo.close();

    system("dot -Tpng lista_imagenes.dot -o lista_imagenes.png");
    cout << "Reporte generado: lista_imagenes.png" << endl;
    system("start lista_imagenes.png");
}

void generarGraphvizImagenCapas(int idImagen) {
    NodoImagen* imagen = buscarImagen(idImagen);

    if (imagen == NULL) {
        cout << "La imagen " << idImagen << " no existe." << endl;
        return;
    }

    ofstream archivo("imagen_capas.dot");

    if (!archivo.is_open()) {
        cout << "No se pudo crear imagen_capas.dot" << endl;
        return;
    }

    archivo << "digraph ImagenCapas {" << endl;
    archivo << "    rankdir=TB;" << endl;
    archivo << "    node [shape=record, style=filled, fontname=Arial];" << endl;

    archivo << "    imagen [label=\"{Imagen " << imagen->idImagen << "|Lista de capas}\", fillcolor=lightgreen];" << endl;

    NodoCapaImagen* capaActual = imagen->listaCapas;
    int contador = 1;
    string nodoAnterior = "imagen";

    while (capaActual != NULL) {
        archivo << "    capaLista" << contador
                << " [label=\"{Apuntador|Capa " << capaActual->capa->idCapa << "}\", fillcolor=lightpink];" << endl;

        archivo << "    capaArbol" << capaActual->capa->idCapa
                << " [label=\"{Capa " << capaActual->capa->idCapa
                << "|Pixeles: " << contarPixelesCapa(capaActual->capa) << "}\", fillcolor=lightblue];" << endl;

        archivo << "    " << nodoAnterior << " -> capaLista" << contador << ";" << endl;
        archivo << "    capaLista" << contador << " -> capaArbol" << capaActual->capa->idCapa
                << " [style=dashed, label=\"apunta\"];" << endl;

        nodoAnterior = "capaLista" + to_string(contador);
        contador++;
        capaActual = capaActual->siguiente;
    }

    archivo << "}" << endl;
    archivo.close();

    system("dot -Tpng imagen_capas.dot -o imagen_capas.png");
    cout << "Reporte generado: imagen_capas.png" << endl;
    system("start imagen_capas.png");
}

// =====================================================
// PRUEBAS BASICAS
// =====================================================

void probarArbolCapas() {
    int opcion = 0;

    do {
        cout << "==============================" << endl;
        cout << "        ARBOL DE CAPAS        " << endl;
        cout << "==============================" << endl;
        cout << "1. Mostrar capas en inorden" << endl;
        cout << "2. Mostrar capas en preorden" << endl;
        cout << "3. Mostrar capas en postorden" << endl;
        cout << "4. Buscar capa" << endl;
        cout << "5. Mostrar pixeles de una capa" << endl;
        cout << "6. Regresar al menu principal" << endl;
        cout << "==============================" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();

        cout << endl;

        if (opcion == 1) {
            mostrarCapasInorden(raizCapas);
        }
        else if (opcion == 2) {
            mostrarCapasPreorden(raizCapas);
        }
        else if (opcion == 3) {
            mostrarCapasPostorden(raizCapas);
        }
        else if (opcion == 4) {
            int idCapa;

            cout << "Ingrese el id de la capa a buscar: ";
            idCapa = leerEntero();

            if (buscarCapa(raizCapas, idCapa) != NULL) {
                cout << "La capa " << idCapa << " si existe." << endl;
            } else {
                cout << "La capa " << idCapa << " no existe." << endl;
            }
        }
        else if (opcion == 5) {
            int idCapa;

            cout << "Ingrese el id de la capa que desea mostrar: ";
            idCapa = leerEntero();

            mostrarPixelesDeCapa(buscarCapa(raizCapas, idCapa));
        }
        else if (opcion == 6) {
            cout << "Regresando al menu principal..." << endl;
        }
        else {
            cout << "Opcion invalida." << endl;
        }

        cout << endl;
    } while (opcion != 6);
}

void probarListaImagenes() {
    int opcion = 0;

    do {
        cout << "==============================" << endl;
        cout << "       LISTA DE IMAGENES      " << endl;
        cout << "==============================" << endl;
        cout << "1. Mostrar imagenes cargadas" << endl;
        cout << "2. Mostrar capas de una imagen" << endl;
        cout << "3. Regresar al menu principal" << endl;
        cout << "==============================" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();

        cout << endl;

        if (opcion == 1) {
            mostrarListaImagenes();
        }
        else if (opcion == 2) {
            int idImagen;

            cout << "Ingrese el id de la imagen: ";
            idImagen = leerEntero();

            mostrarCapasDeImagen(buscarImagen(idImagen));
        }
        else if (opcion == 3) {
            cout << "Regresando al menu principal..." << endl;
        }
        else {
            cout << "Opcion invalida." << endl;
        }

        cout << endl;
    } while (opcion != 3);
}

void probarArbolUsuarios() {
    int opcion = 0;

    do {
        cout << "==============================" << endl;
        cout << "       ARBOL DE USUARIOS      " << endl;
        cout << "==============================" << endl;
        cout << "1. Mostrar usuarios en inorden" << endl;
        cout << "2. Buscar usuario" << endl;
        cout << "3. Mostrar imagenes de un usuario" << endl;
        cout << "4. Regresar al menu principal" << endl;
        cout << "==============================" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero();

        cout << endl;

        if (opcion == 1) {
            mostrarUsuariosInorden(raizUsuarios);
        }
        else if (opcion == 2) {
            string nombre;

            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;

            if (buscarUsuario(raizUsuarios, nombre) != NULL) {
                cout << "El usuario " << nombre << " si existe." << endl;
            } else {
                cout << "El usuario " << nombre << " no existe." << endl;
            }
        }
        else if (opcion == 3) {
            string nombre;

            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;

            mostrarImagenesDeUsuario(buscarUsuario(raizUsuarios, nombre));
        }
        else if (opcion == 4) {
            cout << "Regresando al menu principal..." << endl;
        }
        else {
            cout << "Opcion invalida." << endl;
        }

        cout << endl;
    } while (opcion != 4);
}
