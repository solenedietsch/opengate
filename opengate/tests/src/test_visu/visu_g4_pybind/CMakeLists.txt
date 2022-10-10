#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(B2)

set(CMAKE_CXX_STANDARD 17)

# Need Geant4
find_package(Geant4 REQUIRED OPTIONAL_COMPONENTS ui_all vis_all)
include(${Geant4_USE_FILE})
message(STATUS "OPENGATE - Geant4 version ${Geant4_VERSION}")


# Need pybind11
add_subdirectory(../../../../../core/external/pybind11 ${PROJECT_BINARY_DIR}/pybind11)

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)

pybind11_add_module(exampleB2
        ${PROJECT_SOURCE_DIR}/binding.cpp
        ${sources}
        )

target_link_libraries(exampleB2 PRIVATE pybind11::module ${Geant4_LIBRARIES})

configure_file(
    ${PROJECT_SOURCE_DIR}/exampleB2.py
    ${PROJECT_BINARY_DIR}/exampleB2.py
    COPYONLY
    )

