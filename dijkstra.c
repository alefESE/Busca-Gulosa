#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int minDistance(int dist[], int sptSet[], int n)
{
   // Inicia distancia minima
   int min = INT_MAX, min_index;
  
   for (int v = 0; v < n; v++)
     if (sptSet[v] == 0 && dist[v] <= min)
         min = dist[v], min_index = v;
  
   return min_index;
}

void printPath(int pai[], int j)
{
    if (pai[j]==-1)
        return;
 
    printPath(pai, pai[j]);
 
    printf("%d ", j);
}

int printSolution(int dist[], int n, int pai[], int src)
{
   printf("Vertice\t  Distancia\tCaminho");
   for (int i = 0; i < n; i++){
      printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
		printPath(pai, i);
	}
	printf("\n");
}

void dijkstra(int **grafo, int src, int n)
{
    int dist[n];//o array de saida tem a menor distancia de src ate i
  
    int sptSet[n];//conjunto de vertices incluidos na arvore
	int pai[n];
  
     //Inicia com infinito
     for (int i = 0; i < n; i++){
		pai[i] = -1;
        dist[i] = INT_MAX;
		sptSet[i] = 0;
	}
  
     // Distancia de src ate ele mesmo eh zero
     dist[src] = 0;
  
     // Encontra o menor caminho de todos os vertices
     for (int count = 0; count < n-1; count++)
     {
       // pega a menor distancia no conjunto dist
       int u = minDistance(dist, sptSet, n);
  
       // adiciona vertice no conjunto
       sptSet[u] = 1;
  
       //atualiza o valor dos vertices adjacentes
       for (int v = 0; v < n; v++)
         if (!sptSet[v] && grafo[u][v] && 
			dist[u]+grafo[u][v] < dist[v]){
			pai[v] = u;
            dist[v] = dist[u] + grafo[u][v];
		}
     }
  
     // printta array
     printSolution(dist, n, pai, src);
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
	// aplica algoritmo de dijkstra
	dijkstra(matriz_a, 0, n);
	//printf("%i \n", arvore[0]);//debug
	//printf("%i \n", arvore[1]);//debug
	//printf("%i \n", arvore[2]);//debug
	//printf("%i \n", arvore[3]);//debug
	

	free(matriz_a);
	fclose(arquivo);
	return 1;
}
