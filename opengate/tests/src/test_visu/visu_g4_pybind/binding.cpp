#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "G4VUserDetectorConstruction.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4VVisManager.hh"
#include "QBBC.hh"
#include "DetectorConstruction.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4String.hh"

namespace py = pybind11;

PYBIND11_MODULE(exampleB2, m) {
    py::class_<G4UIExecutive>(m, "G4UIExecutive")
        .def(py::init<>([]() {
            G4int argc = 1;
            char *argv[1];
            argv[0] = (char*) malloc(strlen("qt")+1); // +1 for the terminator
            strcpy(argv[0],"qt");
            return new G4UIExecutive(argc, argv);
        }), py::return_value_policy::reference)
        .def("SessionStart", &G4UIExecutive::SessionStart);

    py::class_<G4VUserDetectorConstruction>(m, "G4VUserDetectorConstruction");
    py::class_<DetectorConstruction, G4VUserDetectorConstruction>(m, "DetectorConstruction")
      .def(py::init(), py::return_value_policy::reference);
    m.def("getDetectorConstruction", []() { return std::unique_ptr<G4VUserDetectorConstruction>(new DetectorConstruction); }, py::return_value_policy::reference);

    py::class_<G4VUserPhysicsList>(m, "G4VUserPhysicsList");
    py::class_<G4VModularPhysicsList, G4VUserPhysicsList>(m, "G4VModularPhysicsList");
    py::class_<G4PhysListFactory>(m, "G4PhysListFactory")
      .def(py::init())
      .def("GetReferencePhysList", &G4PhysListFactory::GetReferencePhysList, py::return_value_policy::reference);

    py::class_<G4RunManager>(m, "G4RunManager")
      .def(py::init(),py::return_value_policy::reference)
      .def("Initialize", &G4RunManager::Initialize, py::return_value_policy::reference)
      .def("SetUserInitialization", py::overload_cast<G4VUserPhysicsList*>(&G4RunManager::SetUserInitialization), py::return_value_policy::reference)
      .def("SetUserInitialization", py::overload_cast<G4VUserDetectorConstruction*>(&G4RunManager::SetUserInitialization), py::return_value_policy::reference);

    py::class_<G4VVisManager>(m, "G4VVisManager");
    py::class_<G4VisManager, G4VVisManager>(m, "G4VisManager");
    py::class_<G4VisExecutive, G4VisManager>(m, "G4VisExecutive")
      .def(py::init(), py::return_value_policy::reference)
      .def("Initialize", &G4VisExecutive::Initialize, py::return_value_policy::reference);

    py::class_<G4UImanager>(m, "G4UImanager")
      .def("ApplyCommand", py::overload_cast<const G4String &>(&G4UImanager::ApplyCommand))
      .def_static("GetUIpointer", &G4UImanager::GetUIpointer, py::return_value_policy::reference);

    py::class_<G4String>(m, "G4String")
      .def(py::init<const std::string &>(), py::return_value_policy::reference);


}

