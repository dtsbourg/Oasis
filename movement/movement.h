/* Project: Oasis
 * Nom: movement.h
 * Description: This header file includes the function to fulfill the 
 * 				task noted for Rendu1 Task2. You can add more functions
 * 				into it if you want as long as you DO NOT CHANGE THE
 * 				prototype of estimate_movement
 */


// Given the coordinate of a cell, the coordinate of the goal cell
// and the size of the step to be taken, it returns the coordinate of
// the cell that the creature should move into according to the rules
// specified in Rendu1 Task2
// INPUT ARGUMENTS:
//		src_x: x value of the coordinate of the starting point
//		src_y: y value of the coordinate of the starting point
//		des_x: x value of the coordinate of the destination point
//		des_y: y value of the coordinate of the destination point
//		step_size: Size of the step to be taken
//		res_x: Pointer to the x value of the coordinate of the estimated
//			   cell coordinate. In other words, the return value.
//		res_y: Pointer to the y value of the coordinate of the estimated
//			   cell coordinate. In other words, the return value.

#ifndef MOVEMENT_H
#define MOVEMENT_H

void estimate_movement(int src_x, int src_y, 
					   int des_x, int des_y, 
					   int step_size, 
					   int* res_x, int* res_y);

int movement_calc_depl(int, int,int, int);
#endif
