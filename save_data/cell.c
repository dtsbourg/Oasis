/* Nom : cell.c
 * Description : Module ayant attrait aux cellules du monde. on y définit les structures,
 * on y crée et libère les cellules, et on y trouve un ensemble de fonction get/save rendant
 * le module opaque. La gestion dynamique de ces cellules y est aussi gérée.
 * Date de dernière MAJ : 23.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */  

#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "../constantes.h"
#include "../simulation/simulation.h"

#define ASCII_0 48
#define STERILE -1.

//Structures de données
struct Prey_cell
{
	PREY * p_prey;
	PREY_CELL * next;
};

struct Pred_cell
{
	PREDATOR * p_pred;
	PRED_CELL * next;
};

struct Cell
{
	float energy;
	PREY_CELL * prey_cell_list;
	PRED_CELL * pred_cell_list;
};

static CELL *** cell = NULL;
int m;

void cell_create_tab(int oasis_size)
{
	int i,j,k;
	
	if (!(cell = (CELL ***) malloc((oasis_size+2*DESERT_SIZE)*sizeof(CELL**))))
	{
		printf ("Cell Allocation Error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}
	
	for(k=0;k<oasis_size+2*DESERT_SIZE;k++)
	{
		cell[k] = (CELL **) malloc((oasis_size+2*DESERT_SIZE)*sizeof(CELL*));
	}
	
	for (i=0;i<oasis_size+2*DESERT_SIZE;i++)
	{
		for(j=0; j<oasis_size+2*DESERT_SIZE;j++)
		{
			cell[i][j] = NULL;
		}
	}
}

void cell_add (int i, int j, char c_cell)
{
	CELL * new_cell = NULL;
	
	if (!(new_cell=(CELL*) malloc(sizeof(CELL)))) 
	{
		printf ("Cell Allocation Error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}
	
	//Correspondances valeurs float et char
	if  (c_cell >= '0' && c_cell <= '9') 
		new_cell->energy=((float)c_cell-ASCII_0);
	else if (c_cell == '.')
		new_cell->energy=STERILE;
	
	new_cell->prey_cell_list=NULL;
	new_cell->pred_cell_list=NULL;	
	
	cell[i][j] = new_cell;		
}

void cell_add_prey (int j, int i, PREY * pt_prey)
{
	m=(sim_get_oasis()-1)/2.;
	
	PREY_CELL * new_prey_cell = NULL;
		
	if (!(new_prey_cell = (PREY_CELL *) malloc(sizeof(PREY_CELL)))) 
	{
		printf ("Prey Allocation Error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}
	
	
	if (cell[i+m+DESERT_SIZE][j+m+DESERT_SIZE] == NULL)
	{
		cell_add(i+m+DESERT_SIZE,j+m+DESERT_SIZE,STERILE);
	}
	
	
	new_prey_cell->p_prey=pt_prey;
	new_prey_cell->next=cell[i+m+DESERT_SIZE][j+m+DESERT_SIZE]->prey_cell_list;
	
	cell[i+m+DESERT_SIZE][j+m+DESERT_SIZE]->prey_cell_list=new_prey_cell;			
}

void cell_add_pred (int j, int i, PREDATOR * p_predator)
{
	m=(sim_get_oasis()-1)/2.;

	PRED_CELL * new_pred_cell = (PRED_CELL *) malloc(sizeof(PRED_CELL));
	
	if (cell[i+m+DESERT_SIZE][j+m+DESERT_SIZE] == NULL)
	{
		cell_add(i+m+DESERT_SIZE,j+m+DESERT_SIZE,STERILE);
	}
	
	new_pred_cell->p_pred=p_predator;
	new_pred_cell->next=cell[i+m+DESERT_SIZE][j+m+DESERT_SIZE]->pred_cell_list;
	cell[i+m+DESERT_SIZE][j+m+DESERT_SIZE]->pred_cell_list=new_pred_cell;
}


void cell_total_energy (float * p_total_energy_cell, float * p_total_energy_predator, float * p_total_energy_prey)
{
	PREDATOR * p_pred = pred_get_tete_list();
	PREY * p_prey = prey_get_tete_list();
	CELL * p_cell = NULL;
	
	int i, j;
	int oasis_size=sim_get_oasis();
	
	for (i=DESERT_SIZE;i<oasis_size+DESERT_SIZE; i++)
	{
		for(j=DESERT_SIZE;j<oasis_size+DESERT_SIZE;j++)
		{
			p_cell = cell[i][j];
			
			if(p_cell && p_cell->energy !=STERILE) (*p_total_energy_cell) += (p_cell->energy);
		}
	}
	
	while (p_pred)
	{
		(*p_total_energy_predator) += pred_get_energy(p_pred);
		p_pred=pred_get_next(p_pred);	
	}
	
	while (p_prey)
	{
		(*p_total_energy_prey) += prey_get_energy(p_prey);
		p_prey=prey_get_next(p_prey);
	}	
}


int cell_get_energy (int i, int j)
{
	CELL * p_cell=cell[i+DESERT_SIZE][j+DESERT_SIZE];
	if (p_cell)
		return (p_cell->energy);
	else return -1;
}


void cell_free_all(void)
{
	int i=0,j=0;
	int oasis_size=sim_get_oasis();
	
	if (cell)
	{
		for (i=0; i<oasis_size+2*DESERT_SIZE;i++)
		{
			for (j=0;j<oasis_size+2*DESERT_SIZE;j++)
			{
				if(cell[i] && cell[i][j])
				{
					if (cell[i][j]->pred_cell_list)
					{
						PRED_CELL * p_pred_cell_list=cell[i][j]->pred_cell_list;
						while(p_pred_cell_list)
						{
							PRED_CELL * delete_pred = p_pred_cell_list;
							p_pred_cell_list=p_pred_cell_list->next;
							free(delete_pred);
						}
					}
					if (cell[i][j]->prey_cell_list)
					{
						PREY_CELL * p_prey_cell_list=cell[i][j]->prey_cell_list;
						while(p_prey_cell_list)
						{
							PREY_CELL * delete_prey = p_prey_cell_list;
							p_prey_cell_list=p_prey_cell_list->next;
							free(delete_prey);
						}
					}
					free(cell[i][j]);
				}
			}
			
			free(cell[i]);
		}
		cell=NULL;
	}
}


void cell_afficher (FILE * out_file)
{
	int i,j;
	float energy = 0.;
	char c_energy = '\n';
	int oasis_size = sim_get_oasis();
	
	for (i=oasis_size-1;i>=0;i--)
	{
		for (j=0;j<oasis_size;j++)
		{
			energy = cell_get_energy(i,j) ;
			
			if (energy == STERILE) c_energy = '.';
			else if (energy >= 0 && energy <= 9) c_energy = energy+48;

			fprintf (out_file, "%c", c_energy);
		}
		fprintf (out_file, "\n");
	}
}

float cell_multiple_prey_percentage (int i, int j)
{
	float total_count=0., dead_count=0.;
	
	if (cell[i][j] && cell[i][j]->prey_cell_list)
	{
		PREY_CELL * p_cell_list = cell[i][j]->prey_cell_list;
		
		while(p_cell_list)
		{
			total_count++;
			
			if (prey_alive(p_cell_list->p_prey) == 0)
				dead_count++;
				
			p_cell_list = p_cell_list->next;	
		}
		return (dead_count/total_count);
	}
	
	else return 0;
}


//Rendu 3
void cell_update()
{
	int i,j;
	
	for (i = DESERT_SIZE ; i < (sim_get_oasis())+DESERT_SIZE ; i++)
	{
		for (j = DESERT_SIZE ; j < (sim_get_oasis())+DESERT_SIZE ; j++)
		{
			cell_croissance(i,j);
			cell_pollen(i,j);
			cell_sterile (i,j);
		}
	}
}

void cell_croissance (int i, int j)
{	
	if (cell[i][j]->energy != ENERGIE_MAX && cell[i][j]->energy != STERILE)
		cell[i][j]->energy += VEG_CROISSANCE;
}

void cell_pollen (int i, int j)
{
	int k, l;

	if (cell[i][j]->energy == STERILE)
	{
		for (k=-1; k<=1; k++)
		{
			for (l=-1; l<=1; l++)
			{
				if ((cell[i+k][j+l]) && cell[i+k][j+l]->energy == ENERGIE_MAX) 
					{
						cell[i][j]->energy = 0.;
					}
			}
		}
	}
}

void cell_sterile (int i, int j)
{
	int count=0;
	
	PREY_CELL * p_prey_cell = NULL;
	PRED_CELL * p_pred_cell = NULL;
	
	if (cell[i][j]->prey_cell_list)
		p_prey_cell = cell[i][j]->prey_cell_list;
	
	if (cell[i][j]->pred_cell_list)
		p_pred_cell = cell[i][j]->pred_cell_list;
	
	while (p_prey_cell) 
	{
		count++;
		p_prey_cell = p_prey_cell->next;
	}
		
	while (p_pred_cell) 
	{
		count++;
		p_pred_cell = p_pred_cell->next;
	}

	if (count >= MAX_MOBILE) 
		cell[i][j]->energy=STERILE;
	
}

PREY * cell_get_prey_in_cell(int i, int j)
{
	if ((cell[i][j]) && (cell[i][j]->prey_cell_list))
		return cell[i][j]->prey_cell_list->p_prey;
	
	else return NULL;
}

PREY_CELL * cell_get_prey_cell (int i, int j)
{
	if (cell[i][j] && cell[i][j]->prey_cell_list)
		return cell[i][j]->prey_cell_list;
	else return NULL;
}



void cell_free_pred_in_cell (int i, int j, PREDATOR * pred)
{
	
	if (cell[i][j])
	{
		PRED_CELL * preda_cell = cell[i][j]->pred_cell_list;
		PRED_CELL * precedent = NULL;
		
		while (preda_cell)
		{
			if (preda_cell->p_pred == pred)
			{
				PRED_CELL * a_supprimer;
				if (precedent != NULL)
				{
					a_supprimer = preda_cell;
					precedent->next = preda_cell->next;
				}
				else
				{
					a_supprimer = preda_cell;
					preda_cell = preda_cell->next;
				}
				if (a_supprimer == cell[i][j]->pred_cell_list)
				{
					cell[i][j]->pred_cell_list = cell[i][j]->pred_cell_list->next;
				}
				
				free (a_supprimer);
				break;
			}
			precedent = preda_cell;
			preda_cell = preda_cell->next;
		}
	}
		
	/*if ((cell[i][j]))
	{
		PRED_CELL * preda_cell = cell[i][j]->pred_cell_list;
		
		if (preda_cell)
		{
			if (preda_cell->p_pred == pred)
			{
				cell[i][j]->pred_cell_list=cell[i][j]->pred_cell_list->next;
				free(preda_cell);
				
				if (cell[i][j]->pred_cell_list==NULL && !(cell_inside_oasis(src_x,src_y)) && cell[i][j]->prey_cell_list == NULL)
				{
					free(cell[i][j]);
					cell[i][j]=NULL;
				}
			}
				
			else 
			{	
				PRED_CELL * precedent = preda_cell;
				PRED_CELL * a_supprimer = NULL;

				while ((precedent->next != NULL) && precedent->next->p_pred != pred)
				{
					precedent=precedent->next;
				}

				if (precedent->next != NULL)
				{
					a_supprimer=precedent->next;
					precedent->next=precedent->next->next;
					free(a_supprimer);
					a_supprimer=NULL;
				}
			}
		}
	}*/
}


int cell_inside_oasis(int i, int j)
{
	int m=(sim_get_oasis()-1)/2.;
	
	if (abs(i)<=m && abs(j)<=m)
	return 1;
	
	else return 0;
}

void cell_eaten(int i, int j)
{
	if (cell[i][j]->energy != STERILE && cell[i][j]->energy > 0.)
	{
		cell[i][j]->energy -= ENERGIE_CONSO;
	}
}

PREY_CELL * cell_get_next_prey_cell(int i, int j, PREY_CELL * prey_cell)
{
	if (prey_cell)
		return prey_cell->next;
	
	else return NULL;
}

PREY * cell_get_next_prey_in_cell(int i, int j, PREY_CELL * prey_cell)
{
	if (prey_cell->next)
		return prey_cell->next->p_prey;
	
	else return NULL;
}

void cell_free_prey_in_cell (int i, int j, PREY * prey)
{	
	if (cell[i][j])
	{
		PREY_CELL * preyy_cell = cell[i][j]->prey_cell_list;
		PREY_CELL * precedent = NULL;
		
		while (preyy_cell)
		{
			if (preyy_cell->p_prey == prey)
			{
				PREY_CELL * a_supprimer;
				if (precedent != NULL)
				{
					a_supprimer = preyy_cell;
					precedent->next = preyy_cell->next;
				}
				else
				{
					a_supprimer = preyy_cell;
					preyy_cell = preyy_cell->next;
				}
				if (a_supprimer == cell[i][j]->prey_cell_list)
				{
					cell[i][j]->prey_cell_list = cell[i][j]->prey_cell_list->next;
				}
				
				free (a_supprimer);
				break;
			}
			precedent = preyy_cell;
			preyy_cell = preyy_cell->next;
		}
		/*
		if(preyy_cell)
		{
			if (preyy_cell->p_prey == prey)
			{
				cell[i][j]->prey_cell_list=cell[i][j]->prey_cell_list->next;
				free(preyy_cell);
				
				if (cell[i][j]->prey_cell_list==NULL && !(inside_oasis(src_x, src_y)) && cell[i][j]->pred_cell_list == NULL)
				{
					free(cell[i][j]);
					cell[i][j]=NULL;
				}
			}
				
			else 
			{	
				PREY_CELL * precedent = preyy_cell;
				PREY_CELL * a_supprimer = NULL;
				
				while ((precedent->next != NULL) && precedent->next->p_prey != prey)
				{
					precedent=precedent->next;
				}

				if (precedent->next != NULL)
				{
					a_supprimer=precedent->next;
					precedent->next=precedent->next->next;
					free(a_supprimer);
					a_supprimer=NULL;
				}
			}
		}
		*/
	}
}

PREDATOR * cell_get_pred_in_cell(int i, int j)
{
	if ((cell[i][j]) && (cell[i][j]->pred_cell_list) && (cell[i][j]->pred_cell_list->p_pred))
		return cell[i][j]->pred_cell_list->p_pred;
	
	else return NULL;
}

PREDATOR * cell_get_next_pred_in_cell(int i, int j, PRED_CELL * pred_cell)
{
	if (pred_cell && pred_cell->next)
		return pred_cell->next->p_pred;
	
	else return NULL;
}

PRED_CELL * cell_get_pred_cell (int i, int j)
{
	if (cell[i][j] && cell[i][j]->pred_cell_list)
		return cell[i][j]->pred_cell_list;
	else return NULL;
}

PRED_CELL * cell_get_next_pred_cell(int i, int j, PRED_CELL * pred_cell)
{
	if (pred_cell)
		return pred_cell->next;
	
	else return NULL;
}

int cell_nb_prey_in_cell(int i, int j)
{
	int count=0;
	
	PREY_CELL * p_prey_cell = NULL;
	
	if (cell[i][j] && cell[i][j]->prey_cell_list)
		p_prey_cell = cell[i][j]->prey_cell_list;
	
	while (p_prey_cell) 
	{
		count++;
		p_prey_cell = p_prey_cell->next;
	}
	
	return count;
}

int cell_nb_pred_in_cell (int i, int j)
{
	int count=0;
	
	PRED_CELL * p_pred_cell = NULL;
	
	if (cell[i][j] && cell[i][j]->pred_cell_list)
		p_pred_cell = cell[i][j]->pred_cell_list;
	
	while (p_pred_cell) 
	{
		count++;
		p_pred_cell = p_pred_cell->next;
	}
	
	return count;
}
