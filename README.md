Oasis
=====

A prey-predator project written in C(++) with a GUI user interface.

#Modules

### Action
Decision making based on a variety of internal and environmental factors :
* proximity to food
* energy levels
* number of entities on a given cell
* location

### Error
Verification of test files.

### Graphic
Rendering of the GUI interface.

### Movement	
Returns next coordinates based on call from action module.

### Save_data
Saves data for further investigation (graphs documenting evolution of populations over time).

### Simulation
Reads and analyzes test file.

#Test Files

```
#Test number

5 # Clock

1 # m with number of cells = 2*m+1
12.
900   # cell energy levels (0<energy<9, with '.' being sterile)
831

1 #number of preys

0 0 1 5 # (x, y, alive, energy) 

1 #number of predators

0 0 5 # (x,y,energy)
```
