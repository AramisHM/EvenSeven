// Simulador de Interrupções e Eventos
// Criado por Aramis Hornung Moraes - 2014

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctype.h>// isdigit
#include "sim_interrupt.h"
#include <stdlib.h>     /* srand, rand */
#include <string.h>     /* srand, rand */

// vairiaveis globais
char *dep_int_file_buff;	long dep_int_file_len;
char *sim_file_buff;		long sim_file_len;
int ic; // interruption counter (use to store interruption on the go)
extern a_interrupt_repre *interruption_representation_vec;
extern int n_representations;



// FLTK
#include <FL/Fl.H>
#include <FL/Fl_Browser.H>
extern Fl_Browser *console;


// ahm library functions
int file_size(char* file_name) // Return a pointer for the file data. Dont forget to free the memory.
{   
	FILE *fp;
	long file_size;

	fp = fopen (file_name , "r");
	if(!fp)	
	{
		printf("Can't find or open the file \"%s\" for reading.\n", file_name);
		return NULL;
	}

	// Get the file size in bytes.
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fclose(fp);
	//rewind(fp);

	return file_size;
}

char *load_file_to_cstring(char* file_name) // Return a pointer for the file data. Dont forget to free the memory.
{   
	FILE *fp;
	long file_size;
	char *buffer; // Memeory.
	int i; // Counter.

	fp = fopen (file_name , "r");
	if(!fp)	
	{
		printf("Can't find or open the file \"%s\" for reading.\n", file_name);
		return NULL;
	}

	// Get the file size in bytes.
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	buffer = (char*)malloc(sizeof(char)*(file_size + 1)); // Allocate memory plus one byte for null termination.

	if(!buffer)
	{
		fclose(fp);
		printf("Can't allocate memory for bufferizing.\n");
		return NULL;
	}

	for(i = 0; i < file_size; i++) // Read the file to memory.
		buffer[i] = fgetc(fp);
	buffer[file_size] = '\0'; // Make a null termination on the memory.

	fclose(fp);

	return buffer;
}

int parse_file_get_dep_interruptions_count(char *event_name)
{
	int interruptions_Count = 0;
	char *program = dep_int_file_buff;//= load_file_to_cstring(file_name);
	int pc; // program pointer

	// automato para fazer parse do código ----------------------- A U T O M A T O ----------------------------
	pc = 0;
	int i = 0;
	int found = 0;

	while(pc <= dep_int_file_len && dep_int_file_len > 0)
	{
		while(program[pc]!=';' && pc <= dep_int_file_len)
		{
			if(event_name[i] == program[pc])
			{
				i++;
				if(program[pc+1] == ';')
				{
					found = 1;
				}
			}
			pc++;
		}

		if(found)
		{
			while(program[pc]!='}')
			{
				// !------------ identifica comentário
				if(program[pc] == '/')
				{
					if(program[pc+1] == '/');
					{
						pc++;
						do{
							pc++;
						}while(program[pc] != '\n' && pc < dep_int_file_len);
					}
				}

				// !------------ identifica interrupção
				if(program[pc] == 'e')
				{
					if(program[pc+1] == 'v')
					{
						if(program[pc+2] == 'e')
						{
							if(program[pc+3] == 'n')
							{
								if(program[pc+4] == 't')
								{

										pc+=5;
										interruptions_Count++;

										/*
										while (program[pc] != ',') // PULA INICIO
										{
											pc++;
										}
										pc++;
										*/ // versão 7 não suporta INICIO para dependentes

										while (program[pc] != ',') // PULA DURACAO
										{
											pc++;
										}
										pc++;

										// Name		--------------------------------------------------
										int i = 0;
										char name[64];
										for(int ti = 0; ti < 64; ti++) // clean buffers
										name[ti] = 0;
									

										while(program[pc] != ')') // 
										{
											if(program[pc] != ' ') // caracteres nao permitidos para nomes
											{
												name[i] = program[pc];
												i++;
											}
											pc++;
										}
								
									interruptions_Count += parse_file_get_dep_interruptions_count(name); // RECURSIVIDADE
									
								}//
							}
						}
					}
				}
				pc++;
			}
			found = 0; // reset
		}
		pc++;
	}

	return interruptions_Count;
}

