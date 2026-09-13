
#include <pybind11/pybind11.h>

#include "flxscalers/bindings/conversions.hpp"
#include "flxscalers/bindings/register.hpp"
#include "flxscalers/scalers/standard_scaler.hpp"

namespace py = pybind11;

void register_standard_scaler(py::module_& m) {
    py::class_<StandardScaler>(m, "StandardScaler")
        .def(py::init<bool, bool>(),
             py::arg("with_mean") = true,
             py::arg("with_std")= true)
        .def(
            "fit",
            [](py::object self, const NpArray& X) {
                self.cast<StandardScaler&>().fit(as_matrix(X));
                return self;  // sklearn-style: fit returns the estimator
            },
            py::arg("X"))
        .def(
            "transform",
            [](const StandardScaler& self, const NpArray& X) {
                return as_numpy(self.transform(as_matrix(X)));
            },
            py::arg("X"))
        .def(
            "fit_transform",
            [](StandardScaler& self, const NpArray& X) {
                return as_numpy(self.fit_transform(as_matrix(X)));
            },
            py::arg("X"))
        .def(
            "inverse_transform",
            [](const StandardScaler& self, const NpArray& X) {
                return as_numpy(self.inverse_transform(as_matrix(X)));
            },
            py::arg("X"));
}
