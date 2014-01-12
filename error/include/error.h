/* Project: Oasis
 * Nom: error.h
 * Description: This header file includes the necessary functions for
 * 				reporting the errors encountered in the input file.
 * IMPORTANT: 
 * All the index values mentioned below start from ZERO just like array 
 * indices.
 */

// You must call this function if the input is valid.
void input_is_valid(void);

// Call this function if the time value is missing.
void error_missing_time(void);


// Call this function if the time value is invalid.
// INPUT ARGUMENT:
// 		invalid_value: the invalid value detected
void error_invalid_time(int invalid_value);


// Call this function if the map size is missing.
void error_missing_map_size(void);


// Call this function if the map size is invalid.
// INPUT ARGUMENT:
// 		invalid_value: the invalid value detected
void error_invalid_map_size(int invalid_value);


// Call this function if the content of a cell is invalid. In other words,
// the energy value is out of limits.
// INPUT ARGUMENTS:
// 		row: The ROW number of the invalid cell in the input file.
//		column: The column number of the invalid cell in the input file.
//		invalid_value: The invalid value detected
// IMPORTANT: 
//		Please note that row and column values are not the x, y
// 		coordinates of the cell.
void error_invalid_cell(int row, int column, char invalid_value);


// Call this function if the prey number value is missing.
void error_missing_prey_number(void);

// Call this function if the prey number value is invalid: Either given 
// value is less than the number of preys listed or it is less than
// zero. Do not call it if the given value is greater than the number of
// the listed predators. You SHOULD call error_missing_prey instead.
// INPUT ARGUMENT:
// 		invalid_value: The invalid value detected
void error_invalid_prey_number(int invalid_value);


// Call this function if a prey is missing.
// INPUT ARGUMENT:
// 		prey_index: The index of the missing prey
void error_missing_prey(int prey_index);


// Call this function if the position of a prey is invalid.
// INPUT ARGUMENTS:
// 		prey_index: The index of the missing prey
//		invalid_x: X value of the invalid COORDINATE of the prey
//		invalid_y: Y value of the invalid COORDINATE of the prey
void error_invalid_prey_position(int prey_index, int invalid_x, int invalid_y);


// Call this function if the energy value of a prey is invalid.
// INPUT ARGUMENTS:
// 		prey_index: The index of the prey with invalid energy value
//		invalid_value: The invalid value detected
void error_invalid_prey_energy(int prey_index, float invalid_value);

// Call this function if the predator number value is missing.
void error_missing_predator_number(void);


// Call this function if the predator number value is invalid: Either given 
// value is less than the number of the listed predators or it is less than
// zero. Do not call it if the given value is greater than the number of
// the listed predators. You SHOULD call error_missing_predator instead.
// INPUT ARGUMENT:
// 		invalid_value: The invalid value detected
void error_invalid_predator_number(int invalid_value);


// Call this function if a predator is missing.
// INPUT ARGUMENT:
// 		pred_index: The index of the missing predator
void error_missing_predator(int pred_index);


// Call this function if the position of a predator is invalid.
// INPUT ARGUMENTS:
// 		pred_index: The index of the missing prey
//		invalid_x: X value of the invalid COORDINATE of the predator
//		invalid_y: Y value of the invalid COORDINATE of the predator
void error_invalid_predator_position(int pred_index, int invalid_x, int invalid_y);


// Call this function if the energy value of a pred is invalid.
// INPUT ARGUMENTS:
// 		pred_index: The index of the predator with invalid energy value
//		invalid_value: The invalid value detected
void error_invalid_predator_energy(int pred_index, float invalid_value);
