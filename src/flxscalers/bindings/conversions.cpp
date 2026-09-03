#include "flxscalers/bindings/conversions.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>

namespace py = pybind11;

Matrix as_matrix(const NpArray& array) {
    if (array.ndim() != 2) {
        throw py::value_error("expected a 2-D array, got " +
                              std::to_string(array.ndim()) + "-D");
    }

    const auto rows = static_cast<std::size_t>(array.shape(0));
    const auto cols = static_cast<std::size_t>(array.shape(1));
    const double* buffer = array.data();
    return Matrix(rows, cols, std::vector<double>(buffer, buffer + rows * cols));
}

py::array_t<double> as_numpy(const Matrix& m) {
    py::array_t<double> out({static_cast<py::ssize_t>(m.rows),
                             static_cast<py::ssize_t>(m.cols)});
    std::copy(m.raw().begin(), m.raw().end(), out.mutable_data());
    return out;
}
