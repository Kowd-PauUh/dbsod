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

#include <vector>
#include <span>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(dbsod_cpp, m) {
    m.doc() = "DBSOD (Density-Based Spatial Outlier Detection) Python binding";

    m.def("dbsod",
        [](py::array_t<double, py::array::c_style | py::array::forcecast> X,
           py::array_t<double, py::array::c_style | py::array::forcecast> eps_space,
           size_t min_pts)
        {
            // validate input data
            if (X.ndim() != 2) {
                throw std::runtime_error("`X` must be a 2D array");
            }

            size_t rows = static_cast<size_t>(X.shape(0));
            size_t cols = static_cast<size_t>(X.shape(1));

            // read-only spans
            std::span<const double> data(X.data(), rows * cols);
            std::span<const double> eps_space_(eps_space.data(), eps_space.size());

            auto result = dbsod::dbsod(data, rows, cols, eps_space_, min_pts);
            return py::array_t<double>(result.size(), result.data());
        },
        py::arg("X"),
        py::arg("eps_space"),
        py::arg("min_pts"),
        "Calculates normalized outlierness scores using DBSOD algorithm."
    );

    py::class_<dbsod::DBSOD>(m, "DBSOD")
        .def(py::init([](py::array_t<double, py::array::c_style | py::array::forcecast> eps_space,
                         size_t min_pts)
                      {
                          std::span<const double> eps_space_(eps_space.data(), eps_space.size());  // read-only span
                          return dbsod::DBSOD(eps_space_, min_pts);
                      }),
                      py::arg("eps_space"),
                      py::arg("min_pts"))

        .def("fit",
            [](dbsod::DBSOD &self,
               py::array_t<double, py::array::c_style | py::array::forcecast> X)
            {
                // validate input data
                if (X.ndim() != 2) {
                    throw std::runtime_error("`X` must be a 2D array");
                }

                size_t rows = static_cast<size_t>(X.shape(0));
                size_t cols = static_cast<size_t>(X.shape(1));

                std::span<const double> data(X.data(), rows * cols);  // read-only span
                return &self.fit(data, rows, cols);
            },
            py::arg("X"),
            py::return_value_policy::reference_internal
        )

        .def("predict",
            [](dbsod::DBSOD &self,
               py::array_t<double, py::array::c_style | py::array::forcecast> X)
            {
                // validate input data
                if (X.ndim() != 2) {
                    throw std::runtime_error("`X` must be a 2D array");
                }

                size_t rows = static_cast<size_t>(X.shape(0));
                size_t cols = static_cast<size_t>(X.shape(1));

                std::span<const double> data(X.data(), rows * cols);  // read-only span
                auto result = self.predict(data, rows, cols);
                return py::array_t<double>(result.size(), result.data());
            },
            py::arg("X"),
            py::return_value_policy::reference_internal
        )

        .def_property_readonly("outlierness_score",
            [](const dbsod::DBSOD &self)
            {
                const auto &vec = self.outlierness_score;
                return py::array_t<double>(vec.size(), vec.data());
            }
        );
}
