import tkinter as tk
from tkinter import ttk, filedialog, messagebox, scrolledtext
import os
import sys
import time

# Agregar el directorio padre al path para importar el módulo compilado
sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))

try:
    from src.cython.neuronet import PyGrafoDisperso
    CYTHON_AVAILABLE = True
except ImportError:
    print("Advertencia: Módulo Cython no disponible. Usando modo demo.")
    CYTHON_AVAILABLE = False

class NeuroNetGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("NeuroNet - Análisis de Grafos Masivos")
        self.root.geometry("800x600")
        
        # Inicializar el grafo
        if CYTHON_AVAILABLE:
            self.grafo = PyGrafoDisperso()
        else:
            self.grafo = None
        
        self.archivo_cargado = None
        self.crear_interfaz()
    
    def crear_interfaz(self):
        # Frame principal
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configurar peso de las columnas y filas
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=1)
        
        # Sección 1: Carga de datos
        datos_frame = ttk.LabelFrame(main_frame, text="📁 Carga de Datos", padding="5")
        datos_frame.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        self.btn_cargar = ttk.Button(datos_frame, text="Seleccionar Archivo Dataset", 
                                   command=self.cargar_archivo)
        self.btn_cargar.grid(row=0, column=0, padx=(0, 10))
        
        self.lbl_archivo = ttk.Label(datos_frame, text="No hay archivo cargado", 
                                   foreground="gray")
        self.lbl_archivo.grid(row=0, column=1, sticky=tk.W)
        
        # Sección 2: Información del grafo
        info_frame = ttk.LabelFrame(main_frame, text="📊 Información del Grafo", padding="5")
        info_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        self.lbl_nodos = ttk.Label(info_frame, text="Nodos: -")
        self.lbl_nodos.grid(row=0, column=0, sticky=tk.W)
        
        self.lbl_aristas = ttk.Label(info_frame, text="Aristas: -")
        self.lbl_aristas.grid(row=0, column=1, sticky=tk.W, padx=(20, 0))
        
        self.lbl_mayor_grado = ttk.Label(info_frame, text="Nodo con mayor grado: -")
        self.lbl_mayor_grado.grid(row=1, column=0, columnspan=2, sticky=tk.W)
        
        # Sección 3: Algoritmos
        algo_frame = ttk.LabelFrame(main_frame, text="🔍 Análisis Topológico", padding="5")
        algo_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # BFS
        ttk.Label(algo_frame, text="Nodo inicio:").grid(row=0, column=0, sticky=tk.W)
        self.entry_nodo_inicio = ttk.Entry(algo_frame, width=10)
        self.entry_nodo_inicio.grid(row=0, column=1, padx=(5, 10))
        self.entry_nodo_inicio.insert(0, "0")
        
        ttk.Label(algo_frame, text="Profundidad:").grid(row=0, column=2, sticky=tk.W)
        self.entry_profundidad = ttk.Entry(algo_frame, width=10)
        self.entry_profundidad.grid(row=0, column=3, padx=(5, 10))
        self.entry_profundidad.insert(0, "2")
        
        self.btn_bfs = ttk.Button(algo_frame, text="Ejecutar BFS", 
                                command=self.ejecutar_bfs, state="disabled")
        self.btn_bfs.grid(row=0, column=4, padx=(10, 0))
        
        self.btn_dfs = ttk.Button(algo_frame, text="Ejecutar DFS", 
                                command=self.ejecutar_dfs, state="disabled")
        self.btn_dfs.grid(row=0, column=5, padx=(10, 0))
        
        # Sección 4: Resultados
        resultado_frame = ttk.LabelFrame(main_frame, text="📋 Resultados", padding="5")
        resultado_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        main_frame.rowconfigure(3, weight=1)
        
        self.text_resultados = scrolledtext.ScrolledText(resultado_frame, height=15, width=80)
        self.text_resultados.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        resultado_frame.columnconfigure(0, weight=1)
        resultado_frame.rowconfigure(0, weight=1)
        
        # Botón para limpiar resultados
        ttk.Button(resultado_frame, text="Limpiar", 
                  command=self.limpiar_resultados).grid(row=1, column=0, sticky=tk.E, pady=(5, 0))
        
        self.log("NeuroNet iniciado. Selecciona un archivo dataset para comenzar.")
    
    def log(self, mensaje):
        """Agregar mensaje al área de resultados con timestamp"""
        timestamp = time.strftime("%H:%M:%S")
        self.text_resultados.insert(tk.END, f"[{timestamp}] {mensaje}\\n")
        self.text_resultados.see(tk.END)
        self.root.update()
    
    def cargar_archivo(self):
        """Carga un archivo dataset"""
        archivo = filedialog.askopenfilename(
            title="Seleccionar archivo dataset",
            filetypes=[("Archivos de texto", "*.txt"), ("Todos los archivos", "*.*")]
        )
        
        if archivo and CYTHON_AVAILABLE and self.grafo:
            try:
                self.log(f"Cargando archivo: {os.path.basename(archivo)}")
                inicio = time.time()
                
                self.grafo.cargar_datos(archivo)
                
                fin = time.time()
                tiempo = fin - inicio
                
                self.archivo_cargado = archivo
                self.lbl_archivo.config(text=f"Archivo: {os.path.basename(archivo)}", 
                                      foreground="green")
                
                # Actualizar información
                num_nodos = self.grafo.get_num_nodos()
                num_aristas = self.grafo.get_num_aristas()
                nodo_max = self.grafo.nodo_mayor_grado()
                grado_max = self.grafo.obtener_grado(nodo_max)
                
                self.lbl_nodos.config(text=f"Nodos: {num_nodos:,}")
                self.lbl_aristas.config(text=f"Aristas: {num_aristas:,}")
                self.lbl_mayor_grado.config(text=f"Nodo con mayor grado: {nodo_max} (grado: {grado_max})")
                
                # Habilitar botones
                self.btn_bfs.config(state="normal")
                self.btn_dfs.config(state="normal")
                
                self.log(f"Carga completada en {tiempo:.2f} segundos")
                self.log(f"Resumen: {num_nodos:,} nodos, {num_aristas:,} aristas")
                
            except Exception as e:
                messagebox.showerror("Error", f"No se pudo cargar el archivo: {str(e)}")
                self.log(f"ERROR: {str(e)}")
        
        elif archivo and not CYTHON_AVAILABLE:
            self.log("Modo demo: Simulando carga de archivo...")
            self.archivo_cargado = archivo
            self.lbl_archivo.config(text=f"Archivo: {os.path.basename(archivo)}", foreground="orange")
            self.lbl_nodos.config(text="Nodos: 1,000 (demo)")
            self.lbl_aristas.config(text="Aristas: 2,500 (demo)")
            self.lbl_mayor_grado.config(text="Nodo con mayor grado: 42 (grado: 15)")
            self.btn_bfs.config(state="normal")
            self.btn_dfs.config(state="normal")
    
    def ejecutar_bfs(self):
        """Ejecuta algoritmo BFS"""
        try:
            nodo_inicio = int(self.entry_nodo_inicio.get())
            profundidad = int(self.entry_profundidad.get())
            
            self.log(f"Ejecutando BFS desde nodo {nodo_inicio}, profundidad {profundidad}...")
            
            if CYTHON_AVAILABLE and self.grafo:
                inicio = time.time()
                resultado = self.grafo.bfs(nodo_inicio, profundidad)
                fin = time.time()
                
                tiempo = (fin - inicio) * 1000
                self.log(f"BFS completado en {tiempo:.2f}ms")
                self.log(f"Nodos encontrados: {len(resultado)}")
                
                # Mostrar algunos nodos del resultado
                if len(resultado) <= 20:
                    self.log(f"Nodos visitados: {resultado}")
                else:
                    self.log(f"Primeros 20 nodos: {resultado[:20]}...")
                    
                # Simular información de visualización
                self.log("\\n--- Información para visualización ---")
                self.log(f"Nodo central: {nodo_inicio}")
                self.log(f"Vecinos nivel 1: {len([n for n in resultado[1:] if n in self.grafo.get_vecinos(nodo_inicio)])}")
                
            else:
                self.log("Modo demo: BFS simulado completado")
                self.log("Nodos encontrados: [0, 1, 5, 8, 12, 15, 23, 42]")
        
        except ValueError:
            messagebox.showerror("Error", "Por favor ingresa números válidos")
        except Exception as e:
            self.log(f"ERROR en BFS: {str(e)}")
    
    def ejecutar_dfs(self):
        """Ejecuta algoritmo DFS"""
        try:
            nodo_inicio = int(self.entry_nodo_inicio.get())
            
            self.log(f"Ejecutando DFS desde nodo {nodo_inicio}...")
            
            if CYTHON_AVAILABLE and self.grafo:
                inicio = time.time()
                resultado = self.grafo.dfs(nodo_inicio)
                fin = time.time()
                
                tiempo = (fin - inicio) * 1000
                self.log(f"DFS completado en {tiempo:.2f}ms")
                self.log(f"Nodos alcanzables: {len(resultado)}")
                
                # Mostrar algunos nodos del resultado
                if len(resultado) <= 20:
                    self.log(f"Recorrido DFS: {resultado}")
                else:
                    self.log(f"Primeros 20 nodos del recorrido: {resultado[:20]}...")
            else:
                self.log("Modo demo: DFS simulado completado")
                self.log("Nodos alcanzables: [0, 1, 3, 7, 8, 15, 23, 42, 56, 89]")
        
        except ValueError:
            messagebox.showerror("Error", "Por favor ingresa un número válido")
        except Exception as e:
            self.log(f"ERROR en DFS: {str(e)}")
    
    def limpiar_resultados(self):
        """Limpia el área de resultados"""
        self.text_resultados.delete(1.0, tk.END)
        self.log("Resultados limpiados.")

def main():
    root = tk.Tk()
    app = NeuroNetGUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()