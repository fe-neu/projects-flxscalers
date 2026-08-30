cmake_minimum_required(VERSION 3.15...3.30)
project(flxscalers LANGUAGES CXX)

# Let pybind11 locate the interpreter scikit-build-core selected.
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

# The compiled module. Add every scaler's .cpp to this list as you create it.
pybind11_add_module(_core
    src/flxscalers/bindings.cpp
    src/flxscalers/scalers/min_max_scaler.cpp
)

# So headers can be included as "flxscalers/scalers/min_max_scaler.hpp".
target_include_directories(_core PRIVATE src)
target_compile_features(_core PRIVATE cxx_std_17)

# Install into the `flxscalers` package -> importable as `flxscalers._core`.
install(TARGETS _core DESTINATION flxscalers)