int parse_file_get_interruptions_precount(char *file_name)
{
	int interruptions_Count = 0;
	char *program;
	program = sim_file_buff;//load_file_to_cstring(file_name);
	int pc; // program pointer

	// automato para fazer parse do código
	pc = 0;
	while(pc <= sim_file_len)
	{
		// !------------ identifica comentário
		if(program[pc] == '/')
		{
			if(program[pc+1] == '/');
			{
				pc++;
				do{
					pc++;
				}while(program[pc] != '\n' && pc < sim_file_len);
			}
		}

		// !------------ identifica interrupção
		if(program[pc] == 'e')
		{
			if(program[pc+1] == 'v')
			{
				if(program[pc+2] == 'e')
				{
					if(program[pc+3] == 'n')
					{
						if(program[pc+4] == 't')
						{


							pc+=5;
								interruptions_Count++;
								/*
								while (program[pc] != ',')
								{
									pc++;
								}
								pc++;
								while (program[pc] != ',')
								{
									pc++;
								}
								pc++;

								// Name		--------------------------------------------------
								int i = 0;
								char name[64];
								for(int ti = 0; ti < 64; ti++) // clean buffers
									name[ti] = 0;
								*/
								//interruptions_Count += parse_file_get_dep_interruptions_count(name);
							
						}//
					}
				}
			}
		}

		pc++;
	}
	return interruptions_Count;
}


int parse_file_define_pre_dep_interruptions(char *event_name) // carrega eventos dependentes no vetor de preinterrupcoes
{
	//char *file_name = "e_dependentes.aed";
	int interruptions_Count = 0;
	//FILE *fp;
//	int dep_file_size = file_size(file_name);
	char *program = dep_int_file_buff;//load_file_to_cstring(file_name);

	int pc; // program pointer

	//fp = fopen(file_name, "r"); // open for parsering

	// automato para fazer parse do código ----------------------- A U T O M A T O ----------------------------
	pc = 0;
	int i = 0;
	int found = 0;

	while(pc <= dep_int_file_len && dep_int_file_len > 0)
	{
		while(program[pc]!=';' && pc <= dep_int_file_len)
		{
			if(event_name[i] == program[pc])
			{
				i++;
				if(program[pc+1] == ';')
				{
					found = 1;
				}
			}
			pc++;
		}

		if(found)
		{
			while(program[pc]!='}')
			{
				// !------------ identifica comentário
				if(program[pc] == '/')
				{
					if(program[pc+1] == '/');
					{
						pc++;
						do{
							pc++;
						}while(program[pc] != '\n' && pc < dep_int_file_len);
					}
				}

				// !------------ identifica interrupção
				if(program[pc] == 'e')
				{
					if(program[pc+1] == 'v')
					{
						if(program[pc+2] == 'e')
						{
							if(program[pc+3] == 'n')
							{
								if(program[pc+4] == 't')
								{
										pc+=5;
										interruptions_Count++;			

										int i = 0;
										int numf = 0; // number flag to alternate between number and number2;
										char number[10];
										char number2[10];
										int num, num2;


										pre_interruption_vector[ic].start_inst_a = -1;
										pre_interruption_vector[ic].start_inst_b = -1;

										// DURACAO		--------------------------------------------------
										i = 0;
										numf = 0;
										for(int ti = 0; ti < 10; ti++) // clean buffers
											number[ti]=number2[ti]= 0;

										while(program[pc] != ',')  // 
										{
											if(program[pc] == '.' && program[pc+1] == '.')
											{
												numf = 1; // alternate for randomize
												pc += 2; // registrate that we read more
												i = 0; // update the index to write on the other buffer (number2)
											}
											if(isdigit(program[pc]) && !numf)
											{
												number[i] = program[pc];
												i++;
											}
											else if(isdigit(program[pc]) && numf)
											{
												number2[i] = program[pc];
												i++;
											}
											pc++;
										}
										if(numf)
										{
											//pre_interruption_vector[ic].duration_inst = rand()% (atoi(number2) - atoi(number)) + atoi(number);
											pre_interruption_vector[ic].duration_inst_a = atoi(number);
											pre_interruption_vector[ic].duration_inst_b = atoi(number2);
										}
										else
										{
											//pre_interruption_vector[ic].duration_inst = atoi(number);
											pre_interruption_vector[ic].duration_inst_a = atoi(number);
											pre_interruption_vector[ic].duration_inst_b = atoi(number);
										}
										pc++;

										// NOME		--------------------------------------------------
										i = 0;
										char name[64];
										for(int ti = 0; ti < 64; ti++) // clean buffers
										name[ti] = 0;
									

										while(program[pc] != ')') // 
										{
											if(program[pc] != ' ') // caracteres nao permitidos para nomes
											{
												name[i] = program[pc];
												i++;
											}
											pc++;
										}
								
										//interruptions_Count += parse_file_get_dep_interruptions_count(name); // RECURSIVIDADE
										ic++; // achamos uma interrupcao dentro dos dependentes, incrementa no vetor para escrever a interrupcao no vetor;
									
								}//
							}
						}
					}
				}
				pc++;
			}
			found = 0; // reset
		}
		pc++;
	}
	return interruptions_Count;
}


