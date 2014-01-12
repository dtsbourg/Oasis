/* Nom : predator.c
 * Description : Module ayant attrait aux prédateurs. on y définit les structures,
 * on y crée et libère l'esapce alloué pour elles (sous forme de liste chaînée)
 * et on y trouve un ensemble de fonction get/save rendant le module opaque.
 * La gestion de la simulation au niveau des prédateurs est aussi gérée ici.
 * Date de dernière MAJ : 23.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */  

#include <stdio.h>
#include <stdlib.h>
#include "predator.h"
#include "cell.h"
#include "../simulation/simulation.h"
#include "../constantes.h"
#include "prey.h"
#include "../movement/movement.h"
#include "../action/action.h"
#include <time.h>

#define X 0
#define Y 1
#define ALIVE 1
#define DEAD 0
#define VB_ID 9

enum Decision {REPRO, EAT, MOVED, STAY, DIED};


struct Predator
{
	int coord_pred[2];
	float energy_pred;
	int bool_pred;
	PREDATOR * next_pred;
};

static PREDATOR * tete_pred_list=NULL;
static PREDATOR ** p_tete_pred_list = &tete_pred_list;


void pred_add (int x, int y, float energy)
{
	PREDATOR *new_pred = NULL;
	
	if (!(new_pred = (PREDATOR*) malloc(sizeof(PREDATOR))))
	{
		printf ("Predator Allocation Error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}
	
	new_pred->coord_pred[X]=x;
	new_pred->coord_pred[Y]=y;
	new_pred->bool_pred=ALIVE;
	new_pred->energy_pred=energy;
	
	new_pred->next_pred = tete_pred_list;
	*p_tete_pred_list = new_pred;
	
	cell_add_pred(x, y, new_pred);
}


int pred_get_x (PREDATOR * p_pred)
{
	if (p_pred)
		return (p_pred->coord_pred[X]);
}

int pred_get_y (PREDATOR * p_pred)
{
	if (p_pred)
		return (p_pred->coord_pred[Y]);
}

float pred_get_energy (PREDATOR * p_pred)
{
	if (p_pred)
		return (p_pred->energy_pred);
}


PREDATOR * pred_get_next (PREDATOR * p_pred)
{
	return (p_pred->next_pred);
}

PREDATOR * pred_get_tete_list (void)
{
	return (tete_pred_list);
}

void pred_free (PREDATOR * elem)
{
	PREDATOR * courant=*p_tete_pred_list;
	PREDATOR * precedent=*p_tete_pred_list;
	
	while (courant != elem && courant != NULL)
	{
		precedent = courant;
		courant = courant->next_pred;
	}
	
	if (courant != NULL)
	{
		if (courant != *p_tete_pred_list)
		{
			precedent->next_pred=courant->next_pred;
			free (courant);
			courant=NULL;
		}
		
		if (courant == *p_tete_pred_list)
		{
			*p_tete_pred_list=courant->next_pred;
			free (courant);
			courant=NULL;
		}
	}
}

void pred_free_all(void)
{
	PREDATOR * elem;
	
	while (*p_tete_pred_list != NULL)
	{
		elem = *p_tete_pred_list;
		pred_free(elem);
	}
}


void pred_update (void)
{
	PREDATOR * pred = tete_pred_list;
		
	if (sim_get_verbose_status()) start_predator_actions();
	
	while (pred)
	{
	
		PREDATOR * n_pred = pred->next_pred;
		int src_x = pred->coord_pred[0];
		int src_y = pred->coord_pred[1];
		int m=(sim_get_oasis()-1)/2;
		
		if (pred_decision(src_y+DESERT_SIZE+m, src_x+DESERT_SIZE+m, pred) == REPRO)
		{
			pred_reproduce(pred);
		}
		pred=n_pred;
	}
	
	if (sim_get_verbose_status()) 
	{
		end_predator_actions();
		save_out_txt (VB_ID);
	}
}


void pred_reproduce (PREDATOR * pred)
{
	if (sim_get_verbose_status()) action_predator_reproduced(pred->energy_pred, pred->coord_pred[X], pred->coord_pred[Y]);
	
	pred_add(pred->coord_pred[X], pred->coord_pred[Y], pred->energy_pred/2.);
	pred->energy_pred = pred->energy_pred/2.;
}

void pred_feed (PREY * prey, PREDATOR * pred)
{
	if (pred->energy_pred < ENERGIE_MAX-CONVERSION_NOURRITURE)
	{
		if (sim_get_verbose_status()) action_predator_fed(pred->energy_pred, pred->coord_pred[X], pred->coord_pred[Y], prey_get_energy(prey));
		
		pred->energy_pred += ENERGIE_CONSO * CONVERSION_NOURRITURE;
		prey_eaten(prey);
	}
}


int pred_decision (int i, int j, PREDATOR * pred)
{
	int prev_src_x = pred->coord_pred[0];
	int prev_src_y = pred->coord_pred[1];
	int src_x = pred->coord_pred[0];
	int src_y = pred->coord_pred[1];
	
	PREY * prey = NULL;
		
	int * res_x = &src_x;
	int * res_y = &src_y;

	int k=0, l=0, m =(sim_get_oasis()-1)/2.;
	static int status;
		
	pred->energy_pred -= ENERGIE_QUANTUM;
	
	if (pred->energy_pred < 0)
	{
		if (sim_get_verbose_status()) action_predator_no_energy(pred->energy_pred, pred->coord_pred[X], pred->coord_pred[Y]);
		
		cell_free_pred_in_cell(prev_src_y+DESERT_SIZE+m, prev_src_x+DESERT_SIZE+m, pred);
		pred_free(pred);
		
		return DIED;
	}
	
	else
	{	
		if ( cell_inside_oasis (src_x, src_y) )
		{
			if (cell_nb_pred_in_cell(i,j) >= 2)
			{
				if (pred_move_rand(src_x, src_y, prev_src_x, prev_src_y, res_x, res_y, pred))
				{
					status=MOVED;
					return MOVED;
				}
			}
			
			if (pred->energy_pred > ENERGIE_REPRO)
			{
				status = REPRO;
				return REPRO;
			}
			
			if ((prey = cell_get_prey_in_cell(i,j)) && (prey_get_energy(prey) > 0))
			{
				if (prey_alive(prey)) 
				{
					prey_captured(prey);
					
					status=EAT;
					return MOVED;
				}
				
				else
				{
					pred_feed(prey, pred);
					
					status = EAT;
					return EAT;
				}
			}
		
			else
			{
				for (k=-PREDATEUR_DEPL; k<=PREDATEUR_DEPL;k++)
				{
					for (l=-PREDATEUR_DEPL; l<= PREDATEUR_DEPL; l++)
					{
						if (cell_inside_oasis(i+k-DESERT_SIZE-m, j+l-DESERT_SIZE-m) && (prey=cell_get_prey_in_cell(i+k,j+l)) 
						&& prey_get_energy(prey) > 0 && prey_get_energy(prey) < pred_get_energy(pred)+1)
						{
							pred_move (src_x, src_y,prev_src_x,prev_src_y, j+l-DESERT_SIZE-m,i+k-DESERT_SIZE-m,res_x,res_y, pred);	
							
							status = EAT;
							return MOVED;
						}
					}
				}
				
				for (k=-PREDATEUR_PERCEPTION; k<=PREDATEUR_PERCEPTION;k++)
				{
					for (l=-PREDATEUR_PERCEPTION; l<= PREDATEUR_PERCEPTION; l++)
					{
						if (cell_inside_oasis(i+k-DESERT_SIZE-m, j+l-DESERT_SIZE-m) && (prey=cell_get_prey_in_cell(i+k,j+l)) 
						&& prey_get_energy(prey) > 0 && prey_get_energy(prey) < pred_get_energy(pred)+1)
						{
							pred_move (src_x, src_y,prev_src_x,prev_src_y, j+l-DESERT_SIZE-m,i+k-DESERT_SIZE-m,res_x,res_y, pred);	
										
							status = MOVED;
							return MOVED;
						}
					}
				}
				
				pred_move_rand(src_x, src_y, prev_src_x, prev_src_y, res_x, res_y, pred);		
			}
		}
		
		else
		{
			pred_move (src_x, src_y,prev_src_x,prev_src_y,0,0,res_x,res_y, pred);
			status=MOVED;
			return MOVED;
		}
	}
	return STAY;
}

int pred_update_nb (void)
{
	int pred_nb=0;
	PREDATOR * pred=tete_pred_list;
	
	while (pred)
	{
		pred_nb ++;
		pred=pred->next_pred;
	}
	return (pred_nb);
}

void pred_move (int src_x, int src_y, int prev_src_x, int prev_src_y, int des_x, int des_y, int * res_x, int * res_y, PREDATOR * pred)
{
	int depl=0;
	int m=(sim_get_oasis()-1)/2;
	
	estimate_movement(src_x, src_y,des_x, des_y, PREDATEUR_DEPL, res_x, res_y);
	depl=movement_calc_depl(prev_src_x, prev_src_y, *res_x, *res_y);
			
	if (sim_get_verbose_status()) action_predator_moved(pred->energy_pred, prev_src_x, prev_src_y, *res_x, *res_y, des_x, des_y);
			
	pred->energy_pred -= depl*depl*ENERGIE_QUANTUM;
	pred->coord_pred[0]=*res_x;
	pred->coord_pred[1]=*res_y;
				
	cell_add_pred(*res_x, *res_y, pred);
	cell_free_pred_in_cell(prev_src_y+DESERT_SIZE+m, prev_src_x+DESERT_SIZE+m,pred);			
}

int pred_move_rand(int src_x, int src_y, int prev_src_x, int prev_src_y, int * res_x, int * res_y, PREDATOR * pred)
{
	int depl=0;
	int m=(sim_get_oasis()-1)/2;
	
	srand(time(NULL));
	
	int des_x=0;
	int des_y=0;
	
	if (m != 0)
	{
		des_x = rand() % m;
		des_y = rand() % m;
	}

				
	estimate_movement(src_x,src_y, des_x, des_y, PREDATEUR_DEPL, res_x, res_y);
				
	if (cell_nb_pred_in_cell(*res_x+m+DESERT_SIZE, *res_y+m+DESERT_SIZE) <= 2)
	{	
		depl=movement_calc_depl(prev_src_x, prev_src_y, *res_x, *res_y);
		
		if (sim_get_verbose_status()) action_predator_moved(pred->energy_pred, prev_src_x, prev_src_y, *res_x, *res_y, des_x, des_y);
		
		pred->energy_pred -= depl*depl*ENERGIE_QUANTUM;
		pred->coord_pred[0]=*res_x;
		pred->coord_pred[1]=*res_y;
									
		cell_add_pred(*res_x, *res_y, pred);
		cell_free_pred_in_cell(prev_src_y+DESERT_SIZE+m, prev_src_x+DESERT_SIZE+m,pred);
					
		return 1;
	}
				
	else 
	{
		*res_x=prev_src_x;
		*res_y=prev_src_y;
		return 0;
	}
				
}
