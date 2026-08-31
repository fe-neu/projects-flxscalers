// The package's single PYBIND11_MODULE; body is just register_* calls.

#include <pybind11/pybind11.h>

#include "flxscalers/bindings/register.hpp"

PYBIND11_MODULE(_core, m) {
    m.doc() = "Compiled core for flxscalers. Internal; use the flxscalers package.";

    register_min_max_scaler(m);
}
