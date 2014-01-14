Oasis
=====

A prey-predator project written in C(++) with a GUI user interface.

#Modules

###action
Decision making based on a variety of internal and environmental factors :
* proximity to food
* energy levels
* number of entities on a given cell
* location

###error
Verification of test files.

###graphic
Rendering of the GUI interface.

###movement	
Returns next coordinates based on call from action module.

###save_data
Saves data for further investigation (graphs documenting evolution of populations over time).

###simulation
Main.
