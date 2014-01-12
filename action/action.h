#include <stdbool.h>

/* Project: Oasis
 * Nom: action.h
 * Description: This header file includes the necessary functions for
 * 				reporting the actions of the preys and predators 
 * 				in every step update.
 */

// You MUST call this function BEFORE you start reporting the actions of
// the preys.
void start_prey_actions(void);


// You MUST call this function WHEN you are done with reporting the 
// actions of the preys.
void end_prey_actions(void);


// Call this function to report the movement of a prey.
// INPUT ARGUMENTS:
// 		energy: Energy of the prey BEFORE the movement
//		from_x: x value of the coordinate of the prey before the movement
//		from y: y value of the coordinate of the prey before the movement
//		to_x: 	x value of the new coordinate of the prey
//		to_y: 	y value of the new coordinate of the prey
void action_prey_moved(float energy, 
					   int from_x, int from_y, 
					   int to_x, int to_y);
					   

// Call this function to report the reproduction of a prey.
// INPUT ARGUMENTS:
// 		energy: Energy of the prey BEFORE the reproduction
//		x: 		x value of the coordinate of the prey
//		y: 		y value of the coordinate of the prey
void action_prey_reproduced(float energy, int x, int y);


// Call this function to report when a prey feeds itself.
// INPUT ARGUMENTS:
// 		energy: Energy of the prey BEFORE the nourishment
//		x:		x value of the coordinate of the prey
//		y:		y value of the coordinate of the prey
void action_prey_fed(float energy, int x, int y);


// Call this function to report that a prey has no more energy.
// Note that you must call this function at the moment where the prey
// doesn't have any more energy to survive. 
// It can happen either in the update of a prey (in this case this 
// function is called in the prey actions section) or, after the 
// nourishment of a predator (in this case it is called in the predator 
// actions just after action_predator_fed).
// INPUT ARGUMENTS:
// 		energy: 	Energy of the prey (must be less than or equal to zero)
//		captured:	true if prey has been captured, false, otherwise
//		x:			x value of the coordinate of the prey
//		y:			y value of the coordinate of the prey
void action_prey_no_energy(float energy, bool captured, int x, int y);


// You MUST call this function BEFORE you start reporting the actions of
// the predators.
void start_predator_actions(void);


// You MUST call this function WHEN you are done with reporting the 
// actions of the predators.
void end_predator_actions(void);


// Call this function to report the movement of a predator.
// INPUT ARGUMENTS:
// 		energy: Energy of the predator BEFORE the movement
//		from_x: x value of the coordinate of the predator before the movement
//		from y: y value of the coordinate of the predator before the movement
//		to_x: 	x value of the new coordinate of the predator
//		to_y: 	y value of the new coordinate of the predator
//		dest_x:	x value of the coordinate of the goal of the predator
//		dest_y: y value of the coordinate of the goal of the predator
void action_predator_moved(float energy,
						   int from_x, int from_y, 
						   int to_x, int to_y,
						   int dest_x, int dest_y);
						   

// Call this function to report the reproduction of a predator.
// INPUT ARGUMENTS:
// 		energy: Energy of the predator BEFORE the reproduction
//		x: 		x value of the coordinate of the predator
//		y: 		y value of the coordinate of the predator
void action_predator_reproduced(float energy, int x, int y);


// Call this function to report when a predator feeds itself.
// Note that if a predator feeds itself from a prey, the prey's energy 
// value must be decreased immediately. And, if another predator feeds
// itself from the same prey in the same update step, 
// you must call this function with the updated energy value of the prey 
// for reporting the new action.
// INPUT ARGUMENTS:
// 		energy_predator: Energy of the predator BEFORE the nourishment
//		x:				 x value of the coordinate of the predator
//		y:				 y value of the coordinate of the predator
// 		energy_prey: 	 Energy of the prey BEFORE the nourishment
void action_predator_fed(float energy_predator, 
						 int x, int y,
						 float energy_prey);
						 
						 
// Call this function to report when a predator feeds itself.
// INPUT ARGUMENTS:
// 		energy_predator: Energy of the predator BEFORE the capture.
//						 Note that if a predator moves into another cell
//						 and then captures a prey, you MUST pass its 
//						 energy AFTER the movement.
//		x:				 x value of the coordinate of the predator
//		y:				 y value of the coordinate of the predator
// 		energy_prey: 	 Energy of the prey BEFORE the capture
void action_predator_captured(float energy_predator, 
							  int x, int y,
							  float energy_prey);


// Call this function to report if a predator has no more energy.
// INPUT ARGUMENTS:
// 		energy: 	Energy of the predator (must be less than or equal to zero)
//		x:			x value of the coordinate of the predator
//		y:			y value of the coordinate of the predator
void action_predator_no_energy(float energy, int x, int y);


