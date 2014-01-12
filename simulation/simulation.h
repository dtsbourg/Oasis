/* Nom : simulation.h
 * Description : Contient les déclarations des deux fonctions de lecture 
 * de données et d'appel à des vérifications de ces dernières, ainsi qu'un
 * ensemble de fonctions assurant la répartition des données de façon opaque
 * dans tout le programme (fonction save/get).
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 3.0
 */

#ifndef SIMULATION_H
#define SIMULATION_H

//Automate de lecture
int sim_input (char *);
void sim_input_test (char lign[100], int *, int *);

//Constantes utilisées pour l'automate de lecture / test
enum Input_status {EXIT=0, CORRECT=1, CLOCK=2, OASIS_SIZE_PARAMETER=3, OASIS=4, PREY_NUMBER=5,
 PREY_COORDINATES=6, PREDATOR_NUMBER=7, PREDATOR_COORDINATES=8};

//Renvoie la coordonnée de l'entité la plus éloignée si elle est en dehors de l'oasis
int sim_find_furthest (void);

//Appel aux différentes fonctions de libération de mémoire
void sim_reset (void);

//Enregistrement de l'état actuel dans un fichier .txt
void save_out_txt (int);

/*Paires de fonctions save/get :
 * save enregistre la valeur
 * get la renvoie */

//Statut de la simulation
void sim_save_status(int);
int sim_get_status (void);

//Horloge
void sim_save_time (int);
int sim_get_time (void);

//Taille de l'oasis
void sim_save_size (int);
void sim_save_oasis (int);
int sim_get_oasis (void);
int sim_get_size (int, int);

//Enregistrement des états successifs dans un fichier .dat
void save_out_dat(void);
void sim_save_output_status(int);
int sim_get_output_status (void);

//Verbose
void sim_save_verbose_status(int );
int sim_get_verbose_status (void);

//View all
void sim_save_va_status(int status);
int sim_get_va_status(void);

//Nb de prédateurs
int sim_get_pred_nb (void);
void sim_save_pred_nb (int);

//Nb de proies
int sim_get_prey_nb (void);
void sim_save_prey_nb (int);

//MAJ de la simulation
void simulation_update(void);


//SOURIS (bonus B)
//Coordonnées
int sim_get_y_mouse (void);
int sim_get_x_mouse (void);
void sim_save_mouse (int , int );
//Zoom
void sim_save_zoom(int);
int sim_get_zoom (void);
void sim_save_mouse_status(int);
int sim_get_mouse_status(void);


#endif
