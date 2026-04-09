/*
 * Copyright 2025 Ivan Danylenko
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

namespace py = pybind11;

PYBIND11_MODULE(dbsod_cpp, m) {
    m.doc() = "DBSOD (Density-Based Spatial Outlier Detection) Python binding";

    m.def("dbsod", &dbsod,
          py::arg("data"),
          py::arg("metric"),
          py::arg("epsSpace"),
          py::arg("minPts"),
          "Calculates normalized outlierness scores using DBSOD algorithm.");
}