int parse_file_define_pre_interruptions(char *file_name) // passo I
{
	int interruptions_Count = 0;

	char *program  = sim_file_buff;//= load_file_to_cstring(file_name);
	int pc; // program pointer

	interruptions_Count = parse_file_get_interruptions_precount(file_name);

	
	pre_interruption_vector = (struct a_pre_interrupt*)malloc(sizeof(struct a_pre_interrupt)*interruptions_Count);


	
	// automato para fazer parse do código
	pc = 0;
	ic = 0;
	while(pc <= sim_file_len)
	{
		// !------------ identifica comentário
		if(program[pc] == '/')
		{
			if(program[pc+1] == '/');
			{
				pc++;
				do{
					pc++;
				}while(program[pc] != '\n' && pc < sim_file_len);
			}
		}


		// !------------ identifica interrupção
		if(program[pc] == 'e')
		{
			if(program[pc+1] == 'v')
			{
				if(program[pc+2] == 'e')
				{
					if(program[pc+3] == 'n')
					{
						if(program[pc+4] == 't' && program[pc+5] != 's')
						{
								pc+=5;
								//interruptions_Count++;

								int i = 0;
								int numf = 0; // number flag to alternate between number and number2;
								char number[10];
								char number2[10];
								int num, num2;
								
								// COMECO		--------------------------------------------------
								i = 0;
								numf = 0;
								for(int ti = 0; ti < 10; ti++) // clean buffers
									number[ti]=number2[ti]= 0;
								
								while(program[pc] != ',' && program[pc] < sim_file_len) // 
								{
									if(program[pc] == '.' && program[pc+1] == '.')
									{
										numf = 1; // alternate for randomize
										pc += 2; // registrate that we read more
										i = 0; // update the index to write on the other buffer (number2)
									}
									if(isdigit(program[pc]) && !numf)
									{
										number[i] = program[pc];
										i++;
									}
									else if(isdigit(program[pc]) && numf)
									{
										number2[i] = program[pc];
										i++;
									}
									pc++;
								}
								
								
								if(numf)
								{
									//pre_interruption_vector[ic].start_inst = rand()% (atoi(number2) - atoi(number)) + atoi(number);
									pre_interruption_vector[ic].start_inst_a = atoi(number);
									pre_interruption_vector[ic].start_inst_b = atoi(number2);
								}
								else
								{
									//pre_interruption_vector[ic].start_inst = atoi(number);
									pre_interruption_vector[ic].start_inst_a = atoi(number);
									pre_interruption_vector[ic].start_inst_b = atoi(number);
								}
								pc++; // must have. precisa pular o atual ",", senão ele passa reto pela proxima compração while*[codkk21] "while(program[pc] != ',') "
								
								
								// DURACAO		--------------------------------------------------
								i = 0;
								numf = 0;
								for(int ti = 0; ti < 10; ti++) // clean buffers
									number[ti]=number2[ti]= 0;

								while(program[pc] != ',')  // *[codkk21] essa compracão aqui que eu estava flando
								{
									if(program[pc] == '.' && program[pc+1] == '.')
									{
										numf = 1; // alternate for randomize
										pc += 2; // registrate that we read more
										i = 0; // update the index to write on the other buffer (number2)
									}
									if(isdigit(program[pc]) && !numf)
									{
										number[i] = program[pc];
										i++;
									}
									else if(isdigit(program[pc]) && numf)
									{
										number2[i] = program[pc];
										i++;
									}
									pc++;
								}
								if(numf)
								{
									//pre_interruption_vector[ic].duration_inst = rand()% (atoi(number2) - atoi(number)) + atoi(number);
									pre_interruption_vector[ic].duration_inst_a = atoi(number);
									pre_interruption_vector[ic].duration_inst_b = atoi(number2);
								}
								else
								{
									//pre_interruption_vector[ic].duration_inst = atoi(number);
									pre_interruption_vector[ic].duration_inst_a = atoi(number);
									pre_interruption_vector[ic].duration_inst_b = atoi(number);
								}
								pc++; // must have.
								

								// QUANTIDADE		--------------------------------------------------
								i = 0;
								numf = 0;
								for(int ti = 0; ti < 10; ti++) // clean buffers
									number[ti]=number2[ti]= 0;

								while(program[pc] != ',')  // 
								{
									if(program[pc] == '.' && program[pc+1] == '.')
									{
										numf = 1;	// alternate for randomize
										pc += 2;	// registrate that we read more
										i = 0;		// update the index to write on the other buffer (number2)
									}
									if(isdigit(program[pc]) && !numf)
									{
										number[i] = program[pc];
										i++;
									}
									else if(isdigit(program[pc]) && numf)
									{
										number2[i] = program[pc];
										i++;
									}
									pc++;
								}
								if(numf)
								{
									//pre_interruption_vector[ic].quantity = rand()% (atoi(number2) - atoi(number)) + atoi(number);
									pre_interruption_vector[ic].quantity_a = atoi(number);
									pre_interruption_vector[ic].quantity_b = atoi(number2);
								}
								else
								{
									pre_interruption_vector[ic].quantity_a = atoi(number);
									pre_interruption_vector[ic].quantity_b = atoi(number);
								}
								pc++; // must have.

								

								// Name		--------------------------------------------------
								i = 0;
								char name[64];
								for(int ti = 0; ti < 64; ti++) // clean buffers
									name[ti] = 0;
									

								while(program[pc] != ')') // 
								{
									if(program[pc] != ' ' && program[pc] != '\t') // caracteres nao permitidos para nomes
									{
										name[i] = program[pc];
										i++;
									}
									pc++;
								}
								strcpy(pre_interruption_vector[ic].interrupt_name, name);
								
								ic++; // terminamos de gravar;
						}//
					}
				}
			}
		}
		pc++;
	}

	return interruptions_Count;
}

