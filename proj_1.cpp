/* Nom : proj_2.cpp
 * Description : Module central contenant le main, responsable
 * de l'interaction avec l'utilisateur par le biais de fonctions
 * callback en réponse à la barre Control (GLUI). Appelle aussi 
 * l'affichage OpenGL.
 * Date de dernière MAJ : 24.05.2013
 * Auteur : Dylan Bourgeois 224797
 * Version : 3.0
 */  

#include <GL/glut.h>
#include <GL/glui.h>
#include <iostream>

extern "C" {
	#include "constantes.h"
	#include "simulation/simulation.h"
	#include "graphic/graphic.h"
	#include "save_data/cell.h"
	#include "save_data/predator.h"
	#include "save_data/prey.h"
			}

#define CHIFFRES 9    //Constante definie pour placer les tests 1-9 (chiffres) avant les test 10-12

//Variables de la simulation
int timer = sim_get_time();
int oasis = sim_get_oasis();
int predator_nb = pred_update_nb();
int prey_nb = prey_update_nb();
int m = 0;

//Statut de la simulation
int status = -1;
static int i=0;

//Variables et pointeurs d'énergie totale
float total_energy_cell =0., total_energy_pred=0., total_energy_prey=0.;

float * p_total_energy_cell=&total_energy_cell;
float * p_total_energy_predator=&total_energy_pred; 
float * p_total_energy_prey=&total_energy_prey;

//Statut de la callback
enum CallBack_Status {FILENAME_ID, LOAD_ID, OUT_ID, SAVE_ID,
 START_ID, PAUSE_ID, STEP_ID, OUTPUT_ID, VIEW_ALL_ID, VERBOSE_ID, STATUS_ID, ZOOM_ID, ZOOM_ACTIVATE_ID};
 
//Statut de la simulation
enum Control_Status {OFF, ON, OK, ERROR, PAUSE, SELECTED, VIEW_ALL_OFF};


namespace {	

	char test_file[20] = {"test1.txt"};
	char out_txt[20] = {"out.txt"};
	char out_dat[20] = {"out.dat"};

	int zoom=OFF, zoom_bool=OFF, view_all, verbose;
	
	GLUI_Panel *p_load_test;
	GLUI_Panel *p_simulation;
	GLUI_Panel *p_save;
	GLUI_Panel *p_options;
	GLUI_Panel *p_information;
	GLUI_Panel *p_zoom;
	
	GLUI_EditText *e_file_name_in;
	GLUI_EditText *e_file_name_out;
	GLUI_EditText *e_time;
	GLUI_EditText *e_size;
	GLUI_EditText *e_veg_energy;
	GLUI_EditText *e_prey_energy;
	GLUI_EditText *e_predator_energy;
	GLUI_EditText *e_prey_nb;
	GLUI_EditText *e_predator_nb;
	GLUI_EditText *e_status;
	
	GLUI_Button *b_load;
	GLUI_Button *b_pause;
	GLUI_Button *b_start;
	GLUI_Button *b_step;
	GLUI_Button *b_save;
	
	GLUI_Checkbox *c_output;
	GLUI_Checkbox *c_view_all;
	GLUI_Checkbox *c_verbose;
	GLUI_Checkbox *c_zoom;
	
	GLUI_Spinner *s_zoom;
}

/* Fonctions du module principal et callbacks */
//Callback de réponse aux actions de l'utilisateur sur la barre Control
void control_cb (int);
//Création de la barre Control (GLUI)
void glui_cb (void);
//Réinitialise les valeurs de la barre Control
void reset_glui (void);

void reset_glui (void)
{
	e_time->set_int_val(0.0);
	e_size->set_int_val(0);
	e_predator_nb->set_int_val(0);
	e_prey_nb->set_int_val(0);
	e_predator_energy->set_float_val(0.0);
	e_prey_energy->set_float_val(0.0);
	e_veg_energy->set_float_val(0.0);
	s_zoom->set_int_val(0);
	c_output->set_int_val(0);
	sim_save_mouse_status(OFF);
}

void set_cb (void)
{	
	predator_nb = pred_update_nb();
	prey_nb = prey_update_nb();
	timer=sim_get_time();
	m = (sim_get_oasis()-1)/2;
	view_all = sim_get_va_status();
	
	cell_total_energy(p_total_energy_cell, p_total_energy_predator, p_total_energy_prey);

	c_view_all->set_int_val (view_all);
	
	e_file_name_in->set_text(test_file);
	e_file_name_out->set_text(out_txt);
	e_time->set_int_val(timer);
	e_size->set_int_val(m);
	e_predator_nb->set_int_val(predator_nb);
	e_prey_nb->set_int_val(prey_nb);
	e_predator_energy->set_float_val(*p_total_energy_predator);
	e_prey_energy->set_float_val(*p_total_energy_prey);
	e_veg_energy->set_float_val(*p_total_energy_cell);
	if (c_zoom->get_int_val()) s_zoom->set_int_val(zoom);
	else 
	{
		s_zoom->set_int_limits(0,0);
		sim_save_mouse_status(OFF);
	}
	
	if (status == OFF)
	{
		p_simulation->disable();
		p_save->disable();
		p_options->disable();
		p_zoom->disable();
	}
	
	else if (status == ON)
	{
		p_simulation->enable();
		p_save->enable();
		p_options->enable();
		p_zoom->enable();
	}
	
	*p_total_energy_cell = 0.;
	*p_total_energy_predator = 0.;
	*p_total_energy_prey = 0.;
	
}

