#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // std::pair <-> tuple, for feature_range

#include "flxscalers/bindings/conversions.hpp"
#include "flxscalers/bindings/register.hpp"
#include "flxscalers/scalers/min_max_scaler.hpp"

namespace py = pybind11;

void register_min_max_scaler(py::module_& m) {
    py::class_<MinMaxScaler>(m, "MinMaxScaler")
        .def(py::init<std::pair<double, double>>(),
             py::arg("feature_range") = std::pair<double, double>(0.0, 1.0))
        .def(
            "fit",
            [](py::object self, const NpArray& X) {
                self.cast<MinMaxScaler&>().fit(as_matrix(X));
                return self;  // sklearn-style: fit returns the estimator
            },
            py::arg("X"))
        .def(
            "transform",
            [](const MinMaxScaler& self, const NpArray& X) {
                return as_numpy(self.transform(as_matrix(X)));
            },
            py::arg("X"))
        .def(
            "fit_transform",
            [](MinMaxScaler& self, const NpArray& X) {
                return as_numpy(self.fit_transform(as_matrix(X)));
            },
            py::arg("X"))
        .def(
            "inverse_transform",
            [](const MinMaxScaler& self, const NpArray& X) {
                return as_numpy(self.inverse_transform(as_matrix(X)));
            },
            py::arg("X"));
}
