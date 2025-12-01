#include "GrafoDisperso.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <chrono>
#include <iomanip>

void GrafoDisperso::cargarDatos(const std::string& archivo) {
    std::cout << "[C++ Core] Cargando dataset '" << archivo << "'..." << std::endl;
    
    std::ifstream file(archivo);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
        return;
    }
    
    std::string linea;
    int maxNodo = 0;
    numAristas = 0;
    
    // Primera pasada: encontrar el nodo máximo
    while (std::getline(file, linea)) {
        // Ignorar líneas que empiecen con # (comentarios)
        if (linea[0] == '#' || linea.empty()) continue;
        
        std::istringstream iss(linea);
        int origen, destino;
        if (iss >> origen >> destino) {
            maxNodo = std::max(maxNodo, std::max(origen, destino));
            numAristas++;
        }
    }
    
    numNodos = maxNodo + 1;
    ajustarTamano(numNodos);
    
    // Segunda pasada: cargar las aristas
    file.clear();
    file.seekg(0);
    
    while (std::getline(file, linea)) {
        if (linea[0] == '#' || linea.empty()) continue;
        
        std::istringstream iss(linea);
        int origen, destino;
        if (iss >> origen >> destino) {
            listaAdyacencia[origen].push_back(destino);
        }
    }
    
    file.close();
    
    // Construir representación CSR
    if (usarCSR) {
        construirCSR();
    }
    
    std::cout << "[C++ Core] Carga completa. Nodos: " << numNodos << " | Aristas: " << numAristas << std::endl;
}

void GrafoDisperso::ajustarTamano(int maxNodo) {
    listaAdyacencia.resize(maxNodo);
}

void GrafoDisperso::construirCSR() {
    std::cout << "[C++ Core] Construyendo CSR..." << std::endl;
    
    // Limpiar vectores CSR
    values.clear();
    col_indices.clear();
    row_ptr.clear();
    row_ptr.resize(numNodos + 1, 0);
    
    // Construir CSR
    for (int i = 0; i < numNodos; i++) {
        row_ptr[i] = values.size();
        for (int vecino : listaAdyacencia[i]) {
            values.push_back(vecino);
            col_indices.push_back(vecino);
        }
    }
    row_ptr[numNodos] = values.size();
    
    std::cout << "[C++ Core] CSR construido. Memoria: " << calcularMemoriaCSR() / (1024*1024) << " MB" << std::endl;
}

size_t GrafoDisperso::calcularMemoriaCSR() const {
    return values.size() * sizeof(int) + 
           col_indices.size() * sizeof(int) + 
           row_ptr.size() * sizeof(int);
}

std::vector<int> GrafoDisperso::BFS(int nodoInicio, int profundidad) {
    std::cout << "[C++ Core] Ejecutando BFS nativo..." << std::endl;
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::vector<int> resultado;
    std::vector<bool> visitado(numNodos, false);
    std::queue<std::pair<int, int>> cola; // par de (nodo, nivel)
    
    cola.push({nodoInicio, 0});
    visitado[nodoInicio] = true;
    resultado.push_back(nodoInicio);
    
    while (!cola.empty()) {
        auto [nodoActual, nivel] = cola.front();
        cola.pop();
        
        if (nivel >= profundidad) continue;
        
        for (int vecino : listaAdyacencia[nodoActual]) {
            if (!visitado[vecino]) {
                visitado[vecino] = true;
                resultado.push_back(vecino);
                cola.push({vecino, nivel + 1});
            }
        }
    }
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
    std::cout << "[C++ Core] Nodos encontrados: " << resultado.size() 
              << ". Tiempo ejecución: " << duracion.count() / 1000.0 << "ms." << std::endl;
    
    return resultado;
}

std::vector<int> GrafoDisperso::DFS(int nodoInicio) {
    std::vector<int> resultado;
    std::vector<bool> visitado(numNodos, false);
    
    DFSRecursivo(nodoInicio, visitado, resultado);
    return resultado;
}

