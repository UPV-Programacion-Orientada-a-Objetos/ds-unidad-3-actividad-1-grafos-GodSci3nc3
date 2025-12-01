from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy
import os

# Configuración del compilador
cpp_files = [
    "src/cpp/GrafoDisperso.cpp"
]

# Directorios de inclusión
include_dirs = [
    "src/cpp",
    numpy.get_include()
]

# Extensión de Cython
extensions = [
    Extension(
        name="src.cython.neuronet",
        sources=["src/cython/neuronet.pyx"] + cpp_files,
        include_dirs=include_dirs,
        language="c++",
        extra_compile_args=["-std=c++17", "-O3"],
        extra_link_args=["-std=c++17"]
    )
]

setup(
    name="NeuroNet",
    version="1.0",
    description="Sistema híbrido para análisis de grafos masivos",
    author="Estudiante POO",
    ext_modules=cythonize(extensions, compiler_directives={'embedsignature': True}),
    zip_safe=False,
    python_requires='>=3.7',
    install_requires=[
        'cython',
        'numpy',
        'networkx',  # Para futura visualización
        'matplotlib'  # Para futura visualización
    ]
)