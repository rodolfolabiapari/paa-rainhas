#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// variável global sobre a quantidade de colunas/linhas da matriz
int global_tamanho_matriz = -1;
// variável que ativa impressoes na tela
char imprime_tela_ativado = 0;

// Variáveis de tempo para cálculo do intervalo de tempo de execução
time_t endwait, start;


/*
 * Estrutura que representa um tabuleiro.
 * 
 * Um ponteiro para um vetor de onde representa cada posicao da rainha na coluna
 * O valor de prêmrio do tabuleiro no estado atual
 */
typedef struct Struct_Tabuleiro {
	int * colunas;
	int   premio;
} Tabuleiro;


/*
 * Estrutura de representação de Fila
 * 
 * Esta fila é utilizada para organizar a ordem de seleção das rainhas
 */
typedef struct Struct_Fila {
	int           lugar; // Posição da rainha
	struct Struct_Fila * proximo;
} Fila;



/*
 * Procedimento que verifica se a fila passada por parâmetro está vazia.
 * 
 * Retorna valores boleanos
 */
char Fila_Esta_Vazia(Fila * f) {
	if (f == NULL) {
		return 1;
	} else
		return 0;
}



/*
 * Procedimento que adiciona um novo valor à fila
 */
void Enfilera(Fila ** f, int l) {
	Fila * atual, * novo;

	// Cria um novo nó e adiciona informações
	novo = calloc(1, sizeof(Fila));
	novo->lugar = l;
	novo->proximo = NULL;


	// Se a fila estiver vazia, coloca na cabeça
	if (Fila_Esta_Vazia(*f)) {

		* f = novo;

	} else {
		// Caso contrário, coloca na calda

		atual = * f;

		while(atual->proximo != NULL) {
			atual = atual->proximo;
		}

		atual->proximo = novo;
	}
}



/*
 * Procedimento que realiza a retirada de um elemento da fila seguindo sua natureza
 */
int Desenfilera(Fila ** f) {
	Fila * retirar, * fila = * f;
	int posicao;

	// Se a fila estiver vazia, informa erro e termina o programa
	if (Fila_Esta_Vazia(*f)) {
		printf("\n\n[ERRO] Impossível retirar de uma fila vazia.");
		exit(1);
		
	} else {
		// caso contrário, retira o primeiro elemento.
		posicao = fila->lugar;
		retirar = fila;

		fila = fila->proximo;

		free(retirar);

		*f = fila;

		return posicao;
	}
}



/*
 * Procedimento que realiza o processo de retirar todos os elementos da fila
 */
void Desaloca_Fila(Fila * f) {
	Fila * retirar;

	// Retira todos os elementos da fila
	while (f != NULL) {
		retirar = f;

		f = f->proximo;

		free (retirar);
	}

}



/*
 * Procedimento para impressão da Fila na tela
 */
void Imprime_Fila(Fila * f) {
	Fila * atual = f;

	if (imprime_tela_ativado) {

		printf("\n\n\t\t");
		while (atual != NULL) {
			printf("%d -> ", atual->lugar);
			atual = atual->proximo;
		}
		printf("NULL");

		fflush(stdout);
	}
}



/*
 * Procedimento responsável por gerar uma fila de posições de rainhas em ordem aleatória.
 *
 * Primeiro cria-se um vetor com os valores de 1..n ordenado simbolizando a ordem das rainhas.
 * Em seguida, é gerado dois valores inteiros posicao1 e posicao2 de forma aleatória e estes 
 *		são utilizados para a comutação dos valores situados em vetor[posicao1] e vetor[posicao2]
 * No final de algumas iterações, estes respectivos valores já desordenados são copiados para 
 *		a fila para que o algoritmo use.
 */
Fila * Cria_Fila_Aleatoria() {
	Fila * f, * cabeca;

	int vetor_valores_fila [global_tamanho_matriz],
			i,
			permutacoes = global_tamanho_matriz * 10,
			posicao1, posicao2, temp;

	// Cria-se um vetor com valores ordenados de 1..n_rainhas
	for (i = 0; i < global_tamanho_matriz; i++) {
		vetor_valores_fila[i] = i;
	}

	// Realiza várias permutações dos valores deste vetor de acordo com os dois índices gerados
		// Realiza-se global_tamanho_matriz * 10 permutações
	i = 0;
	while (i < permutacoes) {
		// Gera primeiro índice
		posicao1 = rand() % global_tamanho_matriz;

		// Gera segundo índice
		posicao2 = rand() % global_tamanho_matriz;

		// Realiza a comutação dos valores dos respectivos índices
		temp = vetor_valores_fila[posicao1];
		vetor_valores_fila[posicao1] = vetor_valores_fila[posicao2];
		vetor_valores_fila[posicao2] = temp;

		i++;
	}

	// Aloca a fila e adiciona os valores após realizar a desordem
	cabeca = calloc(1, sizeof(Fila));

	f = cabeca;

	// Copia os valores do vetor para a fila
	i = 0;
	while (i < global_tamanho_matriz - 1) {

		f->lugar = vetor_valores_fila[i];

		f->proximo = calloc(1, sizeof(Fila));

		f = f->proximo;

		i++;
	}

	f->lugar = vetor_valores_fila[i];

	f->proximo = NULL;

	// Retorna a fila desordenada
	return cabeca;
}