void control_cb (int control)
{	
	switch (control)
	{
		case LOAD_ID:
			sim_reset();
			if (!(sim_input(test_file))) 
			{
				status = OFF;
				sim_save_status(status);
				status = ERROR;
				e_status->set_text("Load Fail");
				reset_glui();
			}
			
			else
			{
				status = ON;
				sim_save_status(status);
				e_status->set_text ("Load OK");
				c_zoom->set_int_val(0);
				sim_save_zoom(0);
				set_cb ();
				s_zoom->set_int_limits(0,0);
				i=0;
			}
			break;
			
		case SAVE_ID:
			save_out_txt(SAVE_ID);
			e_status->set_text("Saved");
			break;
		
		case START_ID:
			e_status->set_text("Simulation");
			set_cb();
			status=START_ID;
			sim_save_status(START_ID);
			break;
			
		case PAUSE_ID:
			sim_save_status(PAUSE_ID);
			e_status->set_text("Paused");
			break;
			
		case ZOOM_ID:
			if (c_zoom->get_int_val())
			sim_save_zoom(zoom);
			else sim_save_zoom(0);
			break;
			
		case ZOOM_ACTIVATE_ID:
			if (!(c_zoom->get_int_val()))
			{
				zoom=OFF;
				s_zoom->set_int_limits(0,0);
				s_zoom->set_int_val(OFF);
				sim_save_zoom(zoom);
				sim_save_status(OK);
			}
			else
			{
				m = (sim_get_oasis()-1)/2;
				if (m==0) m=1;
				s_zoom->set_int_limits(1, m);
				zoom = 1;
				sim_save_zoom(zoom);
			}
			break;
		
		case OUTPUT_ID:
			save_out_dat();
			sim_save_output_status(OUTPUT_ID);
			break;
			
		case VIEW_ALL_ID:
			if (!view_all)
			{
				sim_save_va_status(VIEW_ALL_OFF);
			}
			
			else if (view_all)
			{
				sim_save_va_status(VIEW_ALL_ID);
			}
			break;
			
		case VERBOSE_ID:
			sim_save_verbose_status(ON);
			break;
		
		case STEP_ID:
			{
				status=STEP_ID;
				sim_save_status(STEP_ID);
				set_cb();
			}
	}
}

