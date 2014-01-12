/* Nom : test.h
 * Description : Contient les fonctions du module de test des valeurs fournies
 * ainsi que certaines constantes spécifiques au module test
 * Date : 23.03.2013
 * Auteur : Dylan Bourgeois
 * Version : 1.0
 */




int test_clock (int clock_init);

int test_oasis_size_parameter (int oasis_size_parameter);

int test_oasis_cell (char cell, int i, int j);

int test_number (int number, int param, int oasis_size);

int test_coordinates (int i, int number, int oasis_size, int param, int coord_x, int coord_y, float energy);
