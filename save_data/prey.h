/* Nom : prey.h
 * Description : Fichier header du module proies, contentant la définition
 * du type de structure PREY ainsi que les prototypes des fonctions.
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */  


#ifndef PREY_H
#define PREY_H

typedef struct Prey PREY;


//PGESTION DYNAMIQUES DE LA MEMOIRE
//Ajoute une proie à la liste chaînée
void prey_add (int, int, int, float);
//Libération de l'espace mémoire alloué pour une proie
void prey_free (PREY *);
//Libération de l'espace mémoire alloué pour l'ensemble des proies
void prey_free_all (void);
//Proie suivante dans la liste chaînée
PREY * prey_get_next (PREY *);
//Adresse de la tête de liste des proies
PREY * prey_get_tete_list (void);

//INFOS
//Coordonnées
int prey_get_x (PREY *);
int prey_get_y(PREY *);
//Renvoie 1 si la proie est vivante, 0 si elle est morte
int prey_alive (PREY *);
//Energie
float prey_get_energy(PREY*);
//Prey number
int prey_update_nb(void);

//GESTION DES PROIES
//MAJ des proies
void prey_update (void);
//Fonction de décision de l'action à éxécuter (Fonctionnement décrit dans l'implémentation)
int prey_decision (int, int, PREY *);
//La proie a été capturée (MAJ du booléen vivant)
void prey_captured(PREY *);
//Un prédateur consomme la proie (MAJ de l'énergie)
void prey_eaten (PREY*);
//La proie se nourrit (MAJ de l'énergie)
void prey_feed(int, int, PREY*);
//La proie se reproduit
void prey_reproduce (PREY *);
//La proie se déplace dans une direction aléatoire
int prey_move_rand(int, int, int, int, int*, int*, PREY *);
//La proie se déplace dans une direction donnée
void prey_move(int, int, int, int, int, int, int*, int*, PREY *);

//BONUS FUTIE DES PROIES
int prey_flee (int , int , int , int , int * , int * , PREY * );
#endif


