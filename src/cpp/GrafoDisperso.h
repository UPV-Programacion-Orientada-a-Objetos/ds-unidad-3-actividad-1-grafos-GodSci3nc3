#ifndef GRAFODISPERSO_H
#define GRAFODISPERSO_H

#include "GrafoBase.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <queue>

class GrafoDisperso : public GrafoBase {
private:
    // Representación CSR simplificada
    std::vector<std::vector<int>> listaAdyacencia;  // Más simple que CSR completo
    int numNodos;
    int numAristas;
    
public:
    // Constructor
    GrafoDisperso() : numNodos(0), numAristas(0) {
        std::cout << "[C++ Core] Inicializando GrafoDisperso..." << std::endl;
    }
    
    // Destructor
    ~GrafoDisperso() = default;
    
    // Implementación de métodos abstractos
    void cargarDatos(const std::string& archivo) override;
    std::vector<int> BFS(int nodoInicio, int profundidad) override;
    std::vector<int> DFS(int nodoInicio) override;
    int obtenerGrado(int nodo) override;
    std::vector<int> getVecinos(int nodo) override;
    int getNumNodos() const override { return numNodos; }
    int getNumAristas() const override { return numAristas; }
    int nodoMayorGrado() override;
    void imprimirInfo() override;
    
private:
    // Métodos auxiliares
    void ajustarTamano(int maxNodo);
    void DFSRecursivo(int nodo, std::vector<bool>& visitado, std::vector<int>& resultado);
};

#endif