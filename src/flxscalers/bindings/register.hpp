#ifndef FLXSCALERS_BINDINGS_REGISTER_HPP
#define FLXSCALERS_BINDINGS_REGISTER_HPP

// One register_* per scaler, each defined in its own TU, all called from
// _core.cpp. Matrix is intentionally absent - it stays internal.

#include <pybind11/pybind11.h>

void register_min_max_scaler(pybind11::module_& m);

#endif