void glui_cb (void)
{
	static GLUI *glui = GLUI_Master.create_glui ((char*) "Control");
			
	//Load test panel
	p_load_test = glui->add_panel ((char *)"Load", GLUI_PANEL_EMBOSSED);
	e_file_name_in = glui->add_edittext_to_panel(p_load_test, (char *)"Filename",GLUI_EDITTEXT_TEXT, test_file);
	b_load = glui->add_button_to_panel(p_load_test, (char*) "Load", LOAD_ID, control_cb);
	
	//Save panel
	p_save = glui->add_panel ((char*)"Save", GLUI_PANEL_EMBOSSED);
	e_file_name_out = glui->add_edittext_to_panel(p_save, (char *)"Filename", GLUI_EDITTEXT_TEXT, out_txt);
	b_save = glui->add_button_to_panel(p_save, (char *)"Save", SAVE_ID, control_cb);
	
	//Simulation panel
	p_simulation = glui->add_panel ((char*)"Simulation", GLUI_PANEL_EMBOSSED);
	b_start = glui->add_button_to_panel(p_simulation, (char*) "Start", START_ID, control_cb);
	b_pause = glui->add_button_to_panel(p_simulation, (char*) "Pause", PAUSE_ID, control_cb);
	b_step = glui->add_button_to_panel(p_simulation, (char*) "Step", STEP_ID, control_cb);
	
	//Options panel
	p_options = glui->add_panel ((char *)"Options", GLUI_PANEL_EMBOSSED);
	c_output = glui->add_checkbox_to_panel(p_options, (char*) "Output", NULL , OUTPUT_ID, control_cb);
	c_view_all = glui->add_checkbox_to_panel(p_options, (char*) "View All", &view_all, VIEW_ALL_ID, control_cb);
	c_verbose = glui->add_checkbox_to_panel(p_options, (char*) "Verbose", &verbose, VERBOSE_ID, control_cb);
	
	//Zoom
	p_zoom = glui->add_panel ((char*) "Zoom", GLUI_PANEL_EMBOSSED);
	c_zoom = glui->add_checkbox_to_panel (p_zoom, (char*)"Activate", &zoom_bool, ZOOM_ACTIVATE_ID, control_cb);
	s_zoom = glui->add_spinner_to_panel(p_zoom, (char*)"Parameter", GLUI_SPINNER_INT, &zoom, ZOOM_ID, control_cb);
	s_zoom->set_int_limits(0,10);
	
	//Information panel
	p_information = glui->add_panel ((char *)"Informations", GLUI_PANEL_EMBOSSED);
	e_time = glui->add_edittext_to_panel(p_information, (char *)"Time", GLUI_EDITTEXT_FLOAT);
	e_size = glui->add_edittext_to_panel(p_information, (char *)"Size", GLUI_EDITTEXT_TEXT);
	e_veg_energy = glui->add_edittext_to_panel(p_information, (char *)"E. Vegetation", GLUI_EDITTEXT_FLOAT);
	e_predator_energy = glui->add_edittext_to_panel(p_information, (char *)"E. Predators", GLUI_EDITTEXT_FLOAT);
	e_prey_energy = glui->add_edittext_to_panel(p_information, (char *)"E. Preys", GLUI_EDITTEXT_FLOAT);
	e_predator_nb = glui->add_edittext_to_panel(p_information, (char *)"# Predators", GLUI_EDITTEXT_FLOAT);
	e_prey_nb = glui->add_edittext_to_panel(p_information, (char *)"# Preys", GLUI_EDITTEXT_FLOAT);
	
	//Status
	e_status = glui->add_edittext((char *)"Status", GLUI_EDITTEXT_TEXT);
	
	//Exit	
	glui->add_button((char*) "Exit", 0,(GLUI_Update_CB)exit );
		
}

void save_out_dat ()
{	
	FILE * out_file = NULL;
	
	cell_total_energy(p_total_energy_cell, p_total_energy_predator, p_total_energy_prey);
	
	if (i==0) 
	{
		out_file=fopen(out_dat, "w");
		fprintf (out_file, "#Name of test file : %s\n", test_file);
		i++;
		fclose(out_file);
	}
	
	out_file=fopen(out_dat, "a");
	
	fprintf (out_file, " %d %f %f %f\n", timer, *p_total_energy_cell, *p_total_energy_predator, *p_total_energy_prey);
	
	fclose(out_file);	
}

void save_out_txt (int param)
{
	FILE * out_file = NULL;
	PREY * p_prey = NULL;
	PREDATOR * p_pred = NULL;
	
	//Si on cherche à enregister le out.txt
	if (param == SAVE_ID)
	{
		out_file = fopen(out_txt, "w");
		
		fprintf(out_file, "#Name of test file : %s\n", test_file);
	
		fprintf(out_file, "%d\n", sim_get_time());
	
		fprintf(out_file, "%d\n", (sim_get_oasis()-1)/2);
	
		cell_afficher (out_file);	
		
	}
	
	//Si l'info est demandée pour le mode Verbose
	else if (param == VERBOSE_ID)
	{
		out_file = stdout;
	}

	fprintf (out_file, "%d\n", prey_update_nb());
	
	p_prey = prey_get_tete_list();

	while (p_prey)
	{
		fprintf (out_file, "%d %d %d %f\n", prey_get_x(p_prey), prey_get_y(p_prey), prey_alive(p_prey), prey_get_energy(p_prey));
		
		p_prey=prey_get_next(p_prey);
	}
	
	fprintf (out_file, "%d\n", pred_update_nb());
	
	p_pred = pred_get_tete_list();
	
	while (p_pred)
	{
		fprintf (out_file, "%d %d %f\n", pred_get_x(p_pred), pred_get_y(p_pred), pred_get_energy(p_pred));
		
		p_pred=pred_get_next(p_pred);
	}
	
	if (param == SAVE_ID) fclose(out_file);
}


int main(int argc, char** argv)
{
	if (argc == 2) 
	{
		status = ON;
		sim_save_status(status);
		sim_input (argv[1]);
	}
	
	else 
	{
		status = OFF;
		sim_save_status(status);
	}
	
	/*Initialisation de GLUT*/
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0.,0.);
	glutInitWindowSize(600,600);

	glutCreateWindow("OASIS");
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(1.,1.,1.,0.);
	
	glutSwapBuffers();
	
	glutDisplayFunc ( graphic_display_cb );
	glutReshapeFunc ( graphic_reshape_cb );

	glutMouseFunc ( graphic_souris_cb );

	glui_cb();

	set_cb();
	
	if (argc == 2)
	{
		e_file_name_in->set_text(argv[1]);
	}

	glutMainLoop();

	return 0;
}
