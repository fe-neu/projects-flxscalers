#ifndef FLXSCALERS_BINDINGS_CONVERSIONS_HPP
#define FLXSCALERS_BINDINGS_CONVERSIONS_HPP

// NumPy <-> Matrix marshalling: the only place pybind11 types meet Matrix.

#include <pybind11/numpy.h>

#include "flxscalers/core/matrix.hpp"

// Contiguous, row-major, float64; other layouts/dtypes are cast to fit.
using NpArray =
    pybind11::array_t<double, pybind11::array::c_style | pybind11::array::forcecast>;

// Requires a 2-D array, else throws pybind11::value_error.
Matrix as_matrix(const NpArray& array);
pybind11::array_t<double> as_numpy(const Matrix& m);

#endif