/* Verifica se tem dependencia, se tiver, ele irá inserir no vetor de dependencia, ATENÇÃO ESTA ROTINA TRABALHA DE ACORDO COM A VARIAVEL GLOBAL "ic" - INTERRUPTION COUNTER - */
void verify_depen(char *event_name)
{
	//char *file_name = "e_dependentes.aed";
	//FILE *fp;
	//int dep_file_size = file_size(file_name);
	char *program = dep_int_file_buff;//load_file_to_cstring(file_name);

	int pc; // program counter
	//int ic; // interruption counter (use to store interruption on the go)

	//fp = fopen(file_name, "r"); // open for parsering
	// automato para fazer parse do código ----------------------- A U T O M A T O ----------------------------
	pc = 0;
	int i = 0;
	int found = 0;
	while(pc <= dep_int_file_len)
	{
		while(program[pc]!=';' && pc <= dep_int_file_len)
		{
			if(event_name[i] == program[pc])
			{
				i++;
				if(program[pc+1] == ';')
				{
					found = 1;
				}
			}
			pc++;
		}

		if(found)
		{
			ic++; // incrementa no ventor de interrupções um evento dependente
			while(program[pc]!='}')
			{
				// !------------ identifica comentário
				if(program[pc] == '/')
				{
					if(program[pc+1] == '/');
					{
						pc++;
						do{
							pc++;
						}while(program[pc] != '\n' && pc < dep_int_file_len);
					}
				}

				// !------------ identifica interrupção
				if(program[pc] == 'e')
				{
					if(program[pc+1] == 'v')
					{
						if(program[pc+2] == 'e')
						{
							if(program[pc+3] == 'n')
							{
								if(program[pc+4] == 't')
								{
										pc+=5;
										if(program[pc] == '(');
										{

											int i = 0;
											int numf = 0; // number flag to alternate between number and number2;
											char number[10];
											char number2[10];
											int num, num2;


											
											interruption_vector[ic].start_inst = -1; // eventos dependentes não possuem um instante para ocorrer, eles ocorrem apos o evento pai



											// DURATION		--------------------------------------------------
											i = 0;
											numf = 0;
											for(int ti = 0; ti < 10; ti++) // clean buffers
												number[ti]=number2[ti]= 0;

											while(program[pc] != ',')  // 
											{
												if(program[pc] == '.' && program[pc+1] == '.')
												{
													numf = 1; // alternate for randomize
													pc += 2; // registrate that we read more
													i = 0; // update the index to write on the other buffer (number2)
												}
												if(isdigit(program[pc]) && !numf)
												{
													number[i] = program[pc];
													i++;
												}
												else if(isdigit(program[pc]) && numf)
												{
													number2[i] = program[pc];
													i++;
												}
												pc++;
											}
											if(numf)
											{
												interruption_vector[ic].duration_inst = rand()% (atoi(number2) - atoi(number)) + atoi(number); // INCRIVEL O RAND SÓ FUNCIONA QUANDO DEBUGA.
												printf("dependente: random - > %d\n", interruption_vector[ic].duration_inst);
												//interruption_vector[ic].duration_inst_a = atoi(number);
												//interruption_vector[ic].duration_inst_b = atoi(number2);
											}
											else
											{
												interruption_vector[ic].duration_inst = atoi(number);
												//interruption_vector[ic].duration_inst_a = atoi(number);
											}
											pc++;




											// Name		--------------------------------------------------
											i = 0;
											char name[64];
											for(int ti = 0; ti < 64; ti++) // clean buffers
												name[ti] = 0;
									

											while(program[pc] != ')') // 
											{
												if(program[pc] != ' ' && program[pc] != '\t') // caracteres nao permitidos para nomes
												{
													name[i] = program[pc];
													i++;
												}
												pc++;
											}

											for(int ti = 0; ti < 64; ti++) // clean buffers
											{
												// pre_interruption_vector[ic].interrupt_name[ti] = name[ti];
												// insere no vetor de interrupções
												 interruption_vector[ic].interrupt_name[ti] = name[ti];
												if(name[ti] == 0)
													break;
											}
											//ic++; // atualiza o ponteiro

											// TODO verifica se tem eventos dependentes
											//verify_depen(interruption_vector[ic].interrupt_name); // TODO : VERIFICAR SE RECURSIVIDADE NÃO ESTRAGA O FUNCIONAMENTO

											while(program[pc] != ')'){pc++;}
											if(program[pc] == ')');
											{
												if(program[pc+1] == ';');
												{
													//interruption_vector[ic].interrupt_name[0] = 'T'; // TODO: ARRUMAR ISSO DEPOIS
													//interruption_vector[ic].interrupt_name[1] = 0;
													//ic++;
												}
											}
										}
									
								}//
							}
						}
					}
				}
				pc++;
			}
			found = 0; // reset
		}
		pc++;
	}
}

