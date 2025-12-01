#!/bin/bash

echo "=== Compilando NeuroNet ==="

# Verificar que tenemos Python y las dependencias necesarias
echo "Verificando dependencias..."

# Instalar dependencias si es necesario
echo "Instalando dependencias Python..."
pip3 install cython numpy networkx matplotlib

echo "Compilando extensión C++ con Cython..."
python3 setup.py build_ext --inplace

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
    echo "Para ejecutar la GUI: python3 src/gui/neuronet_gui.py"
else
    echo "❌ Error en la compilación"
    echo "Asegúrate de tener instalado:"
    echo "- Python 3.7+"
    echo "- Compilador C++ (g++)"
    echo "- Cython"
    echo "- NumPy"
fi