# NeuroNet - Manual de Instalación y Uso

## 📋 Requisitos del Sistema

- **Python 3.7+** con pip
- **Compilador C++** (g++ en Linux/Mac, Visual Studio en Windows)
- **Git** (para clonar repositorios si es necesario)

## 🚀 Instalación Rápida

### Opción 1: Script Automático (Linux/Mac)
```bash
./build.sh
```

### Opción 2: Manual
```bash
# Instalar dependencias
pip3 install cython numpy networkx matplotlib

# Compilar extensión
python3 setup.py build_ext --inplace
```

## ▶️ Ejecutar la Aplicación

### Linux/Mac:
```bash
./run.sh
```

### Windows:
```bash
python src/gui/neuronet_gui.py
```

## 📊 Uso Básico

1. **Cargar Dataset**: Usa el botón "Seleccionar Archivo Dataset"
2. **Ver Información**: Los datos del grafo aparecen automáticamente
3. **Ejecutar BFS**: Especifica nodo inicio y profundidad, luego clic en "Ejecutar BFS"
4. **Ejecutar DFS**: Especifica nodo inicio y clic en "Ejecutar DFS"

## 📁 Datasets de Prueba

- `data/test_small.txt`: Grafo pequeño para pruebas básicas (10 nodos, 18 aristas)

## 🔧 Solución de Problemas

### Error: "No module named 'src.cython.neuronet'"
- Ejecuta: `python3 setup.py build_ext --inplace`

### Error de compilación C++
- Instala build-essential: `sudo apt install build-essential` (Ubuntu)
- Instala Xcode tools: `xcode-select --install` (Mac)

### Interfaz no aparece
- Verifica que tengas tkinter: `python3 -m tkinter`
- Si no está instalado: `sudo apt install python3-tk` (Ubuntu)

## 📈 Funcionamiento del Sistema

El sistema **NeuroNet** funciona en 3 capas:

1. **Núcleo C++**: Procesa los grafos con eficiencia usando listas de adyacencia
2. **Cython**: Conecta C++ con Python de forma transparente  
3. **GUI Python**: Interfaz amigable para el usuario

## 💾 Formato de Datasets

Los archivos deben estar en formato "Edge List":
```
nodo_origen nodo_destino
0 1
0 2  
1 2
...
```

## 🎯 Estado Actual (80% completado)

✅ **Implementado:**
- Clase base abstracta en C++
- Implementación de listas de adyacencia (CSR simplificado)
- Algoritmos BFS y DFS nativos en C++
- Wrapper completo de Cython
- Interfaz gráfica funcional
- Sistema de compilación automático

🚧 **Falta (20%):**
- Visualización con NetworkX/Matplotlib
- Optimización de memoria real con CSR
- Manejo de datasets masivos (>1M nodos)
- Métricas adicionales de centralidad