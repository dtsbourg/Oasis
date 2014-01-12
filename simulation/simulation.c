/* Nom : simulation.c
 * Description : Contient les prototypes des deux fonctions de lecture 
 * de données et d'appel à des vérifications de ces dernières, ainsi qu'un
 * ensemble de fonctions assurant la répartition des données de façon opaque
 * dans tout le programme (fonction save/get).
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulation.h"
#include "../error/include/error.h"
#include "test.h"
#include "../constantes.h"
#include "../save_data/cell.h"
#include "../save_data/predator.h"
#include "../save_data/prey.h"

#define X 0
#define Y 1

//Fichier de test
static FILE *test_file = NULL;


int oasis;
int prey_x, prey_y, prey_bool, pred_x, pred_y;
float prey_energy, pred_energy;
char c_cell;
int i;

//Variables globales au module, nécessaires à la simulation
static int horloge=0, size=0, furthest=0, world_size=0;
static int pred_nb=0, prey_nb=0;

static int x_mouse=0, y_mouse=0;

static int start=0;
static int zoom=0;
static int m=0;

static int output=0, verbose_status = 0, mouse_status=0, va_status = 1;

int sim_input (char *file_name)
{
	char lign[220];     //tab to store data read on stdin
	int status = CLOCK;
	int * ptr=NULL;
	int j=0;
	ptr=&j;
	
	test_file = fopen(file_name, "r");
	
	if (test_file == NULL) 
	{
		printf ("File %s is empty or does not exist. \n", file_name);
		status = EXIT;
		return status;
	}
	
	else 
	{
		while ( status != EXIT )
		{
			if (( fgets(lign, 220, test_file) ))
			{
				if ( lign[0] == '#' || lign[0] == '\n' || lign[0] == '\r')
				continue;
				
				else sim_input_test (lign, &status, ptr);
			}
			
			else break;
		} 
		
		if ( !(fgets(lign, 220, test_file)) && *ptr < pred_nb ) 
		{
			error_missing_predator (*ptr);
			status=EXIT;
			return status;
		}
		fclose(test_file);
		return status;
	}
	
	fclose(test_file);

	return status;
}


void sim_input_test (char lign[220], int *status, int *ptr)
{
	int j;
	
	switch (*status)
	{
		case CLOCK:
			if ( sscanf (lign, "%d", &horloge) )
			{
				if ( test_clock (horloge) )
				{
					sim_save_time (horloge);
					*status=OASIS_SIZE_PARAMETER;
					break;
				}
				
				else 
				{
					*status=EXIT;
					break;
				}
			}
			
			else 
			{
				error_missing_time();
				*status = EXIT;
				break;
			}
		
		case OASIS_SIZE_PARAMETER:
			if ( (sscanf(lign, "%d", &oasis)) )
			{
				if ( (test_oasis_size_parameter (oasis) ) )
				{
					oasis = 2*(oasis) + 1;
					sim_save_oasis(oasis);
					cell_create_tab(oasis);
					*status=OASIS;
					i=0;
					break;
				}
				
				else 
				{
					*status=EXIT;
					break;
				}
				
			}
			
			else 
			{
				error_missing_map_size();
				*status=EXIT;
				break;
			}
			
		case OASIS:
			for (j=0; j<oasis;j++)
			{
				if ( (sscanf (lign, "%c", &c_cell) ) )
				{
					if ( (test_oasis_cell(c_cell, i, j)) )
					{
						cell_add(oasis-1+DESERT_SIZE-i, j+DESERT_SIZE, c_cell);
						c_cell=0;
						lign++;
					}
					
					else
					{
						*status=EXIT;
						break;
					}	
				}
			}
			
			i++;
			
			if (i==oasis) 
			{
				*status = PREY_NUMBER;
				i=0;
				break;
			}
			
			break;
			
		case PREY_NUMBER:
			if( (sscanf(lign, "%d", &prey_nb)) && (test_number (prey_nb, PREY_NUMBER, oasis) ) )
			{
				sim_save_prey_nb(prey_nb);
				*status=PREY_COORDINATES;
				break;
			}
			 
			else 
			{
				error_missing_prey_number();
				*status=EXIT;
				break;
			}
			
		case PREY_COORDINATES:
			if ( (sscanf (lign, " %d %d %d %f ", &prey_x, &prey_y, &prey_bool, &prey_energy)) )
			{				
				if(test_coordinates (i, prey_nb, oasis, PREY_COORDINATES, prey_x, prey_y, prey_energy))
				{
					prey_add(prey_x, prey_y, prey_bool, prey_energy);
				
					if (i==prey_nb-1)
					{
						i=0;
						*status=PREDATOR_NUMBER;
						break;
					}
					
					if (i<prey_nb-1)
					{
						i++;
						break;
					}
				}
				
				else 
				{
					*status=EXIT;
					break;
				}
			}
			
			else 
			{
				*status = EXIT;
				break; 
			}
			
		
		case PREDATOR_NUMBER:
			if( (sscanf(lign, "%d", &pred_nb)) && (test_number (pred_nb, PREDATOR_NUMBER, oasis) ) )
			{
				sim_save_pred_nb(pred_nb);
				*status=PREDATOR_COORDINATES;
				break;
			}
			 
			else 
			{
				error_missing_predator_number();
				*status=EXIT;
				break;
			}
			
		case PREDATOR_COORDINATES:	
			if ( (sscanf (lign, " %d %d %f ", &pred_x, &pred_y, &pred_energy)) )
			{		
				pred_add(pred_x, pred_y, pred_energy);
				
				(*ptr)++;

				if(test_coordinates (i, pred_nb, oasis, PREDATOR_COORDINATES, pred_x, pred_y, pred_energy))
				{
					if (i==pred_nb-1)
					{
						input_is_valid();
						*status=CORRECT;
						break;
					}
					
					if (i<pred_nb-1)
					{
						i++;	
					}
				}
				
				else 
				{
					*status=EXIT;
					break;
				}
			}
			
			else 
			{
				*status = EXIT;
				break; 
			}
			
		case EXIT:
			break;
	}
	
}


void simulation_update ()
{
	cell_update();
	prey_update();
	pred_update();
}


int sim_find_furthest ()
{
	PREY * furthest_prey = prey_get_tete_list();
	PREDATOR * furthest_pred = pred_get_tete_list();
	int furthest_prey_tab[2]={0,0}, furthest_pred_tab[2]={0,0};
	
	if (furthest_prey)
		{
			furthest_prey_tab[X]=abs(prey_get_x(furthest_prey));
			furthest_prey_tab[Y]=abs(prey_get_y(furthest_prey));
		}
	
	if (furthest_pred)
		{
			furthest_pred_tab[X]=abs(pred_get_x(furthest_pred));
			furthest_pred_tab[Y]=abs(pred_get_y(furthest_pred));
		}
	
	int furthest_p = 0, furthest_pr = 0;
	m = ((sim_get_oasis()-1)/2);

	while (furthest_prey)
	{
		if (abs(prey_get_x(furthest_prey)) > furthest_prey_tab[X] ) furthest_prey_tab[X] = abs(prey_get_x(furthest_prey));
		if (abs(prey_get_y(furthest_prey)) > furthest_prey_tab[Y] ) furthest_prey_tab[Y] = abs(prey_get_y(furthest_prey));
		
		furthest_prey=prey_get_next(furthest_prey);
	}
	
	while (furthest_pred)
	{
		if (abs(pred_get_x(furthest_pred)) > furthest_pred_tab[X] ) furthest_pred_tab[X] = abs(pred_get_x(furthest_pred));
		if (abs(pred_get_y(furthest_pred)) > furthest_pred_tab[Y] ) furthest_pred_tab[Y] = abs(pred_get_y(furthest_pred));
		
		furthest_pred=pred_get_next(furthest_pred);
	}
	
	if ( furthest_prey_tab[X] > furthest_prey_tab[Y] ) furthest_p = furthest_prey_tab[0];
	else furthest_p = furthest_prey_tab[Y];
	
	if (furthest_pred_tab[X] > furthest_pred_tab[Y]) furthest_pr = furthest_pred_tab[0];
	else furthest_pr = furthest_pred_tab[Y];
	
	if ( furthest_p > furthest_pr ) furthest = furthest_p;
	else furthest = furthest_pr;

	if (furthest < m) return m;
	else return furthest;
}



//TIME
void sim_save_time (int time)
{
	horloge = time;
}

int sim_get_time ()
{
	return horloge;
}

//OASIS
void sim_save_oasis (int oasis)
{
	size = oasis;
}

int sim_get_oasis (void)
{
	return size;
}

int sim_get_size (int furthest, int oasis)
{
	if (furthest > oasis) world_size = 2*furthest+1;
	else world_size = oasis;

	return world_size;
}

//CLEAR
void sim_reset ()
{
	cell_free_all();
	pred_free_all();
	prey_free_all();
}

//STATUS
void sim_save_status (int status)
{
	start = status;
}
int sim_get_status ()
{
	return start;
}


void sim_save_prey_nb (int prey_number)
{
	prey_nb = prey_number;
}

int sim_get_prey_nb(void)
{
	return prey_nb;
}

void sim_save_pred_nb (int predator_nb)
{
	pred_nb = predator_nb;
}
	
int sim_get_pred_nb (void)
{
	return pred_nb;
}

void sim_save_mouse (int x, int y)
{
	x_mouse = x;
	y_mouse = y;
}

int sim_get_x_mouse (void)
{
	return x_mouse;
}

int sim_get_y_mouse (void)
{
	return y_mouse;
}

void sim_save_zoom (int z)
{
	zoom=z;
}

int sim_get_zoom (void)
{
	return zoom;
}

void sim_save_output_status(int out)
{
	output = out;
}

int sim_get_output_status (void)
{
	return output;
}

void sim_save_verbose_status(int verbose)
{
	verbose_status = verbose;
}

int sim_get_verbose_status (void)
{
	return verbose_status;
}

void sim_save_mouse_status(int status)
{
	mouse_status = status;
}

int sim_get_mouse_status(void)
{
	return mouse_status;
}

void sim_save_va_status(int status)
{
	va_status = status;
}

int sim_get_va_status(void)
{
	return va_status;
}
