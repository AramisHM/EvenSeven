// Simulador de Interrupções e Eventos
// Criado por Aramis Hornung Moraes - 2014

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>		// fl_beep()
#include <FL/Fl_Browser.H>
#include "sim_interrupt.h"



// variaveis globais da simulação
int total_interruptions_count;
a_pre_interrupt *pre_interruption_vector;			// prevetor fonte para determinar interrupcoes			 (OK)
a_interrupt *interruption_vector;					// somente interrupcoes									 (OK)
int proc_size;										// duração do processo normal da simulacao;				 (OK)
int sim_duration;									// duração total										 (OK)
a_interrupt_repre *interruption_representation_vec;	// vetor de representação das interrupções
int n_representations = 0;							// number of distinct events (by name)

float *simulation_vector = 0;						// vetor final, compondo o processo normal, FLOAT, POIS O GRÁFICO DESENHA COM FLOAT
													// com as interrupcoes - usado para imprimir o grafico	 (OK)

// Aux funcs
void parse_file(char *file_name);
int create_graph();
int draw_graph(float *input_vec, int n_elem, bool grid);
int close_graph();

Fl_Browser *console; // WHERE WE BASICALLY SHOW INFO

void Butt_CB(Fl_Widget*, void*)
{
	// Create native chooser
	Fl_Native_File_Chooser native;
	native.title("Load simulation");
	native.type(Fl_Native_File_Chooser::BROWSE_FILE);
	native.filter("Interrupt Simulation file\t*.{ais}\n"
				  "Text\t*.txt\n");

	//native.preset_file(G_filename->value());
	// Show native chooser
	switch (native.show())
	{
	case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
	case  1: fprintf(stderr, "*** CANCEL\n"); fl_beep(FL_BEEP_DEFAULT); break;		// CANCEL
			
	default: 								// PICKED FILE
		if (native.filename())
		{
			
			parse_file((char*)native.filename()); // returns the local of the file choosen
			//close_graph();
			create_graph(); // TODO: REMOVE

		}
		break;
	}
}


int main()
{
	
	Fl_Double_Window* main_window = new Fl_Double_Window(790, 455, "EvenSeven");

	Fl_Button *open,
			  *v_grafico,
			  *next_i,
			  *auto_i,
			  *finish;
	Fl_Output *cur_inst;

	Fl_Value_Input *delay_val;
	//create_graph(); // TODO: UNCOMMENT
	
	Fl_Group* group = new Fl_Group(0, 25, 790, 430, "EvenSeven");
	group->labelfont(14);
	group->labelsize(19);
	group->align(Fl_Align(FL_ALIGN_TOP_LEFT));
	Fl_Group::current()->resizable(group);
	
	console = new Fl_Browser(5, 75, 780, 315);
	Fl_Group::current()->resizable(console);

	open = new Fl_Button(5, 405, 125, 30, "Carregar");
	open->callback(Butt_CB);

	Fl_Text_Display* textdisplay = new Fl_Text_Display(5, 45, 775, 30, "Created By Aramis Hornung Moraes");
	textdisplay->labelfont(11);
	textdisplay->align(Fl_Align(FL_ALIGN_TOP_LEFT));
	v_grafico =  new Fl_Button(145, 405, 125, 30, "Desenhar Gr\303\241""fico");

	main_window->show();
	main_window->end();


	// Debug test
	/*
	for(int kok = 0; kok < 40; kok++)
	{
		parse_file("exemplo.ais");
	}
	*/

	while (Fl::check())
	{
		//draw_graph((float*)simulation_vector, sim_duration);
		if(simulation_vector)
		{
			draw_graph(simulation_vector, sim_duration,true);
		}
	}


	close_graph();

	// free the simulator's global variables, the vectors tthat stores the simulation information
	if(simulation_vector)
	{
		free(simulation_vector);
		simulation_vector = 0;	
	}
	if(interruption_representation_vec)
	{
		free(interruption_representation_vec);
		interruption_representation_vec = 0;
	}
	return 1;
}

