# Declaración de la interfaz C++
from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "../cpp/GrafoBase.h":
    cdef cppclass GrafoBase:
        GrafoBase() except +
        vector[int] BFS(int nodoInicio, int profundidad)
        vector[int] DFS(int nodoInicio) 
        int obtenerGrado(int nodo)
        vector[int] getVecinos(int nodo)
        int getNumNodos()
        int getNumAristas()
        int nodoMayorGrado()
        void imprimirInfo()

cdef extern from "../cpp/GrafoDisperso.h":
    cdef cppclass GrafoDisperso(GrafoBase):
        GrafoDisperso() except +
        void cargarDatos(string archivo)