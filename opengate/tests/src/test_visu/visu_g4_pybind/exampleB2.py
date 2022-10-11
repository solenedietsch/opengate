#!/usr/bin/env python3

import exampleB2 as b2

# UI: this will start qt interface
ui = b2.G4UIExecutive()

# world
detectorConstruction = b2.getDetectorConstruction()

# Run Manager
# no seg fault when G4RunManagerFactory is used instead of direct G4RunManager
runManager = b2.G4RunManagerFactory.CreateRunManager()
runManager.SetUserInitialization(detectorConstruction)

# PL
physicsList = b2.QBBC()
print('Physic list', physicsList)
runManager.SetUserInitialization(physicsList)

# Vis
visManager = b2.G4VisExecutive()
visManager.Initialize()

# init
runManager.Initialize()

# visu
commandName = b2.G4String("/control/execute vis.mac")
UImanager = b2.G4UImanager.GetUIpointer()
UImanager.ApplyCommand(commandName)
print("Start visu")

# go
ui.SessionStart()

# del runManager
# del physicsList
# del detectorConstruction

print()
print("DONE ")