void define_all_interruptions()
{
	// Primeiramente, vamos definir a quantidade de vezes que cada evento regular irá ocorrer.

	// ic continua armazenando o número de eventos regulares até o momento. Então vamos aproveitar a informação
	int interruptions_Count = ic;

	for(int i = 0; i < interruptions_Count; i++)
	{
		// aqui nós definimos a quanidade final que um evento irá ocorrer
		if (pre_interruption_vector[i].quantity_b > pre_interruption_vector[i].quantity_a)
			pre_interruption_vector[i].defined_quantity = rand()% (pre_interruption_vector[i].quantity_b - pre_interruption_vector[i].quantity_a) + pre_interruption_vector[i].quantity_a; // faz o rand e define a quantidade final
		else
			pre_interruption_vector[i].defined_quantity = pre_interruption_vector[i].quantity_a;
	}

	// Agora que temos o número de vezes que cada evento corrente
	// acontece, vamos analisar quais destes eventos têm dependencia,
	// e contabilizar suas dependencias, apra ENFIM determinar o
	// numero final de eventos que ocorrem na simulação TODA
	total_interruptions_count = 0; // reset the variable.
	for(int i = 0; i < interruptions_Count; i++)
	{
		// aqui nós definimos a quanidade final que um evento irá ocorrer, incluindo os dependentes
		total_interruptions_count += pre_interruption_vector[i].defined_quantity + ( pre_interruption_vector[i].defined_quantity * parse_file_get_dep_interruptions_count(pre_interruption_vector[i].interrupt_name)); // a conta é a seguinte (quantidade de vezes que o evento "E" ocorre * número de dependencias associado a este evento "D") = Total += de 0 até n_eventos (E*D) 
	}

	// Até aqui temos a quantidade máxima de eventos! Agora vamos preencher o vetor de eventos normal
	a_interrupt *interruption_vector_disordered = (a_interrupt*)malloc(sizeof(struct a_interrupt)*total_interruptions_count); // allocate the vector of interruptions

	// LAND SOME POINTERS
	for(int i = 0; i < total_interruptions_count; i++)
		interruption_vector_disordered[i].visited = false;

	int n_pre_interrupcoes_definidas = 0;//define a quantidade de interrupções pre-definidas ate então

	// pronto, agora vamos preencher o vetor de interrupções!
	for(int i = 0; i < interruptions_Count; i++) // essa interação garante que todos os elementos foram contabilizados
	{
		for(int j = 0; j < pre_interruption_vector[i].defined_quantity; j++) // essa interação garante que todos os elementos foram escolhidos por ordem
		{
			int def_start, def_duration;

			//define -
			// 1 inicio 
			if(pre_interruption_vector[i].start_inst_a == pre_interruption_vector[i].start_inst_b)
				def_start = pre_interruption_vector[i].start_inst_a;
			else
				def_start = rand()% (pre_interruption_vector[i].start_inst_b - pre_interruption_vector[i].start_inst_a) + pre_interruption_vector[i].start_inst_a;

			// 2 duração
			if(pre_interruption_vector[i].duration_inst_a == pre_interruption_vector[i].duration_inst_b)
				def_duration = pre_interruption_vector[i].duration_inst_a;
			else
				def_duration = rand()% (pre_interruption_vector[i].duration_inst_b - pre_interruption_vector[i].duration_inst_a) + pre_interruption_vector[i].duration_inst_a;

			// finalmente, esreve no vetor
			interruption_vector_disordered[n_pre_interrupcoes_definidas].start_inst = def_start;
			interruption_vector_disordered[n_pre_interrupcoes_definidas].duration_inst = def_duration;
			strcpy(interruption_vector_disordered[n_pre_interrupcoes_definidas].interrupt_name, pre_interruption_vector[i].interrupt_name);

			n_pre_interrupcoes_definidas++; // essa variavel vai ser utilizada futuramente para ordenar o vetor de interrupções
		}
	}

	if(pre_interruption_vector) // we will not use this vector anymore
	{
		free(pre_interruption_vector);
		pre_interruption_vector = 0;
	}

	// Agora vamos ordenar o vetor anterior!!
	// mas antes vamos alocar o vetor em que iremos armazenar de forma ordenada
	interruption_vector = (a_interrupt*)malloc(sizeof(a_interrupt)*total_interruptions_count); // allocate the vector of interruptions
	
	int indice_men_inst_ocorrencia = -1; // grava o indice no vetor de interrupções que acontece primeiro
	ic = 0; // instruction counter
	for(int i = 0; i < n_pre_interrupcoes_definidas; i++)
	{
		int menor_inst_de_ocorrencia = INT_MAX; // o que ocorre mais cedo é
		for(int j = 0; j < n_pre_interrupcoes_definidas; j++)
		{
			if(interruption_vector_disordered[j].start_inst < menor_inst_de_ocorrencia && interruption_vector_disordered[j].visited == false)
			{
				menor_inst_de_ocorrencia = interruption_vector_disordered[j].start_inst;
				indice_men_inst_ocorrencia = j;
			}
		}
		// insere na lsita e marca como inserido na lista ordenada
		interruption_vector_disordered[indice_men_inst_ocorrencia].visited = true;

		// copia para o vetor de interrupção ordenado
		interruption_vector[ic].start_inst = interruption_vector_disordered[indice_men_inst_ocorrencia].start_inst;
		interruption_vector[ic].duration_inst = interruption_vector_disordered[indice_men_inst_ocorrencia].duration_inst;
		strcpy(interruption_vector[ic].interrupt_name, interruption_vector_disordered[indice_men_inst_ocorrencia].interrupt_name);
		

		// EU AXO QEU AQUI DEVERIA INSERIR OS DEPENDENTES OBRIGATORIAMENTE
		verify_depen(interruption_vector[ic].interrupt_name);
		ic++;
	}

	if(interruption_vector_disordered)
	{
		free(interruption_vector_disordered);
		interruption_vector_disordered = 0;
	}

	printf("Debug, imprimindo vetor de interrupções ordenado\n");
	for(int i = 0; i < total_interruptions_count; i++)
	{
		printf("%d %d %s\n", interruption_vector[i].start_inst, interruption_vector[i].duration_inst, interruption_vector[i].interrupt_name);
	}
}

