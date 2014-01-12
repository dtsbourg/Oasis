/* Nom : test.c
 * Description : Code source des fonctions de test des valeurs transmises
 * Date de dernière MAJ : 23.03.2013
 * Auteur : Dylan Bourgeois
 * Version : 1.0
 */     

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../error/include/error.h"
#include "test.h"
#include "simulation.h"
#include "../constantes.h"

int test_clock (int clock_init)
{
	if ( clock_init < 0 ) 
	{
		error_invalid_time (clock_init);
		return false;
	}
	else return true;
}

int test_oasis_size_parameter (int oasis_size_parameter)
{
	if ( oasis_size_parameter < 0 || oasis_size_parameter > 100)
	{
		error_invalid_map_size (oasis_size_parameter);
		return false;
	}
	
	else return true;
}

int test_oasis_cell (char cell, int i, int j)
{
	
	if ( (int) cell < '.' || (int) cell > ENERGIE_MAX + 2 + '.' || (int) cell == '/' ) 
	{
		error_invalid_cell (i, j, cell);
		return false;
	}
		
	else return true;
}


int test_number (int number, int param, int oasis_size)
{
	if (number < 0)
	{
		switch (param)
		{
			case PREY_NUMBER:
				error_invalid_prey_number (number);
				break;
				
			case PREDATOR_NUMBER:
				error_invalid_predator_number (number);
				break;
			
		}
		
		return false;
	}
	
	else return true;
}

int test_coordinates (int i, int number, int oasis_size, int param, int coord_x, int coord_y, float energy)
{
	switch (param)
	{
		case PREY_COORDINATES:
			if ( coord_x >= DESERT_SIZE+oasis_size
			  || coord_y >= DESERT_SIZE+oasis_size )
			  
			  {
				 error_invalid_prey_position (i, coord_x, coord_y); 
				 return false;
			  }
			  
			if ( energy > ENERGIE_MAX || energy < 0 )
			{
				error_invalid_prey_energy (i, energy);
				return false;
			}
			
			else return true;
			
		case PREDATOR_COORDINATES:
			if ( coord_x >= DESERT_SIZE+oasis_size
			  || coord_y >= DESERT_SIZE+oasis_size )
			  
			  {
				 error_invalid_predator_position (i, coord_x, coord_y); 
				 return false;
			  }
			  
			if ( energy > ENERGIE_MAX || energy < 0 )
			{
				error_invalid_predator_energy (i, energy);
				return false;
			}
			
			else return true;
		
		default: return false;

	
	}
}
