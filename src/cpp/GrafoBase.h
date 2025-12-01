#ifndef GRAFOBASE_H
#define GRAFOBASE_H

#include <vector>
#include <string>

class GrafoBase {
public:
    // Constructor y destructor virtual
    GrafoBase() = default;
    virtual ~GrafoBase() = default;
    
    // Métodos virtuales puros (abstractos)
    virtual void cargarDatos(const std::string& archivo) = 0;
    virtual std::vector<int> BFS(int nodoInicio, int profundidad) = 0;
    virtual std::vector<int> DFS(int nodoInicio) = 0;
    virtual int obtenerGrado(int nodo) = 0;
    virtual std::vector<int> getVecinos(int nodo) = 0;
    virtual int getNumNodos() const = 0;
    virtual int getNumAristas() const = 0;
    
    // Método para encontrar el nodo con mayor grado
    virtual int nodoMayorGrado() = 0;
    
    // Nuevas métricas
    virtual double densidad() = 0;
    virtual std::vector<int> componenteConexo(int nodo) = 0;
    virtual int diametroAproximado() = 0;
    
    // Método para obtener información del grafo
    virtual void imprimirInfo() = 0;
};

#endif