void GrafoDisperso::DFSRecursivo(int nodo, std::vector<bool>& visitado, std::vector<int>& resultado) {
    visitado[nodo] = true;
    resultado.push_back(nodo);
    
    for (int vecino : listaAdyacencia[nodo]) {
        if (!visitado[vecino]) {
            DFSRecursivo(vecino, visitado, resultado);
        }
    }
}

int GrafoDisperso::obtenerGrado(int nodo) {
    if (nodo >= 0 && nodo < numNodos) {
        return listaAdyacencia[nodo].size();
    }
    return 0;
}

std::vector<int> GrafoDisperso::getVecinos(int nodo) {
    if (nodo >= 0 && nodo < numNodos) {
        return listaAdyacencia[nodo];
    }
    return std::vector<int>();
}

int GrafoDisperso::nodoMayorGrado() {
    int maxGrado = 0;
    int nodoMax = 0;
    
    for (int i = 0; i < numNodos; i++) {
        int grado = obtenerGrado(i);
        if (grado > maxGrado) {
            maxGrado = grado;
            nodoMax = i;
        }
    }
    
    return nodoMax;
}

double GrafoDisperso::densidad() {
    if (numNodos <= 1) return 0.0;
    double aristasMaximas = static_cast<double>(numNodos) * (numNodos - 1);
    return (2.0 * numAristas) / aristasMaximas;
}

std::vector<int> GrafoDisperso::componenteConexo(int nodo) {
    std::vector<int> componente;
    std::vector<bool> visitado(numNodos, false);
    
    // BFS para encontrar todos los nodos conectados
    std::queue<int> cola;
    cola.push(nodo);
    visitado[nodo] = true;
    componente.push_back(nodo);
    
    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        
        for (int vecino : listaAdyacencia[actual]) {
            if (!visitado[vecino]) {
                visitado[vecino] = true;
                componente.push_back(vecino);
                cola.push(vecino);
            }
        }
    }
    
    return componente;
}

int GrafoDisperso::diametroAproximado() {
    if (numNodos == 0) return 0;
    
    // Método simple: BFS desde varios nodos aleatorios
    int maxDistancia = 0;
    int muestras = std::min(10, numNodos);
    
    for (int muestra = 0; muestra < muestras; muestra++) {
        int nodoInicio = (muestra * 17) % numNodos; // Simple sampling
        
        std::vector<int> distancia(numNodos, -1);
        std::queue<int> cola;
        
        cola.push(nodoInicio);
        distancia[nodoInicio] = 0;
        
        while (!cola.empty()) {
            int actual = cola.front();
            cola.pop();
            
            for (int vecino : listaAdyacencia[actual]) {
                if (distancia[vecino] == -1) {
                    distancia[vecino] = distancia[actual] + 1;
                    maxDistancia = std::max(maxDistancia, distancia[vecino]);
                    cola.push(vecino);
                }
            }
        }
    }
    
    return maxDistancia;
}

void GrafoDisperso::imprimirInfo() {
    std::cout << "=== Información del Grafo ===" << std::endl;
    std::cout << "Nodos: " << numNodos << std::endl;
    std::cout << "Aristas: " << numAristas << std::endl;
    std::cout << "Densidad: " << std::fixed << std::setprecision(6) << densidad() << std::endl;
    std::cout << "Diametro aproximado: " << diametroAproximado() << std::endl;
    
    // Calcular memoria estimada
    size_t memoriaLista = 0;
    for (const auto& lista : listaAdyacencia) {
        memoriaLista += lista.size() * sizeof(int);
    }
    memoriaLista += listaAdyacencia.size() * sizeof(std::vector<int>);
    
    std::cout << "Memoria lista: " << memoriaLista / (1024 * 1024) << " MB" << std::endl;
    
    if (usarCSR && !values.empty()) {
        std::cout << "Memoria CSR: " << calcularMemoriaCSR() / (1024 * 1024) << " MB" << std::endl;
        std::cout << "Ahorro memoria: " << (memoriaLista > calcularMemoriaCSR() ? "Si" : "No") << std::endl;
    }
    
    int nodoMax = nodoMayorGrado();
    std::cout << "Nodo con mayor grado: " << nodoMax << " (grado: " << obtenerGrado(nodoMax) << ")" << std::endl;
    std::cout << "=========================" << std::endl;
}