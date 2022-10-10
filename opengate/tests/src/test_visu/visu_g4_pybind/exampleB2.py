#!/usr/bin/env python3

import exampleB2 as b2

def visu_g4_pybind():
    ui = b2.G4UIExecutive()
    detectorConstruction = b2.getDetectorConstruction()
    runManager = b2.G4RunManager()
    runManager.SetUserInitialization(detectorConstruction)
    physListName = b2.G4String("QBBC")
    factory = b2.G4PhysListFactory()
    physicsList = factory.GetReferencePhysList(physListName)
    print(physicsList)
    runManager.SetUserInitialization(physicsList)

    visManager = b2.G4VisExecutive()
    #visManager.Initialize()

    #UImanager = b2.G4UImanager.GetUIpointer();

    runManager.Initialize()

    del runManager

    print("done")

# -----------------------------------------------------------------------------
if __name__ == '__main__':
    visu_g4_pybind()
