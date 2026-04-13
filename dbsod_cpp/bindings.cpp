/*
 * Copyright 2026 Ivan Danylenko
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dbsod.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(dbsod_cpp, m) {
    m.doc() = "DBSOD (Density-Based Spatial Outlier Detection) Python binding";

    m.def("dbsod",
        [](py::array_t<double, py::array::c_style> X,
           std::vector<double> eps_space,
           size_t min_pts)
        {
            size_t rows = static_cast<size_t>(X.shape(0));
            size_t cols = static_cast<size_t>(X.shape(1));

            // read-only data span
            std::span<const double> data(X.data(), rows * cols);

            return dbsod::dbsod(data, rows, cols, eps_space, min_pts);
        },
        py::arg("data"),
        py::arg("eps_space"),
        py::arg("min_pts"),
        "Calculates normalized outlierness scores using DBSOD algorithm."
    );
}
