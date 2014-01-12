/* Nom : graphic.c
 * Description : Contient les fonctions de dessin des différents 
 * éléments du monde, ainsi que le monde lui-même.
 * Date de dernière MAJ : 25.05.2013
 * Auteur : Dylan Bourgeois
 * Version : 2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "graphic.h"
#include "../simulation/simulation.h"
#include "../save_data/prey.h"
#include "../save_data/predator.h"
#include "../save_data/cell.h"
#include "../constantes.h"

#define WHITE 1.,1.,1.
#define BLACK 0.,0.,0.
#define LIGHT_BLUE 0,0.75,1.
#define LIGHT_BLUE_2 0.,1.,1.
#define ALPHA_VALUE 0.3
#define DARK_GREEN 0.2,0.55,0.04
#define PRED_RED 0.92,0.06,0.06
#define RED 0.72,0.06,0.06
#define DARK_YELLOW 0.9, 0.7, 0.



//Simulation
enum Status {STERILE=-1,OFF=0,START=4,SELECTED=5,PAUSE=5,VA_OFF=6,STEP=6, OUT_DAT=7};

float k, pas, percent;
int i, j, horloge, oasis, furthest;
static float x_mouse=0, y_mouse=0;
static int ox=0, oy=0;

//Fenêtre
enum Coord {X_MIN =-1,X_MAX=1,Y_MIN=-1,Y_MAX=1};
static int width, height;
static GLfloat x_min, x_max, y_min ,y_max;

PREY * p_prey = NULL;
PREDATOR * p_pred = NULL;

void graphic_monde ()
{
	p_prey=prey_get_tete_list();
	p_pred=pred_get_tete_list();
		
	int size=0;
	int mag = sim_get_zoom();
	int ox=0, oy=0;
	
	oasis = sim_get_oasis();
	furthest = sim_find_furthest();
	
	int m = (oasis-1)/2;
	int map = m;

	if (furthest > map && sim_get_va_status() != VA_OFF) size = 2*furthest + 1;
	else size = oasis+4;
	
	pas = 2./size;
	
	if (sim_get_mouse_status() == SELECTED && mag != OFF)
	{
		mag=sim_get_zoom();
		m=mag;
		size = 2.*mag +1;
		pas=2./size;
		
		ox = sim_get_x_mouse();
		oy = sim_get_y_mouse();
	}
	
	//Oasis
	glBegin(GL_QUADS);
	glColor3f(BLACK);
	{
		glVertex2f((1.+2.*(map-ox))/size, (1.+2.*(map-oy))/size);
		glVertex2f((-1.-2.*(map+ox))/size, (1.+2.*(map-oy))/size);
		glVertex2f((-1.-2.*(map+ox))/size, (-1.-2.*(map+oy))/size);
		glVertex2f((1.+2.*(map-ox))/size, (-1.-2.*(map+oy))/size);
	}
	glEnd();
	
	for (i=0; i<oasis; i++)
	{	
		for (j=0; j<oasis;j++)
		{	
			float energy = cell_get_energy(i,j);
			char c_energy[10] ;
		
			sprintf(c_energy, "%.0f", energy);
			//Végétation
			glBegin(GL_QUADS);
			//Stérile	
			if (energy == STERILE) 
			{
				glColor3f(WHITE);
				
				glVertex2f((2.*(j-map-ox))/size,(-1.+2.*(i-map-oy))/size);
				glVertex2f((2.*(j-map-ox))/size,(1.+2*(i-map-oy))/size);
				glVertex2f((1.+2*(j-map-ox))/size,(1.+2.*(i-map-oy))/size);
				glVertex2f((1.+2*(j-map-ox))/size,(-1.+2.*(i-map-oy))/size);	
			}
			//Vivante
			else 
			{
				glColor3f(DARK_GREEN);
			
				glVertex2f((2.*(j-map-ox))/size,(-1.+2.*(i-map-oy))/size);
				glVertex2f((2.*(j-map-ox))/size,(-1.+2.*(energy/9.)+2*(i-map-oy))/size);
				glVertex2f((1.+2*(j-map-ox))/size,(-1.+2.*(energy/9)+2.*(i-map-oy))/size);
				glVertex2f((1.+2*(j-map-ox))/size,(-1.+2.*(i-map-oy))/size);
			}
			glEnd();
			
			if (sim_get_mouse_status()==SELECTED && sim_get_zoom()==1 && energy != STERILE)
			{
				glColor3f(WHITE);
				graphic_render_string((0.2+2.*(j-map-ox))/size,(-0.8+2.*(i-map-oy))/size, GLUT_BITMAP_9_BY_15, c_energy);
			}
			
			if (sim_get_mouse_status()==SELECTED && sim_get_zoom()==1 && energy == STERILE)
			{
				glColor3f(DARK_GREEN);
				graphic_render_string((0.05+2.*(j-map-ox))/size,(-0.8+2.*(i-map-oy))/size, GLUT_BITMAP_9_BY_15, "INFERTILE");
			}
		}	
	}
	
	ox=0; oy=0;
	
	//Proies
	int x = 0;
	int y = 0;
	
	while (p_prey)
	{
		x = prey_get_x(p_prey);
		y = prey_get_y(p_prey);
		ox = sim_get_x_mouse();
		oy = sim_get_y_mouse();
		
		//Si la proie se trouve dans l'oasis, on vérifie si d'autres se trouvent au même endroit
		percent = (float)(cell_multiple_prey_percentage(y+map+DESERT_SIZE, x+map+DESERT_SIZE));
		
		//Si l'option zoom est active, on adapte les coordonnées
		if (sim_get_mouse_status() == SELECTED && sim_get_zoom() != OFF)
		{
			x = x - ox;
			y = y - oy;
		}
				
		//Proies vivantes
		glBegin(GL_POLYGON);
		glColor3f(LIGHT_BLUE);
			
		glVertex2f((-1.+2.*x)/size,(-1.+2.*y)/size);
		glVertex2f((-1.+2.*x)/size, (2.*y)/size);
		glVertex2f((2.*x)/size,(2.*y)/size);
		glVertex2f((2.*x)/size, (-1.+2.*y)/size);
		glEnd();
		
		//Proies mortes
		glBegin(GL_POLYGON);
		glColor3f(DARK_YELLOW);
					
		glVertex2f((-1.+2.*x)/size,(-1.+2.*y)/size);
		glVertex2f((-1.+2.*x)/size, (-1.+2.*y+percent)/size);
		glVertex2f((2.*x)/size,(-1.+2.*y+percent)/size);
		glVertex2f((2.*x)/size, (-1.+2.*y)/size);			
		glEnd();
		
		p_prey=prey_get_next(p_prey);
		x=0; y=0;
		ox=0; oy=0;
	
	}
	p_prey=prey_get_tete_list();
	
	
	
	while (p_pred)
	{
		int x = pred_get_x(p_pred);
		int y = pred_get_y(p_pred);
		char c_energy[10] ;
		
		sprintf(c_energy, "%.2f", pred_get_energy(p_pred));
		
		ox = sim_get_x_mouse();
		oy = sim_get_y_mouse();
	
		
		//Si l'option zoom est active, on adapte les coordonnées
		if (sim_get_mouse_status() == SELECTED && sim_get_zoom() != OFF)
		{
			x = x - ox;
			y = y - oy;
		}

		glBegin(GL_POLYGON);
		glColor3f(PRED_RED);
		
		glVertex2f((-1.+2.*x)/size,(2.*y)/size);
		glVertex2f((-1+2.*x)/size, (1.+2.*y)/size);
		glVertex2f((2.*x)/size,(1.+2.*y)/size);
		glVertex2f((2.*x)/size, (2.*y)/size);
		glEnd();
		
		p_pred=pred_get_next(p_pred);
		x=0; y=0;
		ox=0; oy=0;
	}
	p_pred=pred_get_tete_list();
	
	//Cellule séléctionnée mise en surbrillance si l'option zoom est active
	if (sim_get_mouse_status() == SELECTED && sim_get_zoom() != OFF)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glBegin(GL_QUADS);
	
		glColor4f(LIGHT_BLUE_2, ALPHA_VALUE);
			
		glVertex2f(-1./size, -1./size);
		glVertex2f(-1./size, 1./size);
		glVertex2f(1./size, 1./size);
		glVertex2f(1./size, -1./size);
		
		glEnd();
	}
	
	//Cadre délimitant l'oasis
	glLineWidth(3.0);
	glBegin(GL_LINES);
	
	if (sim_get_mouse_status() == SELECTED && sim_get_zoom() != OFF)
	{
		ox = sim_get_x_mouse();
		oy = sim_get_y_mouse();
	}
	
	else
	{
		ox=0;
		oy=0;
	}
	
	glColor3f(DARK_GREEN);
	{
		glVertex2f((1.+2.*(map-ox))/size, (1.+2.*(map-oy))/size);
		glVertex2f((-1.-2.*(map+ox))/size, (1.+2.*(map-oy))/size);
		
		glVertex2f((-1.-2.*(map+ox))/size, (1.+2.*(map-oy))/size);
		glVertex2f((-1.-2.*(map+ox))/size, (-1.-2.*(map+oy))/size);
		
		glVertex2f((-1.-2.*(map+ox))/size, (-1.-2.*(map+oy))/size);
		glVertex2f((1.+2.*(map-ox))/size, (-1.-2.*(map+oy))/size);
		
		glVertex2f((1.+2.*(map-ox))/size, (-1.-2.*(map+oy))/size);
		glVertex2f((1.+2.*(map-ox))/size, (1.+2.*(map-oy))/size);
	}
	glEnd();
	ox=0;
	oy=0;
	
	if (sim_get_mouse_status()==SELECTED && sim_get_zoom()==1)
	{
		int ox=sim_get_x_mouse();
		int oy=sim_get_y_mouse();
		
		for (i=0;i<oasis;i++)
		{
			for(j=0;j<oasis;j++)
			{
				draw_prey_energy (i, j, ox, oy, map, size);
				draw_pred_energy (i, j, ox, oy, map, size);
			}
		}
	}
	
	//Grille du monde
	glLineWidth(1.0);
	glBegin(GL_LINES);
	glColor3f(RED);
	
	for (k=-1; k<1; k+=pas)
	{
		//lignes
		glVertex2f(0,k);
		glVertex2f(-1,k);
		glVertex2f(0,k);
		glVertex2f(1,k);

		//colonnes
		glVertex2f(k,0);
		glVertex2f(k,-1);
		glVertex2f(k,0);
		glVertex2f(k,1);
	}
	glEnd ();
	
	m=0;
}


void graphic_reshape_cb (int w, int h)
{
	glViewport(0,0,w,h);
	
	width = w;
	height = h;
	
	float aspect_ratio = (float) (width)/height;
	
	if (aspect_ratio <= 1.)
	{
		x_min=X_MIN; x_max=X_MAX;
		y_min=Y_MIN/aspect_ratio; y_max=Y_MAX/aspect_ratio;
	}
	else
	{
		x_min=X_MIN*aspect_ratio; x_max=X_MAX*aspect_ratio;
		y_min=Y_MIN; y_max=Y_MAX;
	}
	
}

void graphic_display_cb ()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	glOrtho(x_min, x_max, y_min, y_max, -1.,1.);

	if (sim_get_status())
	{			 
		if (sim_get_status() == STEP)
		{
			if (sim_get_output_status()==OUT_DAT) save_out_dat();
			simulation_update();
			sim_save_time(sim_get_time()+1);
			set_cb();
			sim_save_status(1);
		}
		
		if (sim_get_status() == START)
		{
			if (sim_get_output_status()==OUT_DAT) save_out_dat();
			simulation_update();
			sim_save_time(sim_get_time()+1);
			set_cb();
		}
		
		if (sim_get_status() == PAUSE)
		{
			graphic_monde();
		}
		
		else graphic_monde();
	} 
	
	else glClearColor(WHITE,0);
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void graphic_souris_cb (int button, int state, int x, int y)
{
	int furthest = sim_find_furthest();

	button = GLUT_LEFT_BUTTON;
	
	if (sim_get_mouse_status() == SELECTED && sim_get_zoom() != OFF)
	{
		furthest = sim_get_zoom();
	}
	
	if (state == GLUT_DOWN)
	{
		if (sim_get_zoom() != OFF)
		{
			//On adapte les coordonnées de la souris à la fenêtre
			x_mouse = ((float)x)/ (600.);
			y_mouse = (600.-(float)y)/(600.);
			
			//On repère l'indice de la cellule sélectionnée
			int zx = (int) (x_mouse * (2*furthest+1));
			int zy = (int) (y_mouse * (2*furthest+1));
			
			if (sim_get_status() == SELECTED && sim_get_zoom() != OFF)
			{	
				zx += sim_get_x_mouse();
				zy += sim_get_y_mouse();
			}
			
			//On adapte à l'origine centrée à l'écran
			ox += (zx - furthest);
			oy += (zy - furthest);
						
			sim_save_mouse (ox, oy);
			sim_save_mouse_status(SELECTED);
			zx=0; zy=0; x_mouse=0; y_mouse=0;
		}
	}
}


void graphic_render_string (float x, float y, void * font, const char * s)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2f(x,y);
	
	while(*s)
	{
		glutBitmapCharacter(font, *s);
		s++;
	}
}

void draw_prey_energy (int i, int j, int ox, int oy, int map, int size)
{
	glColor3f(WHITE);
	
	int nb_prey = cell_nb_prey_in_cell(i+DESERT_SIZE,j+DESERT_SIZE);
	PREY * p_prey=cell_get_prey_in_cell(i+DESERT_SIZE, j+DESERT_SIZE);
	PREY_CELL * prey_cell=cell_get_prey_cell(i+DESERT_SIZE, j+DESERT_SIZE);
	
	int x = j-ox-map;
	int y = i-oy-map;
	
	int pos_alive=0;
	int pos_dead = nb_prey;
	
	glLineWidth(0.5);
	
	if (nb_prey <= 5 && nb_prey != 0)
	{		
		while (p_prey)
		{
			char c_energy[10] ;
			sprintf(c_energy, "%.2f", prey_get_energy(p_prey));
			
			if (prey_alive(p_prey))
			{
				pos_alive++;
				
				glBegin(GL_LINES);
				glColor3f(WHITE);
				
				glVertex2f((-1+2.*x)/size,((((float)(-pos_alive))/(nb_prey))+2.*y)/size);
				glVertex2f((2.*x)/size,((((float)(-pos_alive))/(nb_prey))+2.*y)/size);
				glEnd();
				
				graphic_render_string((-0.8+2.*x)/size,((((float)(-pos_alive))/(nb_prey+0.6))+2.*y)/size, GLUT_BITMAP_9_BY_15, c_energy);
			}
					
			else
			{
				glBegin(GL_LINES);
				glColor3f(WHITE);
				
				glVertex2f((2.*x)/size,((((float)(-pos_dead))/(nb_prey))+2.*y)/size);
				glVertex2f((-1+2.*x)/size,((((float)(-pos_dead))/(nb_prey))+2.*y)/size);
				glEnd();
				
				graphic_render_string((-0.8+2.*x)/size,((((float)(-pos_dead))/(nb_prey+0.6))+2.*y)/size, GLUT_BITMAP_9_BY_15, c_energy);
				pos_dead--;
				
			}
			
			p_prey=cell_get_next_prey_in_cell(i+DESERT_SIZE, j+DESERT_SIZE, prey_cell);
			prey_cell=cell_get_next_prey_cell(i+DESERT_SIZE, j+DESERT_SIZE, prey_cell);
		}
	}
	pos_dead=0; pos_alive=0;

}

void draw_pred_energy (int i, int j, int ox, int oy, int map, int size)
{
	glColor3f(WHITE);
	int nb_pred = cell_nb_pred_in_cell(i+DESERT_SIZE,j+DESERT_SIZE);
	PREDATOR * p_pred=cell_get_pred_in_cell(i+DESERT_SIZE, j+DESERT_SIZE);
	PRED_CELL * pred_cell=cell_get_pred_cell(i+DESERT_SIZE, j+DESERT_SIZE);
	
	int x = j-ox-map;
	int y = i-oy-map;
	
	int pos=0;
	
	glLineWidth(0.5);
	
	if (nb_pred <= 5 && nb_pred != 0)
	{		
		while (p_pred)
		{
			char c_energy[10] ;
			sprintf(c_energy, "%.2f", pred_get_energy(p_pred));
			
			pos++;
				
			glBegin(GL_LINES);
			glColor3f(WHITE);
			glVertex2f((-1+2.*x)/size,((((float)(pos))/(nb_pred))+2.*y)/size);
			glVertex2f((2.*x)/size,((((float)(pos))/(nb_pred))+2.*y)/size);
			glEnd();
				
			graphic_render_string((-0.8+2.*x)/size,((((float)(pos))/(nb_pred+1))+2.*y)/size, GLUT_BITMAP_9_BY_15, c_energy);
			
			p_pred=cell_get_next_pred_in_cell(i+DESERT_SIZE, j+DESERT_SIZE, pred_cell);
			pred_cell=cell_get_next_pred_cell(i+DESERT_SIZE, j+DESERT_SIZE, pred_cell);
			
		}
	}
	pos=0;
}