/*
 * Procedimento para retorndo do de prêmio específico de uma célula
 */
int Retorna_Premio(int * premios, int linha, int coluna) {

	// Acessa o vetor como se fosse uma matriz comum nxn.
	return premios[linha * global_tamanho_matriz + coluna];
}



/*
 * Procedimento para realizar a leitura dos arquivo prêmio
 * 
 * Este também armazena o maior premio lido e o retorna.
 */
int Le_Premios(char * diretorio, int ** premios){
	FILE * file;
	int i = 0, maior_premio = -1;
	int premio_lido;
	int quantidade_celulas = 0;

	// Abre o arquivo de prêmios em forma de leitura
	file = fopen(diretorio, "r");

	// Verifica se a abertura foi feita com sucesso
	if (file != NULL) {

		//lê a primeira informação (quantas linhas existem)
		fscanf(file, "%d", &global_tamanho_matriz);
		if (imprime_tela_ativado)
			printf("\n[INFO] Quantidade de colunas do tabuleiro = %d.\n", global_tamanho_matriz);

		// Se o valor de tamanho da matriz for um valor válido
		if (global_tamanho_matriz > 0) {

			quantidade_celulas = global_tamanho_matriz * global_tamanho_matriz;

			// Aloca a matriz de prêmios
			* premios = calloc(quantidade_celulas, sizeof(int));

			// Começa a coleta dos prêmios
			fscanf(file, "%d", &premio_lido);

			// Salvas os prêmios e coleta o próximo
			while(i < quantidade_celulas) {

				if (imprime_tela_ativado) {
					// Imprime na tela o prêmio lido
					printf("\n[INFO] Lendo premio [%d,%d] = %d.", i / global_tamanho_matriz + 1, i % global_tamanho_matriz + 1, premio_lido);
					fflush(stdout);
				}

				// Armazena o maior prêmio encontrado
				if (premio_lido > maior_premio) {
					maior_premio = premio_lido;
				}

				// Salva no vetor
				(*premios)[i++] = premio_lido;

				// Lê o próximo prêmio
				fscanf(file, "%d", &premio_lido);
			}
		} else {
			// Informa Erro
			printf("\n[ERRO] Quantidade de rainhas insuficiente!\n\n");
			exit(-1);
		}

		// Fecho o arquivo
		fclose(file);
	}
	else {
		// Informa Erro
		printf("\n[ERRO] Falha na leitura do arquivo de configuração!\n\n");
		exit(-1);
	}

	// Retorna o maior prêmio encontrado
	return maior_premio;
}



/*
 * Como utiliza-se uma estrutura fila com todas as opções e sem repetição, não 
 *		existe a possibilidade de duas rainhas ficarem numa mesma linha 
 *		vertical, horizontal (isso pois os valores não se repetem).
 *
 * Com isso, basta verificar se as diagonais estão conflitando.
 */
char Posicao_Eh_Valida(Tabuleiro t, int col, int pos) {
	int i = 0;
	char esquerda_diagonal, direita_diagonal;

	// Inicializa dizendo que as diagonais não estão ocupadas
	esquerda_diagonal = direita_diagonal = 1;

	// Da posição da rainha até a coluna 0, faça:
	i = col;
	while (i > 0) {

		// Verifica:
		//		Diagonal esquerda-direita
		if (t.colunas[col - i]  == pos + i) {
			esquerda_diagonal = 0;

		// Verifica:
		//		Diagonal direita-esquerda
		} else
			if (t.colunas[col - i] == pos - i) {
			direita_diagonal = 0;
		}

		// Verifica validade
		// Caso alguma diagonal já esteja ocupada, aborta o procedimento
			// informando que esta posição é inválida
		if (esquerda_diagonal == 0 || direita_diagonal == 0) {
			return 0;
		}

		i--;
	}

	// Verifica validade das diagonais
	if (esquerda_diagonal == 1 && direita_diagonal == 1)
		return 1;
	else
		return 0;
}



