#!/bin/bash

echo "=== Ejecutando NeuroNet ==="

# Verificar que la extensión esté compilada
if [ ! -f "src/cython/neuronet.*.so" ] && [ ! -f "src/cython/neuronet.pyd" ]; then
    echo "Extensión no encontrada. Compilando primero..."
    ./build.sh
    if [ $? -ne 0 ]; then
        echo "Error en compilación"
        exit 1
    fi
fi

echo "Iniciando interfaz gráfica..."
python3 src/gui/neuronet_gui.py

# Script para ejecutar NeuroNet
echo "=== Iniciando NeuroNet ==="

# Verificar que el módulo esté compilado
if [ ! -f "src/cython/neuronet.cpython-*.so" ] && [ ! -f "src/cython/neuronet.pyd" ]; then
    echo "Módulo no compilado. Ejecutando compilación..."
    ./build.sh
fi

echo "Ejecutando interfaz gráfica..."
python3 src/gui/neuronet_gui.py