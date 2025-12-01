#!/bin/bash

# Script para ejecutar NeuroNet
echo "=== Iniciando NeuroNet ==="

# Verificar que el módulo esté compilado
if [ ! -f "src/cython/neuronet.cpython-*.so" ] && [ ! -f "src/cython/neuronet.pyd" ]; then
    echo "Módulo no compilado. Ejecutando compilación..."
    ./build.sh
fi

echo "Ejecutando interfaz gráfica..."
python3 src/gui/neuronet_gui.py