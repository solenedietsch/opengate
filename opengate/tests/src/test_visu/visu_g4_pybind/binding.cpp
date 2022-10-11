#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "G4VUserDetectorConstruction.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4VVisManager.hh"
#include "QBBC.hh"
#include "DetectorConstruction.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4VModularPhysicsList.hh"
#include "G4String.hh"

namespace py = pybind11;

PYBIND11_MODULE(exampleB2, m) {

    // G4UIExecutive
    py::class_<G4UIExecutive>(m, "G4UIExecutive")
        .def(py::init<>([]() {
            G4int argc = 1;
            char *argv[1];
            argv[0] = (char *) malloc(strlen("qt") + 1); // +1 for the terminator
            strcpy(argv[0], "qt");
            return new G4UIExecutive(argc, argv);
        }), py::return_value_policy::reference)
        .def("SessionStart", &G4UIExecutive::SessionStart);

    // G4VUserDetectorConstruction
    py::class_<G4VUserDetectorConstruction>(m, "G4VUserDetectorConstruction");

    // DetectorConstruction
    py::class_<DetectorConstruction, G4VUserDetectorConstruction>(m, "DetectorConstruction")
        .def(py::init(), py::return_value_policy::reference);
    m.def("getDetectorConstruction",
          []() { return std::unique_ptr<G4VUserDetectorConstruction>(new DetectorConstruction); },
          py::return_value_policy::reference);

    // G4VUserPhysicsList
    py::class_<G4VUserPhysicsList>(m, "G4VUserPhysicsList");

    // G4VModularPhysicsList
    py::class_<G4VModularPhysicsList, G4VUserPhysicsList>(m, "G4VModularPhysicsList")
        .def("SetVerboseLevel", &G4VModularPhysicsList::SetVerboseLevel);

    // G4PhysListFactory
    py::class_<G4PhysListFactory>(m, "G4PhysListFactory")
        .def(py::init(), py::return_value_policy::reference)
        .def("GetReferencePhysList", &G4PhysListFactory::GetReferencePhysList, py::return_value_policy::reference);

    // G4VModularPhysicsList
    // It is important to keep inheritance chain: G4VModularPhysicsList and G4VUserPhysicsList
    py::class_<QBBC, G4VModularPhysicsList, G4VUserPhysicsList>(m, "QBBC")
        .def(py::init(), py::return_value_policy::reference);

    // G4VPhysicsConstructor
    py::class_<G4VPhysicsConstructor>(m, "G4VPhysicsConstructor");

    // G4EmStandardPhysics_option4
    py::class_<G4EmStandardPhysics_option4, G4VPhysicsConstructor,
        std::unique_ptr<G4EmStandardPhysics_option4, py::nodelete>>(m, "G4EmStandardPhysics_option4")
        .def(py::init<G4int>(), py::return_value_policy::reference);

    // G4RunManagerFactory
    py::class_<G4RunManagerFactory, std::unique_ptr<G4RunManagerFactory, py::nodelete>>(m, "G4RunManagerFactory")
        .def("CreateRunManager",
             []() { return G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default); },
             py::return_value_policy::reference);

    // G4RunManager
    py::class_<G4RunManager>(m, "G4RunManager")
        .def(py::init(), py::return_value_policy::reference)
        .def("Initialize", &G4RunManager::Initialize, py::return_value_policy::reference)
        .def("SetUserInitialization",
             py::overload_cast<G4VUserPhysicsList *>(&G4RunManager::SetUserInitialization))

        .def("SetUserInitialization",
             py::overload_cast<G4VUserDetectorConstruction *>(&G4RunManager::SetUserInitialization));

    // G4VVisManager
    py::class_<G4VVisManager>(m, "G4VVisManager");

    // G4VisManager
    py::class_<G4VisManager, G4VVisManager>(m, "G4VisManager");

    // G4VisExecutive
    py::class_<G4VisExecutive, G4VisManager>(m, "G4VisExecutive")
        .def(py::init(), py::return_value_policy::reference)
        .def("Initialize", &G4VisExecutive::Initialize, py::return_value_policy::reference);

    // G4UImanager
    py::class_<G4UImanager>(m, "G4UImanager")
        .def("ApplyCommand",
             py::overload_cast<const G4String &>(&G4UImanager::ApplyCommand),
             py::return_value_policy::reference)
        .def_static("GetUIpointer", &G4UImanager::GetUIpointer, py::return_value_policy::reference);

    // G4String
    py::class_<G4String>(m, "G4String")
        .def(py::init<const std::string &>(), py::return_value_policy::reference);


}

