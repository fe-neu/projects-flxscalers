cmake_minimum_required(VERSION 3.16...3.30)
project(flxscalers LANGUAGES CXX)

# --- Core C++ library ------------------------------------------------------
# The scaler/matrix sources are built once as a static library so that both
# the Python extension and the native Catch2 test binary can link the same
# object code. POSITION_INDEPENDENT_CODE is required because this static
# library is linked into the `_core` shared module.
add_library(flxscalers_core STATIC
    src/flxscalers/core/matrix.cpp
    src/flxscalers/scalers/min_max_scaler.cpp
)
target_include_directories(flxscalers_core PUBLIC src)
target_compile_features(flxscalers_core PUBLIC cxx_std_17)
set_target_properties(flxscalers_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# --- Python extension --------------------------------------------------------
# On by default (scikit-build-core drives the wheel build this way). Turn it
# off for a tests-only dev build that has no pybind11 in scope:
#   cmake -S . -B build-test -DFLXSCALERS_BUILD_TESTS=ON -DFLXSCALERS_BUILD_PYTHON=OFF
option(FLXSCALERS_BUILD_PYTHON "Build the pybind11 extension module" ON)
if(FLXSCALERS_BUILD_PYTHON)
    # Let pybind11 locate the interpreter scikit-build-core selected.
    set(PYBIND11_FINDPYTHON ON)
    find_package(pybind11 CONFIG REQUIRED)

    # The compiled module. Only the binding glue lives here; everything it
    # wraps comes from flxscalers_core. Add each new binding .cpp below.
    pybind11_add_module(_core
        src/flxscalers/bindings/_core.cpp
        src/flxscalers/bindings/conversions.cpp
        src/flxscalers/bindings/scalers/min_max_scaler.cpp
    )
    target_link_libraries(_core PRIVATE flxscalers_core)

    # Install into the `flxscalers` package -> importable as `flxscalers._core`.
    install(TARGETS _core DESTINATION flxscalers)
endif()

# --- Native C++ tests --------------------------------------------------------
# Off by default so `pip install` / wheel builds never need the test
# framework. Enable a dev build with:
#   cmake -S . -B build-test -DFLXSCALERS_BUILD_TESTS=ON
#   cmake --build build-test
#   ctest --test-dir build-test --output-on-failure
option(FLXSCALERS_BUILD_TESTS "Build the C++ unit tests" OFF)
if(FLXSCALERS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/cpp)
endif()
