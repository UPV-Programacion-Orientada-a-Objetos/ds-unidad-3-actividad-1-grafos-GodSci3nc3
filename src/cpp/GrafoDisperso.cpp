#include "GrafoDisperso.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <chrono>

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
            // Para grafos no dirigidos, agregar también la arista inversa
            // listaAdyacencia[destino].push_back(origen);
        }
    }
    
    file.close();
    std::cout << "[C++ Core] Carga completa. Nodos: " << numNodos << " | Aristas: " << numAristas << std::endl;
}

void GrafoDisperso::ajustarTamano(int maxNodo) {
    listaAdyacencia.resize(maxNodo);
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

void GrafoDisperso::imprimirInfo() {
    std::cout << "=== Información del Grafo ===" << std::endl;
    std::cout << "Nodos: " << numNodos << std::endl;
    std::cout << "Aristas: " << numAristas << std::endl;
    
    // Calcular memoria estimada (muy aproximado)
    size_t memoriaBytes = 0;
    for (const auto& lista : listaAdyacencia) {
        memoriaBytes += lista.size() * sizeof(int);
    }
    memoriaBytes += listaAdyacencia.size() * sizeof(std::vector<int>);
    
    std::cout << "Memoria estimada: " << memoriaBytes / (1024 * 1024) << " MB" << std::endl;
    
    int nodoMax = nodoMayorGrado();
    std::cout << "Nodo con mayor grado: " << nodoMax << " (grado: " << obtenerGrado(nodoMax) << ")" << std::endl;
    std::cout << "=========================" << std::endl;
}