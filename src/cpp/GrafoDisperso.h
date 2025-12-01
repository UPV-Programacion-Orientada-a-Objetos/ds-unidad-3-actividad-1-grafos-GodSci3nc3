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
    // Representación CSR (Compressed Sparse Row) real
    std::vector<int> values;        // Nodos destino  
    std::vector<int> col_indices;   // Índices de columnas (igual que values para grafos simples)
    std::vector<int> row_ptr;       // Punteros de inicio de cada fila
    
    // Para compatibilidad y búsquedas rápidas
    std::vector<std::vector<int>> listaAdyacencia;  // Mantener para acceso rápido
    
    int numNodos;
    int numAristas;
    bool usarCSR;  // Flag para alternar entre representaciones
    
public:
    // Constructor
    GrafoDisperso() : numNodos(0), numAristas(0), usarCSR(true) {
        std::cout << "[C++ Core] Inicializando GrafoDisperso con CSR..." << std::endl;
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
    double densidad() override;
    std::vector<int> componenteConexo(int nodo) override;
    int diametroAproximado() override;
    void imprimirInfo() override;
    
private:
    // Métodos auxiliares
    void ajustarTamano(int maxNodo);
    void construirCSR();
    void DFSRecursivo(int nodo, std::vector<bool>& visitado, std::vector<int>& resultado);
    size_t calcularMemoriaCSR() const;
};

#endif