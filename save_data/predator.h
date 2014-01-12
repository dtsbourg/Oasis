/* Nom : predator.h
 * Description : Fichier header du module predateurs. Contient la déclaration du
 * type de structure PREDATOR, ainsi que les prototypes des fonctions.
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */  

#ifndef PREDATOR_H
#define PREDATOR_H

#include "prey.h"
typedef struct Predator PREDATOR;


//GESTION DYNAMIQUE DE LA MEMOIRE
//Ajoute un prédateur à la liste chaînée
void pred_add (int, int, float);
//Libération de l'espace mémoire alloué pour un prédateur donné
void pred_free (PREDATOR *);
//Libération de l'espace mémoire alloué pour l'ensemble des prédateurs
void pred_free_all (void);
//Prédateur suivant dans la liste chaînée
PREDATOR * pred_get_next (PREDATOR *);
//Adresse de la tête de liste des prédateurs
PREDATOR * pred_get_tete_list (void);

//INFORMATIONS
//Coordonnées
int pred_get_x (PREDATOR *);
int pred_get_y (PREDATOR *);
//Energie
float pred_get_energy (PREDATOR *);
//Predator number
int pred_update_nb(void);


//GESTION DE LA SIMULATION
//MAJ des prédateurs
void pred_update (void);
//Fonction décidant de la décision à prendre quant à l'action des prédateurs
//(Fonctionnement décrit dans l'implémentation)
int pred_decision(int, int, PREDATOR *);
//Un prédateur se reproduit
void pred_reproduce (PREDATOR *);
//Le prédateur se nourrit
void pred_feed (PREY *, PREDATOR *);
//Le prédateur se déplace dans un direction donnée
void pred_move (int, int, int, int, int, int, int *, int *, PREDATOR *);
//Le prédateur se déplace dans une direction aléatoire
int pred_move_rand(int, int, int, int, int *, int *, PREDATOR *);

#endif