/*
 * Procedimento que copia os dados de um tabuleiro para outro.
 * 
 * Este procedimento é utilizando quando encontra-se um novo valor de prêmio maior
 *		que o atual e assim, realiza-se a substituição do tabuleiro antigo pelo novo
 *		encontrado.
 */
void Copia_Novo_Tabuleiro(Tabuleiro tabuleiro_atual, Tabuleiro * tabuleiro_maior) {
	int i;

	tabuleiro_maior->premio           = tabuleiro_atual.premio;

	for (i = 0; i < global_tamanho_matriz; i++) {
		tabuleiro_maior->colunas[i]   = tabuleiro_atual.colunas[i];
	}
}



/*
 * Procedimento que adiciona uma nova rainha no tabuleiro já calculando o prêmio desta.
 * 
 * Este procedimento não precisa verificar a validade da posição já que este já foi calculado
 *		quando a rainha foi selecionada.
 */
void Adiciona_Rainha_Tabuleiro_Calculando_Premio(Tabuleiro * t, int posicao,  int r, int * premios) {

	t->premio += Retorna_Premio(premios, r, posicao);
	t->colunas[posicao] = r;
}



/*
 * Procedimento que retira a rainha do tabuleiro calculando o novo prêmio
 */
void Retira_Rainha_Tabuleiro_Calculando_Premio(Tabuleiro * t, int posicao, int r, int * premios) {
	t->premio -= Retorna_Premio(premios, r, posicao);
	t->colunas[posicao] = -1;
}



/*
 * Procedimento que inicializa um novo tabuleiro
 */
Tabuleiro * Cria_Tabuleiro(){
	int i;
	Tabuleiro * t;

	t = calloc(1, sizeof(Tabuleiro));
	t->colunas = calloc(global_tamanho_matriz, sizeof(int));


	for (i = 0; i < global_tamanho_matriz; i++)
		t->colunas[i] = -1;

	return t;
}



/*
 * Procedimento que imprime o tabuleiro junto com os prêmios
 */
void Imprime_Tabuleiro (Tabuleiro t, Tabuleiro maior, int * premios) {
	int i, j;

	if (imprime_tela_ativado) {
		
		printf("\n");

		for (i = 0; i < global_tamanho_matriz; i++) {
				printf("\n\t");
			for (j = 0; j < global_tamanho_matriz * 2; j++) {
				if ( j < global_tamanho_matriz) {
					if (i == t.colunas[j])
						printf("%3d ", i);
					else
						printf(" -1 ");
				}
				else {
					if (j == global_tamanho_matriz)
						printf("\t\t");
					printf("%3d ", premios[i * global_tamanho_matriz + j - global_tamanho_matriz]);
				}

			}

			fflush(stdout);
		}

		printf("\tPremio_Atual: %d;\tMaior_Premio: %d.", t.premio, maior.premio);

		fflush(stdout);
	}
}


/*
 * Procedimento que imprime o tabuleiro, junto com os prêmios informando fim da execução
 */
void Imprime_Tabuleiro_Final (Tabuleiro maior, int * premios) {
	int i, j;
	FILE * saida;

	if (imprime_tela_ativado) {
		
		printf("\n\n[INFO] Resultado Final do Processamento:");

		for (i = 0; i < global_tamanho_matriz; i++) {
				printf("\n\t");
			for (j = 0; j < global_tamanho_matriz * 2; j++) {
				if ( j < global_tamanho_matriz) {
					if (i == maior.colunas[j])
						printf("%3d ", i);
					else
						printf(" -1 ");
				}
				else {
					if (j == global_tamanho_matriz)
						printf("\t\t");
					printf("%3d ", premios[i * global_tamanho_matriz + j - global_tamanho_matriz]);
				}

			}

			fflush(stdout);
		}

		printf("\tPremio: %d.",maior.premio);
		printf("\n[INFO] Resultado Final do Processamento.\n");
	} else {
		saida = fopen("algoritmo2.txt", "a");
		
		if (saida) {
			fprintf(saida, "%d\n", maior.premio);
		}
		
		fclose(saida);
	}

	fflush(stdout);
}



/*
 * Procedimento que libera memória
 */
void Desaloca_Tabuleiro (Tabuleiro * f) {
	free(f);
}



/*
 * Procedimento que libera memória
 */
void Desaloca_Premios (int * premios) {
	free(premios);
}



/*
 * Método de branch and bound desenvolvido
 */
