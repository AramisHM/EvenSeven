// Simulador de Interrupções e Eventos
// Criado por Aramis Hornung Moraes - 2014

typedef struct a_pre_interrupt // estrutura para pre armazenamento de interrupcoes
{
	char interrupt_name[32];
	int start_inst_a;
	int start_inst_b;
	int duration_inst_a;
	int duration_inst_b;
	int quantity_a;
	int quantity_b;
	int defined_quantity;
}a_pre_interrupt;

typedef struct a_interrupt	// eventos regulares
{
	char interrupt_name[32];
	int start_inst;
	int duration_inst;
	bool visited; // We use this variable to auxiliate during the ordenation of the vector... 'CAUSE USER CAN BE A DOUCHE AND WRITE THE EVENTS OUT OF ORDER... YOU KNOW....
}a_interrupt;

typedef struct adep_interrupt // eventos dependentes
{
	char dep_name[32];
	unsigned int duration_inst;
}adep_interrupt;

typedef struct a_interrupt_repre	// vetor de representação das interrupções; Usado para definir um indice,
									//para posteriormente desenhar em um gráfico, assim, o indice é o mesmo para todos os eventos de mesmo nome
{
	char name[32];
}a_interrupt_repre;

// variaveis globais da simulação
extern int total_interruptions_count;
extern a_pre_interrupt *pre_interruption_vector;;
extern a_interrupt *interruption_vector;	// somente interrupcoes
extern int proc_size;						// duração do processo normal da simulacao;
extern int sim_duration;					// duração total
extern float *simulation_vector;			// vetor final, compondo o processo normal, com as interrupcoes



