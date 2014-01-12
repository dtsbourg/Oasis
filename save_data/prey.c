/* Nom : prey.c
 * Description : Module ayant attrait aux proies. on y définit les structures,
 * on y crée et libère l'esapce alloué pour elles (sous forme de liste chaînée)
 * et on y trouve un ensemble de fonction get/save rendant le module opaque.
 * La gestion dynamique de la simulation des proies est gérée ici.
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */  


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prey.h"
#include "../constantes.h"
#include "cell.h"
#include "../simulation/simulation.h"
#include "../movement/movement.h"
#include "../action/action.h"

#define X 0
#define Y 1

enum Decision {REPRO, EAT, MOVED, STAY, DIED};

struct Prey
{
	int coord_prey[2];
	float energy_prey;
	int bool_prey;
	PREY * next_prey; 
};

static PREY * tete_prey_list=NULL;
static PREY ** p_tete_prey_list = &tete_prey_list;

void prey_add (int x, int y, int alive, float energy)
{	
	PREY *new_prey = NULL;
	
	if (!(new_prey = (PREY*) malloc(sizeof(PREY))))
	{
		printf ("Prey Allocation Error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}
	
	new_prey->coord_prey[X]=x;
	new_prey->coord_prey[Y]=y;
	new_prey->bool_prey=alive;
	new_prey->energy_prey=energy;
	
	new_prey->next_prey=tete_prey_list;
	*p_tete_prey_list=new_prey;
	
	cell_add_prey(x , y, new_prey);
}

int prey_get_x (PREY * p_prey)
{
	if (p_prey)
	return (p_prey->coord_prey[X]);

}

int prey_get_y (PREY * p_prey)
{
	if (p_prey)
	return (p_prey->coord_prey[Y]);
	
}

int prey_alive (PREY * p_prey)
{
	if (p_prey)
	return (p_prey->bool_prey);
}

float prey_get_energy (PREY * p_prey)
{
	if (p_prey)
	return (p_prey->energy_prey);
}


PREY * prey_get_next (PREY * p_prey)
{
	return (p_prey->next_prey);
}

PREY * prey_get_tete_list (void)
{
	return (tete_prey_list);
}

void prey_free (PREY * elem)
{
	PREY * courant= *p_tete_prey_list;
	PREY * precedent= *p_tete_prey_list;
	
	while (courant != elem && courant != NULL)
	{
		precedent = courant;
		courant = courant->next_prey;
	}
	
	if (courant != NULL)
	{
		if (courant != *p_tete_prey_list)
		{
			precedent->next_prey=courant->next_prey;
			free (courant);
			courant=NULL;
		}
		
		if (courant == *p_tete_prey_list)
		{
			*p_tete_prey_list=courant->next_prey;
			free (courant);
			courant=NULL;
		}
	}
}

void prey_free_all(void)
{
	PREY * elem;
	
	while (*p_tete_prey_list !=NULL)
	{
		elem = *p_tete_prey_list;
		prey_free(elem);
	}
}


//RENDU 3
int prey_decision (int i, int j, PREY * prey)
{
	static int status;
	int k,l;
	int m = (sim_get_oasis()-1)/2;
	
	int prev_src_x = prey->coord_prey[0];
	int prev_src_y = prey->coord_prey[1];
	int src_x = prey->coord_prey[0];
	int src_y = prey->coord_prey[1];
		
	int * res_x = &src_x;
	int * res_y = &src_y;
	
	
	if(prey->energy_prey > 0) 
		prey->energy_prey -= ENERGIE_QUANTUM;
	
	if (prey_alive(prey) && prey->energy_prey > 0)
	{
		if (cell_inside_oasis(src_x, src_y))
		{		
			if (prey_flee (src_x,src_y, prev_src_x, prev_src_y,res_x, res_y,prey) == 0)
			{
				if (cell_nb_prey_in_cell(i,j) >= 2)
				{
					if (prey_move_rand (src_x, src_y, prev_src_x, prev_src_y, res_x, res_y, prey))
					{
						status = MOVED;
						return MOVED;
					}
				}
				
				if (prey->energy_prey > ENERGIE_REPRO && cell_nb_prey_in_cell(i,j) < 2 )
				{
					status = REPRO;
					return REPRO;
				}
				
				if (cell_get_energy(i-DESERT_SIZE,j-DESERT_SIZE) > 0 )
				{
					status = EAT;
					return EAT;
				}
				
				else 
				{
					for (k=-PROIE_DEPL;k<=PROIE_DEPL; k++)
					{
						for (l=-PROIE_DEPL;l<=PROIE_DEPL; l++)
						{
							if (cell_inside_oasis(src_y, src_x) && cell_get_energy(i+k-DESERT_SIZE,j+l-DESERT_SIZE) > 0 
							&& cell_nb_prey_in_cell(i+k,j+l)<2)
							{
								prey_move(src_x, src_y, prev_src_x, prev_src_y, j+l-DESERT_SIZE-m, i+k-DESERT_SIZE-m, res_x, res_y, prey);
												
								status = EAT;
								return MOVED;
							}
						}
					}
					
					for (k=-PROIE_PERCEPTION;k<=PROIE_PERCEPTION; k++)
					{
						for (l=-PROIE_PERCEPTION;l<=PROIE_PERCEPTION; l++)
						{
							if (cell_inside_oasis(src_y, src_x) && cell_get_energy(i+k-DESERT_SIZE,j+l-DESERT_SIZE) > 0 
							&& cell_nb_prey_in_cell(i+k,j+l)<2 )
							{
								prey_move(src_x, src_y, prev_src_x, prev_src_y, j+l-DESERT_SIZE-m, i+k-DESERT_SIZE-m, res_x, res_y, prey);
													
								status = MOVED;
								return MOVED;
							}
						}
					}
				}
			}
			
		}

		else 
		{
			prey_move(src_x, src_y, prev_src_x, prev_src_y, m, m, res_x, res_y, prey);
			
			status=MOVED;
			return MOVED;
		}
	}
	
	if (prey->energy_prey < ENERGIE_QUANTUM)
	{
		cell_free_prey_in_cell(src_y+DESERT_SIZE+m,src_x+DESERT_SIZE+m,prey);
		prey_free(prey);
		
		status=DIED;
		return DIED;
	}
	
	return STAY;
}


void prey_captured(PREY * prey)
{
	if (prey) 
	{
		prey->bool_prey=0;
	}
}

void prey_eaten (PREY * prey)
{
	if (prey)
	{
		if (prey->energy_prey < ENERGIE_CONSO) 
			{
				prey->energy_prey=0;
				if (sim_get_verbose_status()) 
					action_prey_no_energy(prey->energy_prey, 1, prey->coord_prey[X], prey->coord_prey[Y]);
			}
	
		else prey->energy_prey -= ENERGIE_CONSO;
	}
}

void prey_feed (int i, int j, PREY * prey)
{
	if (prey->energy_prey <= ENERGIE_MAX-(ENERGIE_CONSO*CONVERSION_NOURRITURE))
	{
		if (sim_get_verbose_status()) action_prey_fed(prey->energy_prey, prey->coord_prey[X], prey->coord_prey[Y]);
		
		prey->energy_prey += ENERGIE_CONSO * CONVERSION_NOURRITURE;
		cell_eaten(i,j);
	}
}

void prey_update (void)
{	
	PREY * prey = tete_prey_list;
	
	if (sim_get_verbose_status()) 
	{
		printf("%d\n", sim_get_time());
		start_prey_actions();
	}
	
	while (prey)
	{		
		PREY * n_prey = prey->next_prey;
		int prev_src_x = prey->coord_prey[0];
		int prev_src_y = prey->coord_prey[1];
		int src_x = prey->coord_prey[0];
		int src_y = prey->coord_prey[1];
		int m=(sim_get_oasis()-1)/2.;

		int status_decision = prey_decision(src_y+DESERT_SIZE+m, src_x+DESERT_SIZE+m, prey);

		if (status_decision == EAT)
		{
			prey_feed(prev_src_y+DESERT_SIZE+m,prev_src_x+DESERT_SIZE+m, prey);
		}
		
		else if (status_decision == REPRO)
		{
			prey_reproduce ( prey );
		}
		if (prey->energy_prey <= 0 && status_decision != DIED)
		{
			if (sim_get_verbose_status())
			{
				 action_prey_no_energy(prey->energy_prey, 0, src_x, src_y);
			}
			
			cell_free_prey_in_cell(prey->coord_prey[1]+DESERT_SIZE+m,prey->coord_prey[0]+DESERT_SIZE+m,prey);
			prey_free(prey);
		}
		prey=n_prey;
	}
	
	if (sim_get_verbose_status()) end_prey_actions();
}

void prey_reproduce (PREY * prey)
{
	if (sim_get_verbose_status()) action_prey_reproduced(prey->energy_prey, prey->coord_prey[X], prey->coord_prey[Y]);
	prey_add(prey->coord_prey[X], prey->coord_prey[Y], 1, prey->energy_prey/2.);
	prey->energy_prey = prey->energy_prey/2.;
}

int prey_update_nb (void)
{
	int prey_nb=0;
	PREY * prey=tete_prey_list;
	
	while (prey)
	{
		prey_nb++;
		prey=prey->next_prey;
	}
	
	return (prey_nb);
}

int prey_move_rand (int src_x, int src_y, int prev_src_x, int prev_src_y, int * res_x, int * res_y, PREY * prey)
{
	srand(time(NULL));
	int m=(sim_get_oasis()-1)/2.;
	int des_x=0;
	int des_y=0;
	
	if (m != 0)
	{
		des_x = rand() % m;
		des_y = rand() % m;
	}

	
	int depl=0;
				
	estimate_movement(src_x,src_y, des_x, des_y, PROIE_DEPL, res_x, res_y);
				
	if (cell_nb_prey_in_cell((*res_x)+m+DESERT_SIZE, (*res_y)+m+DESERT_SIZE) < 2)
	{	
		depl=movement_calc_depl(prev_src_x, prev_src_y, *res_x, *res_y);
				
		if (sim_get_verbose_status()) action_prey_moved(prey->energy_prey, prev_src_x, prev_src_y, *res_x, *res_y);
		
		prey->energy_prey -= depl*depl*ENERGIE_QUANTUM;
		
		prey->coord_prey[X]= *res_x;
		prey->coord_prey[Y]= *res_y;
		
		cell_add_prey(*res_x, *res_y, prey);
		cell_free_prey_in_cell(prev_src_y+DESERT_SIZE+m, prev_src_x+DESERT_SIZE+m,prey);
		
		return 1;
	}
				
	else 
	{
		*res_x=prev_src_x;
		*res_y=prev_src_y;
		return 0;
	}
}


void prey_move (int src_x, int src_y, int prev_src_x, int prev_src_y, int des_x, int des_y, int * res_x, int * res_y, PREY * prey)
{
	int depl=0;
	int m=(sim_get_oasis()-1)/2;
	
	estimate_movement(src_x, src_y, des_x, des_y, PROIE_DEPL, res_x, res_y);
	
	depl=movement_calc_depl(prev_src_x, prev_src_y, *res_x, *res_y);
	
	if (sim_get_verbose_status()) action_prey_moved(prey->energy_prey, prev_src_x, prev_src_y, *res_x, *res_y);
						
	prey->energy_prey -= depl*depl*ENERGIE_QUANTUM;
	
	prey->coord_prey[X]= *res_x;
	prey->coord_prey[Y]= *res_y;

	cell_add_prey(*res_x, *res_y, prey);
	cell_free_prey_in_cell(prev_src_y+DESERT_SIZE+m,prev_src_x+DESERT_SIZE+m,prey);				
}

int prey_flee (int src_x, int src_y, int prev_src_x, int prev_src_y, int * res_x, int * res_y, PREY * prey)
{
	int k, l, m=(sim_get_oasis()-1)/2;
	int pred_vector[2]={0,0};
	int depl=0, count=0;
	
	for(k=-PROIE_PERCEPTION;k<=PROIE_PERCEPTION;k++)
	{
		for(l=-PROIE_PERCEPTION;l<=PROIE_PERCEPTION;l++)
		{
			if (cell_get_pred_in_cell(src_y+k+DESERT_SIZE+m, src_x+l+DESERT_SIZE+m))
			{
				pred_vector[X] += l;
				pred_vector[Y] += k;
				
				count++;
			}
		}
	}
	
	if (count >= 4 && prey->energy_prey >= 4. && m>3)
	{
		estimate_movement(src_x, src_y, -(pred_vector[X]), -(pred_vector[Y]), PROIE_DEPL, res_x, res_y);
		
		depl=movement_calc_depl(prev_src_x, prev_src_y, *res_x, *res_y);
		
		if (sim_get_verbose_status()) action_prey_moved(prey->energy_prey, prev_src_x, prev_src_y, *res_x, *res_y);
								
		prey->energy_prey -= depl*depl*ENERGIE_QUANTUM;
		
		prey->coord_prey[X]= *res_x;
		prey->coord_prey[Y]= *res_y;
		
		cell_add_prey(*res_x, *res_y, prey);
		cell_free_prey_in_cell(prev_src_y+DESERT_SIZE+m,prev_src_x+DESERT_SIZE+m,prey);	
		
		return 1;
	}
	
	else return 0;
}