Tabuleiro * n_Rainhas_Prize(int coluna_atual, Tabuleiro * tabuleiro_atual, Tabuleiro * tabuleiro_maior, Fila ** posicoes_restantes, int * premios) {
	int iteracoes = 0, linha_atual_temp;
	float fator_continua_recursao;

	// Recebe o tempo atual.
	start = time(NULL);
	
	// Verifica se o tempo excedeu o liminte estabelecido.
	if (start > endwait) {
		// Se sim, cancela totalmente a continuação da recursão
		return tabuleiro_atual;
	}
	
	// Verifica se a fila esta vazia
	if (Fila_Esta_Vazia(*posicoes_restantes)) {
		// TODO verifica se aqui é acessado
	} else {

		// Varre a fila com os valores que sobraram
			// A cada recursão, um item é retirado
		
		// Caso tenha percorrido todas as rainhas deste contexto de recursão,
			// o while será impedido de ser executando forçando a realizar o 
			// retorno à um nível acima de recursao
		while (iteracoes < global_tamanho_matriz - coluna_atual) {

			Imprime_Fila(* posicoes_restantes);
			Imprime_Tabuleiro(*tabuleiro_atual, *tabuleiro_maior, premios);

			// Retira um item da fila e salva numa variável local
			linha_atual_temp = Desenfilera( posicoes_restantes );

			Imprime_Fila(* posicoes_restantes);
			if (imprime_tela_ativado) {
				printf("\t\tBuffer_Atual: %d", linha_atual_temp);
				fflush(stdout);
			}


			// Testa a validade da rainha retirada no momento
				// Se for posição válida realizará o processamento desta
				// Caso contrário, ela será posta no final da fila=
			if(Posicao_Eh_Valida(*tabuleiro_atual, coluna_atual, linha_atual_temp)) {

				// Adiciona a rainha no tabuleiro calculando o prêmio com sua inclusão
				Adiciona_Rainha_Tabuleiro_Calculando_Premio(tabuleiro_atual, coluna_atual, linha_atual_temp, premios);

				Imprime_Tabuleiro(*tabuleiro_atual, *tabuleiro_maior, premios);


				// Se a fila estiver vazia, significa que acabou de ser gerado uma solução
					// válida.
				// Assim, será verificado se o prêmio é melhor que o atual.
				if (Fila_Esta_Vazia(* posicoes_restantes)) {

					if (tabuleiro_atual->premio > tabuleiro_maior->premio) {

						if (imprime_tela_ativado)
							printf("\n\n[INFO] Novo Recorde Encontrado!");
						Copia_Novo_Tabuleiro(*tabuleiro_atual, tabuleiro_maior);

						Imprime_Tabuleiro(*tabuleiro_atual, *tabuleiro_maior, premios);
					}

					
					Imprime_Fila(*posicoes_restantes);

					// Após chegar na folha, a recursão é revertida até que encontre 
						// uma próxima solução pra explorar.
					Enfilera(posicoes_restantes, linha_atual_temp);

					Imprime_Fila(*posicoes_restantes);

					// Como mencionado, a raínha é posta novamente na fila para a procura
						// de novas soluções
					Retira_Rainha_Tabuleiro_Calculando_Premio(tabuleiro_atual, coluna_atual, linha_atual_temp, premios);


					return tabuleiro_atual;

					
				// Se não for a última rainha, então realiza as análises de bound
				} else { 

					if (imprime_tela_ativado)
						printf("\n%10f\n", ((float) coluna_atual) / global_tamanho_matriz);

					
					//Considerações
						// - Ler o relatório que acompanha o código.
						// - Enquanto o maior premio encontrado ainda for 0, então os filtros não
								// serão aplicados
					
					
					// Filtro 1
					if (tabuleiro_maior->premio != 0  &&  ((float) coluna_atual) / global_tamanho_matriz >= 0.60  && ((float) coluna_atual) / global_tamanho_matriz < 0.7) {

						fator_continua_recursao =  (float) tabuleiro_atual->premio / tabuleiro_maior->premio;

						if (imprime_tela_ativado) {
							printf("\n[INFO] Premio Atual: %d;\tMaior Premio: %d\tFator Continua Recursão: %.7f", tabuleiro_atual->premio, tabuleiro_maior->premio, fator_continua_recursao);
							fflush(stdout);
						}

						//if (fator_continua_recursao > 0.50)
						if (fator_continua_recursao > 0.6)
							n_Rainhas_Prize(coluna_atual + 1, tabuleiro_atual, tabuleiro_maior, posicoes_restantes, premios);


					// Filtro 2
						// TODO fazer um arquivo pra ler esses valores seria bem legal
					} else if (tabuleiro_maior->premio != 0  &&  ((float) coluna_atual) / global_tamanho_matriz >= 0.7 &&  ((float) coluna_atual) / global_tamanho_matriz < 0.8) {

						fator_continua_recursao =  (float) tabuleiro_atual->premio / tabuleiro_maior->premio;

						if (imprime_tela_ativado) {
							printf("\n[INFO] Premio Atual: %d;\tMaior Premio: %d\tFator Continua Recursão: %.7f", tabuleiro_atual->premio, tabuleiro_maior->premio, fator_continua_recursao);
							fflush(stdout);
						} 

						//if (fator_continua_recursao > 0.60)
						if (fator_continua_recursao > 0.7)
							n_Rainhas_Prize(coluna_atual + 1, tabuleiro_atual, tabuleiro_maior, posicoes_restantes, premios);


					// Filtro 3
					} else if (tabuleiro_maior->premio != 0  &&  ((float) coluna_atual) / global_tamanho_matriz >= 0.8) {

						fator_continua_recursao =  (float) tabuleiro_atual->premio / tabuleiro_maior->premio;

						if (imprime_tela_ativado) {
							printf("\n[INFO] Premio Atual: %d;\tMaior Premio: %d\tFator Continua Recursao: %.7f", tabuleiro_atual->premio, tabuleiro_maior->premio, fator_continua_recursao);
							fflush(stdout);
						}

						//if (fator_continua_recursao > 0.82)
						if (fator_continua_recursao > 0.8)
							n_Rainhas_Prize(coluna_atual + 1, tabuleiro_atual, tabuleiro_maior, posicoes_restantes, premios);


					// Enquanto o maior premio encontrado ainda for 0, então os filtros não
						// serão aplicados e a recursão segue normalmente usando força
						// bruta
					} else {
						n_Rainhas_Prize(coluna_atual + 1, tabuleiro_atual, tabuleiro_maior, posicoes_restantes, premios);
					}

					// Ao retornar das recursões, a rainha atual será retirada do tabuleiro e colocada na fila novamente
						// e será procurado a próxima solução disponível
					
					Enfilera(posicoes_restantes, linha_atual_temp);

					Retira_Rainha_Tabuleiro_Calculando_Premio(tabuleiro_atual, coluna_atual, linha_atual_temp, premios);

					Imprime_Fila(*posicoes_restantes);

					iteracoes++;
				} // if

				
			// Se a posição escolhida não for válida
			} else { 

				if(imprime_tela_ativado) {
					printf("\n[INFO] Posição Inválida. Retornando o valor %d à fila", linha_atual_temp);
					fflush(stdout);
				}

				// Readiciona-la no final da fila
				Enfilera(posicoes_restantes, linha_atual_temp);

				Imprime_Fila(*posicoes_restantes);

				// Passa pra próxima rainha
				iteracoes++;

			} // if

		} // While

		
		// Após ter percorrido todos as soluções deste nível e seus subníveis
			// é retornado um nível para continuar a busca.

		if(imprime_tela_ativado)
			printf("\n[INFO] Saindo do nível %d", coluna_atual);

		return tabuleiro_atual;
	}

	// Fim de execução de forma natural.
	
	return tabuleiro_atual;
}



