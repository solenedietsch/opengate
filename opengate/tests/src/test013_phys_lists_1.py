#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import opengate as gate
from test013_phys_lists_helpers import create_pl_sim, phys_em_parameters

paths = gate.get_default_test_paths(__file__, "gate_test013_phys_lists")

# create simulation
sim = create_pl_sim()
ui = sim.user_info
ui.g4_verbose = True

# remove ion sources
sim.source_manager.user_info_sources.pop("ion1")
sim.source_manager.user_info_sources.pop("ion2")

# change physics
p = sim.get_physics_user_info()
p.physics_list_name = "G4EmStandardPhysics_option4"
p.enable_decay = True

# em parameters
p.em_parameters["fluo"] = True
p.em_parameters["auger"] = True
p.em_parameters["auger_cascade"] = True
p.em_parameters["pixe"] = False
p.em_parameters["deexcitation_ignore_cut"] = True

um = gate.g4_units("um")
global_cut = 7 * um
sim.physics_manager.global_production_cuts.gamma = global_cut
sim.physics_manager.global_production_cuts.electron = global_cut
sim.physics_manager.global_production_cuts.positron = global_cut
sim.physics_manager.global_production_cuts.proton = global_cut

# print cuts
print("Phys list cuts:")
print(sim.physics_manager.dump_production_cuts())

# start simulation
# sim.apply_g4_command("/tracking/verbose 1")
sim.run()

# Gate mac/main_1.mac
stats = sim.output.get_actor("Stats")
stats_ref = gate.read_stat_file(paths.gate_output / "stat_1.txt")
is_ok = gate.assert_stats(stats, stats_ref, tolerance=0.1)

gate.test_ok(is_ok)
