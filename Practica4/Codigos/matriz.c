#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int i, j, k, aux, n = 3;
	//int matriz1[n][n], matriz2[n][n];
	int suma[n][n], resta[n][n], multiplica[n][n], trans[n][n], inversa[n][n];
	
	int matriz1[3][3] = {{2, 0, 1}, {3, 0, 0}, {5, 1, 1}};
	int matriz2[3][3] = {{1, 0, 1}, {1, 2, 1}, {1, 1, 0}};	

	// Suma
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			suma[i][j] = matriz1[i][j] + matriz2[i][j]; 			
		}
	}

	// Imprimir matriz
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
	
			printf(" %d/n",suma[i][j]);					
		}
	}

	// Resta
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			suma[i][j] = matriz1[i][j] + matriz2[i][j]; 			
		}
	}

	// Multiplicacion

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			aux = 0;
			for(k = 0; k < n; j++)
			{
				aux = matriz1[i][k] * matriz2[k][j] + aux;
			}
			multiplica[i][j] = aux;
		}
	}

	// Transpuesta

	

		return 0;
}
