/* Nom : graphic.h
 * Description : Contient les fonctions de dessin des différents 
 * éléments du monde, ainsi que le monde lui-même.
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */

#ifndef GRAPHIC_H
#define GRPAHIC_H

/* Fonction chargée de dessiner les différents éléments du monde :
 * proies, predateurs, végétation, grille, oasis. 
 * Gère aussi l'adaptation des coordonnées en mode zoom
 */
void graphic_monde(void);

/* Fonction callback de la glutMainLoop, appelle les fonctions de dessin,
 * puis met à jour le buffer 
 * */
void graphic_display_cb(void);

/* Fonction callback de la glutMouseFunc, gère les coordonnées de la souris
 * pour le mode zoom
 * */
void graphic_souris_cb (int, int, int, int);

//Réponse au changement de taille de la fenêtre pour éviter les déformations
void graphic_reshape_cb (int, int);

//MAJ des valeurs affichées par la barre Control
void set_cb (void);

//BONUS ZOOM B+
//Fonction traçant les lettres 
void graphic_render_string (float , float , void *, const char *);
//Appelle la fonction graphic_render_string pour les proies
void draw_prey_energy (int, int, int, int, int, int);
//Appelle la fonction graphic_render_string pour les prédateurs
void draw_pred_energy (int, int, int, int, int, int);

#endif
