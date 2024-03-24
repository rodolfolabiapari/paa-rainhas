
#include <stdio.h>
#include <stdlib.h>

# define global_tamanho_matriz 8


typedef struct Struct_Tabuleiro {
	int  colunas           [global_tamanho_matriz];
	char esquerda_diagonal [global_tamanho_matriz];
	char direita_diagonal  [global_tamanho_matriz];
	int  premio;
} Tabuleiro;

int Le_Premios(char * diretorio, int ** premios){
	FILE * file;
	int i = 0, maior_premio = -1;
	int premio_lido, quantidade_linhas;
	int quantidade_celulas = 0;
			
	// Abre o arquivo de prêmios em forma de leitura
	file = fopen(diretorio, "r");
	
	// Verifica se a abertura foi feita com sucesso
	if (file != NULL) {
		
		//lê a primeira informação (quantas linhas existem)
		fscanf(file, "%d", &quantidade_linhas);
		printf("\n[INFO] Quantidade de colunas do tabuleiro = %d.\n", quantidade_linhas);
		
		// Se o valor de tamanho da matriz for um valor válido
		if (quantidade_linhas > 0) {
			
			quantidade_celulas = quantidade_linhas * quantidade_linhas;
			
			// Aloca a matriz de prêmios
			* premios = calloc(quantidade_celulas, sizeof(int));

			// Começa a coleta dos prêmios
			fscanf(file, "%d", &premio_lido);

			// Salvas os prêmios e coleta o próximo
			while(i < quantidade_celulas) {

				// Imprime na tela o prêmio lido
				printf("\n[INFO] Lendo premio [%d,%d] = %d.", i / quantidade_linhas + 1, i % quantidade_linhas + 1, premio_lido);
				fflush(stdout);
				
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
	
	return maior_premio;
}




char validaPosicao(int y, int vetorRainha[], int x) {
	int i;
	for (i = 0; i < x; i++) {
		//Verifica se já existe um valor igual a y já no vetor
		if (vetorRainha[i] == y) {
			return 0;
		}

		//Verifica se o valor a ser posicionado na matriz infringe a diagonal do valor que está na posição vetor[i]
		if (abs(vetorRainha[i] - y ) == abs(i - x)) {
			return 0;
		}
	}
	
	//Caso já tenha percorrido todo o vetor e nenhuma dessas restrições foi negada, 
		// retorna que a posição é válida para a alocação da nova rainha
	return 1;	
}



void NRainhas(int vetorRainha [], int eixoY, int eixoX) {

	//Define a posição do eixo y atual para que seja possível fazer uma volta circular no tabuleiro
	int Y_atual = eixoY;

	int indice_atual_Y_operacoes;

	//Realiza a movimentação pelo eixo y do tabuleiro a procura de alguma solução válida.
			//o valor i será utilizado com i % tamTabuleiro para que seja circular sua trajetória
	while (Y_atual < 8 + eixoY) {

		//Verifica se chegou no ótimo
		if (vetorRainha[8 - 1] != -1) {
			return ;
		}

		//Realiza o cálculo do indice atual
		indice_atual_Y_operacoes = Y_atual % 8;

		//Verifica se a posição atual é valida. Lembrando que indiceAtual representa o eixoY onde 
				//será validado que 'se colocar uma rainha aqui, ela terá problemas'?
		if (validaPosicao(indice_atual_Y_operacoes, vetorRainha, eixoX)) {
			//Se a posição for válida adiciona o eixoY ao vetor indicando que naquela posição
					//existe uma rainha
			vetorRainha[eixoX] = indice_atual_Y_operacoes;

			//Verifica se a posição adicionada foi a última do vetor
					//Se sim não realiza mais nennhum cálculo e retorna todas suas chamadas de recursão.
			if (vetorRainha[8 - 1] == -1) {

				//verifica se o indiceAtual ultrapassou o limite do tabuleiro
				if (indice_atual_Y_operacoes == 8 - 1)
				{
					//Se sim altera o indice para 0
					NRainhas(vetorRainha, 0, eixoX + 1);
				}
				else 
				{
					//Caso contário passa pro próximo índice
					NRainhas(vetorRainha, indice_atual_Y_operacoes+  1, eixoX + 1);
				}
			} 
		}

		//Caso alguma recursão retorne por caminho inválido, incrementa o i para que procure na casa seguinte uma nova combinação
		Y_atual++;
	}

	//Após verificar todas as casas do eixo, se não tiver chegado no ótimo significa que
			//toda aquele eixo é invalido. Assim, ele apaga a última alteração feita e termina a recurção deste eixo
	if (vetorRainha[8 - 1] == -1) {
		vetorRainha[eixoX - 1] = -1;
	}
}



int main(int argc, char** argv) {
	int i, vetorRainha [8];
	
	for (i = 0; i < 8; i++) {
		vetorRainha[i] = -1;
	}
	
	NRainhas(vetorRainha, 0, 0);

	return (EXIT_SUCCESS);
}

