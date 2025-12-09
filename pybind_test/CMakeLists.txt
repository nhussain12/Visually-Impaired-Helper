cmake_minimum_required(VERSION 3.14)
project(mybindings_project LANGUAGES CXX)

# Path to pybind11 CMake config (from `python3 -m pybind11 --cmakedir`)
set(pybind11_DIR "/opt/anaconda3/lib/python3.13/site-packages/pybind11/share/cmake/pybind11")

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(mybindings bind.cpp product.cpp)

set_property(TARGET mybindings PROPERTY CXX_STANDARD 17)
