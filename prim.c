#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//vertice escolhido para iniciar a arvore
#define vertice 2

int grau(int **grafo, int v, int n){
	int i, grau = 0;
	for(i = 0; i < n; i++)
		if(grafo[v][i] != 0) grau++;
	return grau;
}

void prim(int **grafo, int orig, int n, int *pai){
	int i, j, dest, primeiro, menor;
	for(i = 0; i < n; i++)
		pai[i] = -1;
	pai[orig] = orig;
	while(1){
		primeiro = 1;
		//percorre todos os vertices
		for(i = 0; i < n; i++){
			//achou vertices ja visitados
			if(pai[i] != -1){
				//percorre os vizinhos do vertice visitado
				for(j = 1; j < grau(grafo, orig, n); j++){
					if(pai[j] == -1){
						if(primeiro){
							menor = grafo[i][j];
							orig = i;
							dest = j;
							primeiro = 0;
						}else{
							if(menor > grafo[i][j]){
								menor = grafo[i][j];
								orig = i;
								dest = j;
							}
						}
					}
				}
			}
		}
		if(primeiro == 1) break;
		printf("dest: %d, orig: %d\n", dest, orig);
		pai[dest] = orig;
	}
}

int **monta_matriz(FILE *arquivo, int *n) {
	char buffer[256];
	char *token;
	int i, j;
	fgets(buffer, sizeof(buffer), arquivo);
	token = strtok(buffer, " \t");
	*n = atoi(token);
	printf("numero de vertices %i\n", *n); //debug
	int **matriz_a = (int**)malloc(* n *sizeof(int*));
	for(i = 0; i < *n; i++) matriz_a[i] = (int*)malloc(*n * sizeof(int));
	i = 0;
	while(fgets(buffer, sizeof(buffer), arquivo)){// i = 0 to n-1
		//printf("buffer: %s", buffer); //debug
		token = strtok(buffer, " \t");
		for(j = i; j < *n; j++){
			if(i == j){
				matriz_a[i][j] = 0;
				printf("linha %i, coluna %i: %i\n", i, j, matriz_a[i][j]);// debug
				continue;
			}else{
				matriz_a[i][j] = atoi(token);
				matriz_a[j][i] = atoi(token);
				token = strtok(NULL, " \t");
			}
			printf("linha %i, coluna %i: %i\n", i, j, matriz_a[i][j]);// debug
		}
		i++;
	}
	return matriz_a;
}

int main(int argc, char **argv){
	// trata trollagem
	if(argc > 2 || argc <= 1){
		printf("modo de uso: prim <ArquivoMatrizAdjacencia>.txt\n");
		return 0;
	}
	// problema de buffer
	if(strstr(argv[1], ".txt") == NULL){
		printf("adicione a extensao .txt no fim de \"%s\"\n", argv[1]);
		return 0;
	}

	FILE *arquivo;

	// arquivo nao encontrado
	if((arquivo = fopen(argv[1], "r")) == NULL){
		printf("erro ao abrir arquivo %s\n", argv[1]);
		return 0;
	}
	int n; //quantidade de vertices
	int **matriz_a; //matriz de adjacencia(grafo)
	matriz_a = monta_matriz(arquivo, &n); // monta grafo
	int arvore[n]; //arvore geradora minima
	// aplica algoritmo de prim
	prim(matriz_a, 0, n, arvore);
	//printf("%i \n", arvore[0]);//debug
	//printf("%i \n", arvore[1]);//debug
	//printf("%i \n", arvore[2]);//debug
	//printf("%i \n", arvore[3]);//debug
	

	free(matriz_a);
	fclose(arquivo);
	return 1;
}
