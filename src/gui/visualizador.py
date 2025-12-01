import matplotlib.pyplot as plt
import networkx as nx
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
from tkinter import ttk

class VisualizadorGrafo:
    def __init__(self, parent_frame):
        self.parent_frame = parent_frame
        self.fig = None
        self.canvas = None
        
    def visualizar_subgrafo(self, nodos, obtener_vecinos_func):
        """Visualiza un subgrafo usando NetworkX y Matplotlib"""
        try:
            # Crear grafo NetworkX
            G = nx.Graph()
            
            # Agregar nodos
            for nodo in nodos:
                G.add_node(nodo)
            
            # Agregar aristas
            aristas_agregadas = set()
            for nodo in nodos:
                vecinos = obtener_vecinos_func(nodo)
                for vecino in vecinos:
                    if vecino in nodos:
                        # Evitar aristas duplicadas
                        arista = tuple(sorted([nodo, vecino]))
                        if arista not in aristas_agregadas:
                            G.add_edge(nodo, vecino)
                            aristas_agregadas.add(arista)
            
            # Crear nueva ventana para la visualización
            ventana_viz = tk.Toplevel(self.parent_frame)
            ventana_viz.title(f"Visualización del Subgrafo - {len(nodos)} nodos")
            ventana_viz.geometry("800x600")
            
            # Crear figura matplotlib
            self.fig = plt.Figure(figsize=(10, 8))
            ax = self.fig.add_subplot(111)
            
            # Configurar layout
            if len(nodos) <= 50:
                pos = nx.spring_layout(G, k=1, iterations=50)
            else:
                pos = nx.spring_layout(G, k=0.5, iterations=30)
            
            # Dibujar el grafo
            nx.draw_networkx_nodes(G, pos, ax=ax, 
                                 node_color='lightblue', 
                                 node_size=300,
                                 alpha=0.8)
            
            nx.draw_networkx_edges(G, pos, ax=ax,
                                 edge_color='gray',
                                 width=1,
                                 alpha=0.6)
            
            # Mostrar etiquetas solo si hay pocos nodos
            if len(nodos) <= 20:
                nx.draw_networkx_labels(G, pos, ax=ax, font_size=8)
            
            ax.set_title(f'Subgrafo - {len(nodos)} nodos, {G.number_of_edges()} aristas')
            ax.axis('off')
            
            # Agregar canvas a la ventana
            self.canvas = FigureCanvasTkAgg(self.fig, ventana_viz)
            self.canvas.draw()
            self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
            
            # Agregar botón para cerrar
            btn_cerrar = ttk.Button(ventana_viz, text="Cerrar", 
                                  command=ventana_viz.destroy)
            btn_cerrar.pack(pady=5)
            
            return True
            
        except Exception as e:
            print(f"Error en visualización: {str(e)}")
            return False