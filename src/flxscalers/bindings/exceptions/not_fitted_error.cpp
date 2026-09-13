#include <pybind11/pybind11.h>

#include "flxscalers/bindings/register.hpp"
#include "flxscalers/exceptions/not_fitted_error.hpp"

namespace py = pybind11;

void register_not_fitted_error(py::module_& m) {
    py::register_exception<NotFittedError>(m, "NotFittedError");
}