void fetch_dep_events()
{
	char *program;
	int pc; // program counter

	program = sim_file_buff;// load_file_to_cstring(file_name);

	pc = 0;
	while(pc <= sim_file_len)
	{
		// !------------ identifica comentário
		if(program[pc] == '/')
		{
			if(program[pc+1] == '/');
			{
				pc++;
				do{
					pc++;
				}while(program[pc] != '\n' && pc < sim_file_len);
			}
		}

		// !------------ identifica quantidade de instrucoes no programa normal (proc)
		if(program[pc] == 'd')
		{
			if(program[pc+1] == 'e')
			{
				if(program[pc+2] == 'p')
				{
					if(program[pc+3] == 'e')
					{
						if(program[pc+4] == 'v')
						{
							if(program[pc+5] == 'e')
							{
								if(program[pc+6] == 'n')
								{
									if(program[pc+7] == 't')
									{
										if(program[pc+8] == 's')
										{
											pc += 9;

											while(program[pc] != '"')
											{
												pc++;
											}pc++;


											int i = 0;
											char file_name[64];

											while(program[pc] != '"')
											{
												file_name[i] = program[pc];
												i++;

												pc++;
											}
											file_name[i] = 0; // null termination

											dep_int_file_buff = load_file_to_cstring(file_name);
											dep_int_file_len = file_size(file_name);
										}
									}
								}
							}
						}
					}//
				}
			}
		}
		pc++;
	}
}

