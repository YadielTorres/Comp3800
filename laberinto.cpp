#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Dimensiones maximas
const int MAX = 64;

/**
*Lee el laberinto desde un archivo de texto y cargarlo en una matriz.
*Parametros: 
*nombreArchivo: string con la ruta del archivo.
*mapa: matriz de caracteres donde se guarda el laberinto.
*filas, columnas: guardan el tamaño detectado.
*Pre-condiciones: El archivo debe existir y tener un formato rectangular.
*Post-condiciones: La matriz 'mapa' queda llena y se actualizan filas/columnas.
*/
bool cargarLaberinto(string nombreArchivo, char mapa[MAX][MAX], int &filas, int &columnas) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return false;
    }

    string linea;
    filas = 0;
    while (getline(archivo, linea) && filas < MAX) {
        columnas = linea.length();
        for (int j = 0; j < columnas; j++) {
            mapa[filas][j] = linea[j];
        }
        filas++;
    }
    archivo.close();
    return true;
}

/*
*Muestra el laberinto en la consola.
*/
void imprimirLaberinto(char mapa[MAX][MAX], int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << mapa[i][j];
        }
        cout << endl;
    }
}

/*
*Resolver el laberinto
*Parametros: 
*x, y: coordenadas.
*mapa: matriz del laberinto.
*condiciones: El laberinto debe contener un punto '?' que es alcanzable.
*Pre-condiciones: El laberinto debe contener un punto '?' alcanzable.
*Post-condiciones: Regresa true si encuentra la meta, marca el camino con puntos.
*/
bool resolver(int x, int y, char mapa[MAX][MAX], int filas, int columnas) {
    //Caso: Fuera de limites
    if (x < 0 || x >= filas || y < 0 || y >= columnas) return false;

    //Caso: Encontro la meta
    if (mapa[x][y] == '?') return true;

    //Caso: Pared ya visitada
    if (mapa[x][y] == '#' || mapa[x][y] == '.' || mapa[x][y] == 'v') return false;

    //Marcar como visitado (v) temporalmente
    char original = mapa[x][y];
    if (mapa[x][y] != 'S') mapa[x][y] = 'v';

    //Intentar mover en 4 direcciones: Abajo, Arriba, Derecha, Izquierda
    if (resolver(x + 1, y, mapa, filas, columnas) || resolver(x - 1, y, mapa, filas, columnas) || resolver(x, y + 1, mapa, filas, columnas) || resolver(x, y - 1, mapa, filas, columnas)) {
        
        //Si una direccion funciono, marcar camino con puntos
        if (original != 'S') mapa[x][y] = '.';
        return true;
    }

    // Si ninguna direccion funciono, no es parte del camino (Backtrack)
    // Se deja como 'v' (visitado) para no repetir el error.
    return false;
}

int main() {
    char mapa[MAX][MAX];
    int filas, columnas;
    string nombreArchivo;

    cout << "--- Solucionador de Laberinto ---" << endl;
    cout << "Ingrese el nombre del archivo del laberinto: ";
    cin >> nombreArchivo;

    if (cargarLaberinto(nombreArchivo, mapa, filas, columnas)) {
        cout << "\nLaberinto Original:\n";
        imprimirLaberinto(mapa, filas, columnas);

        //Buscar punto de inicio 'S'
        int inicioX = -1, inicioY = -1;
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                if (mapa[i][j] == 'S') {
                    inicioX = i;
                    inicioY = j;
                }
            }
        }

        if (inicioX != -1 && resolver(inicioX, inicioY, mapa, filas, columnas)) {
            cout << "\nLaberinto Resuelto:\n";
            imprimirLaberinto(mapa, filas, columnas);
            cout << "\nMeta '?' encontrada" << endl;
        } else {
            cout << "\nNo se encontro una solucion para este laberinto." << endl;
        }
    }

    return 0;
}