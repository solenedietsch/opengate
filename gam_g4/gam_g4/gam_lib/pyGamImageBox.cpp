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
#include "GamHelpers.h"

/*
 * The "trampoline" functions below are required if we want to
 * allow callbacks on the py side.
 *
 * If it is not needed: to not define trampoline functions in class that inherit from VActor.
 *
 * It must be defined also in all classes that inherit from GamVActor
 *
 * Hence, BeginOfRunAction, BeginOfEventAction etc maybe define in py side
 * (but it will be slower, especially for steps)
 */

class PyGamImageBox : public GamImageBox {
public:
    // Inherit the constructors
    using GamImageBox::GamImageBox;


    void DescribeYourselfTo(G4VGraphicsScene* scene) override {
        PYBIND11_OVERLOAD(void, GamImageBox, G4VGraphicsScene, scene);
    }

    void setResolutionX(const size_t x) override {
        PYBIND11_OVERLOAD(void, GamImageBox, setResolutionX, x);
    }

};

void init_GamImageBox(py::module &m) {

    py::class_<GamImageBox, PyGamImageBox,
        std::unique_ptr<GamImageBox, py::nodelete>>(m, "GamImageBox")
        .def(py::init<py::dict &>())
        .def("DescribeYourselfTo", &GamVActor::DescribeYourselfTo);
        .def("setResolutionX", &GamVActor::setResolutionX);
}