void parse_file(char *file_name)
{
	char *program;
	int pc; // program counter
	
	// load the files and its sizes
	sim_file_buff = load_file_to_cstring(file_name);
	sim_file_len = file_size(file_name);
	fetch_dep_events(); // TODO criar uma flag que indica que existe o arquivo que informa os eventos dependentes
	/*
	dep_int_file_buff = load_file_to_cstring("e_dependentes.aed");
	dep_int_file_len = file_size("e_dependentes.aed");
	*/

	program = sim_file_buff;// load_file_to_cstring(file_name);

	//pre-scan the simulation router to get the count of interruptions
	//total_interruptions_count = parse_file_get_interruptions_count(file_name); // TODO DESCOMENTAR? 20140924
	parse_file_define_pre_interruptions(file_name); // pre_interruption_vector // OK

	
	// Agora precisamos definir realmente TODAS AS interrupções que ocorrem nesta MARIVILHOSA SIMULAÇÃO :)
	define_all_interruptions(); // este procedimento DEVERA definir todas as interr~upções que ocorrerão
	
	

	// automato para fazer parse do código ----------------------- A U T O M A T O ----------------------------
	pc = 0;

	
	while(pc <= sim_file_len)
	{
		// !------------ identifica comentário
		if(program[pc] == '/')
		{
			if(program[pc+1] == '/');
			{
				pc++;
				do{
					pc++;
				}while(program[pc] != '\n' && pc < sim_file_len);
			}
		}

		// !------------ identifica quantidade de instrucoes no programa normal (proc)
		if(program[pc] == 'p')
		{
			if(program[pc+1] == 'r')
			{
				if(program[pc+2] == 'o')
				{
					if(program[pc+3] == 'c')
					{
						
						pc += 4;
						int i = 0;
						char number[10];
						while(program[pc] != ';')
						{
							if(isdigit(program[pc]))
							{
								number[i] = program[pc];
								i++;
							}
							pc++;
						}
						// DEFINE NUMERO DE INSTRUCOES OPROGRAMA TEM
						proc_size = atoi(number);
						break;
					}
				}
			}
		}
		pc++;
	}
	
	// we finished using the buffer, FREE it.
	free(sim_file_buff);
	sim_file_buff = 0;
	program = 0;
	if(dep_int_file_buff)
	{
		free(dep_int_file_buff);
		dep_int_file_buff = 0;
	}
	
	// reset the global variables
	sim_duration = 0;

	//lets calculate the total  "sim_duration"
	int cnt; // contador auxiliar
	for(cnt = 0; cnt < total_interruptions_count; cnt++)
	{
		sim_duration += interruption_vector[cnt].duration_inst;
	}
	sim_duration += proc_size;
	
	// Now lets define the final vector (represents the final result and we draw it on the 2d graph)
	if(simulation_vector)
	{
		free(simulation_vector);
		simulation_vector = 0;
	}
	simulation_vector = (float*)malloc(sizeof(float)*sim_duration);

	for(cnt = 0; cnt < sim_duration; cnt++)
	{
		simulation_vector[cnt] = 0; // ZERO represents the normal process
	}


	// Before filling the vector, lets define a index for each event by its name, that way the graph doesn't assume a only incremental form.
	if(interruption_representation_vec)
	{
		free(interruption_representation_vec); // clean the representation vect, now that we finished using it.
		interruption_representation_vec = 0;
	}

	interruption_representation_vec = (a_interrupt_repre*)malloc(sizeof(a_interrupt_repre)*total_interruptions_count); // tought the number of interruptions isn't the number of interruptions type, it can cover the number of interruptions type.
	for(int i = 0; i < total_interruptions_count; i++)
	{
		// clean it before using (Ecistia um bug aqui.)
		// FIXED BUG:se não realizarmos esta iniciação
		// das strings para anular qualquer informacao
		// nelas, existe uma grande chance de os nomes
		// terem caracteres invalisods que impossibilitam
		// suas comparações posteriors. O problema piorava,
		// pois mais a frente era necessario a comparação
		// destes nomes para determinar a quantidade de
		// eventos distintos (por nome) que existiam na
		// simulação, logo, a contagem de eventos unicos tbm
		// era errônea, por consequencia os nomes não eram
		// mostrados sempres na GUI (grafico)
		interruption_representation_vec[i].name[0] = 0;
	}

	int rc = 0; //representation counter;
	printf("Total interruptions count: %d\n", total_interruptions_count);
	for(int i = 0; i < total_interruptions_count; i++) // PARA CADA ELEMENTO NO VENTOR DE NTERRUPÇOES
	{
		int found_ev_name_flag = 0; // auxiliar variable to check if found the event name on the representation vector.
		for(int j = 0; j < total_interruptions_count; j++)  // PARA CADA ELEMENTO NO VENTOR DE REPRESENTADOS
		{
			printf("%s --- %s\n", interruption_representation_vec[j].name, interruption_vector[i].interrupt_name);
			if(strcmp(interruption_representation_vec[j].name, interruption_vector[i].interrupt_name) == 0)
			{
				found_ev_name_flag = 1;
			}
		}
		if(!found_ev_name_flag) // the event have no representation, so lets add it to the vector of representations with a index
		{
			strcpy(interruption_representation_vec[rc].name, interruption_vector[i].interrupt_name);
			rc++;
			printf("EVENT COUNTER: %d\n", n_representations);
		}
	}
	n_representations = rc;

	// now lets fill the interruptions on the final vector
	for(cnt = 0; cnt < total_interruptions_count; cnt++)
	{
		int val = 0;
		while(strcmp(interruption_vector[cnt].interrupt_name, interruption_representation_vec[val].name) != 0) // determina o valor no grafico da interrupção atual, o seu numero, que representa o nome da interrupção no grafico
		{
			val++;
		}

		int start_inst = interruption_vector[cnt].start_inst;

		if(start_inst == -1) // o evento corrente é dependente do evento anterior, entao usa o start_inst do evento anterior
			start_inst = interruption_vector[cnt-1].start_inst;

		int momentun = 0; // Variavel que REAJUSTA tempo de duração de um evento (caso ele ocorra simultaneamente com outro evento, ele prorroga seu acontecimento até que seja possivel sua ocorrencia)
		for(int j = start_inst; j < (start_inst + interruption_vector[cnt].duration_inst + momentun); j++)
			if(simulation_vector[j] == 0)
				simulation_vector[j] = val+1;// porque o valor 0 é do processo corrente normal
			else
				momentun++; // reajustar o tempo de duração
	}
	
	int n_ev_dep = 0; // numero de eventos dependentes

	for(int i = 0; i < total_interruptions_count; i++)
		if(interruption_vector[i].start_inst == -1)
			n_ev_dep++;
	














	// Show some info to user (using FLTK)
	char view_message[1024];
	char temp[64];
	memset(view_message, 0, 1024);
	console->clear();
	//strcpy(view_message, (char*)ip_imput->value());
	strcat(view_message, "Simulador de eventos e interrupções");
	console->add(view_message);

	strcpy(view_message, "Desenvolvido por Aramis Hornung Moraes");
	strcat(view_message, " - Versão 0.1.3b");
	console->add(view_message);

	console->add("-----------------------------------------------");

	strcpy(view_message, "Numero interrupções: ");
	sprintf(temp,"%d",total_interruptions_count);
	strcat(view_message, temp);
	console->add(view_message);

	strcpy(view_message, "Numero total de instrucoes: ");
	sprintf(temp,"%d", (sim_duration-1));
	strcat(view_message, temp);
	console->add(view_message);

	strcpy(view_message, "Numero total de instrucoes dependentes: ");
	sprintf(temp,"%d", n_ev_dep);
	strcat(view_message, temp);
	console->add(view_message);

	console->add("-----------------------------------------------");

	for(int i = 0; i < total_interruptions_count; i++)
	{
		strcpy(view_message, "interrupção #");
		
		sprintf(temp,"%d", i);
		strcat(view_message, temp);

		strcat(view_message, " :");
		console->add(view_message);

		strcpy(view_message, "inicio ");
		sprintf(temp,"%d", interruption_vector[i].start_inst);
		strcat(view_message, temp);
		console->add(view_message);

		strcpy(view_message, "duração ");
		sprintf(temp,"%d", interruption_vector[i].duration_inst);
		strcat(view_message, temp);
		console->add(view_message);

		console->add("-");
	}
















	// free interruption vector
	free(interruption_vector);
	interruption_vector = 0;
}