// EXISTEM TODOs
int main(int argc, char** argv) {
	int maior_premio, * premios;
	char * diretorio;
	Tabuleiro * tabuleiro_atual, * tabuleiro_maximo_encontrado;
	Fila * fila_rainhas;
    time_t seconds;
	
	if (argc != 4) {
		diretorio = argv[1];
		srand(atoi(argv[2]));
		seconds = atoi(argv[3]);
		imprime_tela_ativado = atoi(argv[4]);	
		
	} else {
		exit(-1);
	}

	maior_premio = Le_Premios(diretorio, &premios);

	tabuleiro_atual = Cria_Tabuleiro();
	tabuleiro_maximo_encontrado = Cria_Tabuleiro();

	fila_rainhas = Cria_Fila_Aleatoria();

	start = time(NULL);
	
    endwait = start + seconds;

	n_Rainhas_Prize(0, tabuleiro_atual, tabuleiro_maximo_encontrado, &fila_rainhas, premios);


	Imprime_Tabuleiro_Final(*tabuleiro_maximo_encontrado, premios);


	Desaloca_Fila(fila_rainhas);

	Desaloca_Premios(premios);

	Desaloca_Tabuleiro(tabuleiro_maximo_encontrado);
	Desaloca_Tabuleiro(tabuleiro_atual);
	
	return (EXIT_SUCCESS);
}
