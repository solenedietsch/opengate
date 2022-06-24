/* --------------------------------------------------
   Copyright (C): OpenGATE Collaboration
   This software is distributed under the terms
   of the GNU Lesser General  Public Licence (LGPL)
   See LICENSE.md for further details
   -------------------------------------------------- */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "GamImageBox.h"

void init_GamImageBox(py::module &m) {

    py::class_<GamImageBox>(m, "GamImageBox")
        .def(py::init<py::dict &>())
        .def("InitialiseSlice", &GamImageBox::InitialiseSlice);
}


