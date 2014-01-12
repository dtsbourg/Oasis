/* Nom : movement.c
 * Description : Contient le code source de la fonction estimate_movement
 * définissant le déplacement des proies et des predateurs. 
 * Date de dernière MAJ : 30.04.2013
 * Auteur : Colin Dupont
 * Modifications : Dylan Bourgeois
 */

#include <stdlib.h>
#include <stdio.h>
#include "movement.h"
#include <math.h>

void estimate_movement(int src_x, int src_y, 
					   int des_x, int des_y, 
					   int step_size, 
					   int* res_x, int* res_y)
{
	int dx, dy, voisinage_k;
	float rapport;
	
	//calcul du vecteur de direction et de sa norme infinie
	dx = des_x - src_x;
	dy = des_y - src_y;
	voisinage_k  = abs(abs(dx) >= abs(dy) ? dx : dy);
	
	//évite calcules inutiles si cellule atteignable
	if (step_size >= voisinage_k)
	{
		*res_x = des_x;
		*res_y = des_y;
		return;
	}
	
	else
	{
		//redimensionne le vecteur direction à une longeur step_size
		//selon norme infinie puis déplace entité selon ce même vecteur
		rapport = step_size / (float)voisinage_k;
		
		*res_x = src_x + (int)(dx * rapport);
		*res_y = src_y + (int)(dy * rapport);
	}
}

int movement_calc_depl (int prev_src_x, int prev_src_y, int src_x, int src_y)
{
	if (abs(prev_src_x-src_x) > abs(prev_src_y-src_y)) return (abs(prev_src_x-src_x));
	else return abs(prev_src_y-src_y);
}
