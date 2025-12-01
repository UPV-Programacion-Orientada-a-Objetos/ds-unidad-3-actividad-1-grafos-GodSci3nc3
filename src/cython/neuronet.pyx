# Wrapper de Python para la clase C++
from libcpp.vector cimport vector
from libcpp.string cimport string
from .neuronet cimport GrafoDisperso

cdef class PyGrafoDisperso:
    cdef GrafoDisperso* grafo_cpp
    
    def __cinit__(self):
        print("[Cython] Inicializando wrapper Python...")
        self.grafo_cpp = new GrafoDisperso()
    
    def __dealloc__(self):
        if self.grafo_cpp != NULL:
            del self.grafo_cpp
    
    def cargar_datos(self, str archivo):
        """Carga un archivo de dataset"""
        print(f"[Cython] Solicitud de carga: {archivo}")
        cdef string archivo_cpp = archivo.encode('utf-8')
        self.grafo_cpp.cargarDatos(archivo_cpp)
    
    def bfs(self, int nodo_inicio, int profundidad=2):
        """Ejecuta BFS desde un nodo con profundidad limitada"""
        print(f"[Cython] Solicitud recibida: BFS desde Nodo {nodo_inicio}, Profundidad {profundidad}.")
        cdef vector[int] resultado_cpp = self.grafo_cpp.BFS(nodo_inicio, profundidad)
        
        # Convertir vector de C++ a lista de Python
        cdef list resultado_python = []
        cdef int i
        for i in range(resultado_cpp.size()):
            resultado_python.append(resultado_cpp[i])
        
        print("[Cython] Retornando lista de nodos a Python.")
        return resultado_python
    
    def dfs(self, int nodo_inicio):
        """Ejecuta DFS desde un nodo"""
        cdef vector[int] resultado_cpp = self.grafo_cpp.DFS(nodo_inicio)
        
        cdef list resultado_python = []
        cdef int i
        for i in range(resultado_cpp.size()):
            resultado_python.append(resultado_cpp[i])
        
        return resultado_python
    
    def obtener_grado(self, int nodo):
        """Obtiene el grado de un nodo"""
        return self.grafo_cpp.obtenerGrado(nodo)
    
    def get_vecinos(self, int nodo):
        """Obtiene la lista de vecinos de un nodo"""
        cdef vector[int] vecinos_cpp = self.grafo_cpp.getVecinos(nodo)
        
        cdef list vecinos_python = []
        cdef int i
        for i in range(vecinos_cpp.size()):
            vecinos_python.append(vecinos_cpp[i])
        
        return vecinos_python
    
    def get_num_nodos(self):
        """Obtiene el número total de nodos"""
        return self.grafo_cpp.getNumNodos()
    
    def get_num_aristas(self):
        """Obtiene el número total de aristas"""
        return self.grafo_cpp.getNumAristas()
    
    def nodo_mayor_grado(self):
        """Encuentra el nodo con mayor grado"""
        return self.grafo_cpp.nodoMayorGrado()
    
    def imprimir_info(self):
        """Imprime información del grafo"""
        self.grafo_cpp.imprimirInfo()