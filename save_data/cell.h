/* Nom : cell.h
 * Description : Header du module cellules, contenant les typedef
 * des structures ainsi que les prototypes des fonctions s'y trouvant.
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */  

#ifndef CELL_H
#define CELL_H

typedef struct Cell CELL;
typedef struct Prey_cell PREY_CELL;
typedef struct Pred_cell PRED_CELL;

#include "prey.h"
#include "predator.h"

//CREATION ET GESTION DES CELLULES
//Création du tableau de cellules par allocation dynamique
void cell_create_tab (int oasis_size);
//Ajoute une cellule au tableau de cell
void cell_add (int, int, char);
//Ajoute un prédateur à la cellule dans laquelle elle se trouve (si dans oasis)
void cell_add_pred(int, int, PREDATOR *);
//Ajoute une proie à la cellule dans laquelle elle se trouve (si dans oasis)
void cell_add_prey(int, int, PREY*);
//Libération de l'espace mémoire alloué pour l'ensemble des cellules
void cell_free_all (void);
//Affichage des cellules dans le fichier out
void cell_afficher (FILE *);

//CELL INFO
//Energie
int cell_get_energy (int, int);
//Calcule les énergies totales de prédateurs, proies, et végétation
void cell_total_energy (float*, float*, float*);
//Calcul du pourcentage de proies vivantes/mortes dans une même cellule
float cell_multiple_prey_percentage (int, int);
//Renvoie 1 si la cellule demandée se trouve dans l'oasis, 0 sinon
int cell_inside_oasis(int, int);

//GESTION DE LA RESSOURCE VEGETALE
//Croissance des cellules à chaque tour
void cell_croissance (int, int);
//Fertilisation des cellules stériles si un de ses voisins est à ENERGIE_MAX
void cell_pollen (int, int);
//Stérilisation de la cellule si il y a plus de MAX_MOBILE dessus
void cell_sterile (int, int);
//Diminue l'énergie de la végétation lorsqu'elle est consommée
void cell_eaten (int, int);
//Fonction de MAJ des cellules
void cell_update(void);


//PREYS IN CELL
//Renvoie un pointeur sur la première proie de la liste chaînée 
//des proies se trouvant dans la cellule
PREY * cell_get_prey_in_cell(int, int);
//Renvoie un pointeur sur le premier element de la liste chainée
//des proies dans la cellule
PREY_CELL * cell_get_prey_cell (int, int);
//Renvoie un pointeur sur l'élement suivant de la liste chaînée
//des proies de la cellule
PREY_CELL * cell_get_next_prey_cell (int, int, PREY_CELL *);
//Renvoie un pointeur sur la proie suivante se trouvant dans la cellule
PREY * cell_get_next_prey_in_cell(int, int, PREY_CELL *);
//Renvoie le nombre de proies présentes dans la cellule
int cell_nb_prey_in_cell(int, int);
//Libère la proie de la liste chaînée de la cellule
void cell_free_prey_in_cell(int,int,PREY*);


//PREDATORS IN CELL
//Renvoie un pointeur sur le premier predateur de la liste chaînée 
//des prédateurs se trouvant dans la cellule
PREDATOR * cell_get_pred_in_cell(int, int);
//Renvoie un pointeur sur le premier element de la liste chainée
//des prédateurs de la cellule
PRED_CELL * cell_get_pred_cell (int, int);
//Renvoie un pointeur sur le prédateur suivant se trouvant dans la cellule
PREDATOR * cell_get_next_pred_in_cell(int,int, PRED_CELL *);
//Renvoie un pointeur sur l'élement suivant de la liste chaînée
//des prédateurs de la cellule
PRED_CELL * cell_get_next_pred_cell(int, int, PRED_CELL *);
//Renvoie le nombre de prédateurs présents dans la cellule
int cell_nb_pred_in_cell(int, int);
//Libère le prédateur de la liste chaînée de la cellule
void cell_free_pred_in_cell(int, int, PREDATOR *);

#endif
