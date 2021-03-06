//	Compilación: gcc 6.c -o 6
//	Ejecutar: 6 [RutaDestino]

#include <windows.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "funciones.h"
#define n 10
	
double **matriz1, **matriz2, **suma, **resta, **mul, **tran1, **tran2, **inv1, **inv2;
char* path;

DWORD WINAPI funcionHilo(LPVOID lpParam);

int main(int argc, char const *argv[])
{
	// Para medir el tiempo
	clock_t tiempo_inicio, tiempo_final;
	double segundos;

	tiempo_inicio = clock();
	int i;
	time_t t;
	srand((unsigned) time(&t));
	
	// Inicializa las matrices.
	matriz1 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		matriz1[i] = (double*)calloc(n,sizeof(double));

	matriz2 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		matriz2[i] = (double*)calloc(n,sizeof(double));	
	resta = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		resta[i] = (double*)calloc(n,sizeof(double));

	mul = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		mul[i] = (double*)calloc(n,sizeof(double));

	tran1 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		tran1[i] = (double*)calloc(n,sizeof(double));

	tran2 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		tran2[i] = (double*)calloc(n,sizeof(double));
	
	inv1 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		inv1[i] = (double*)calloc(n, sizeof(double));

	inv2 = (double**)calloc(n, sizeof(double*));
	for (i = 0; i < n; i++)
		inv2[i] = (double*)calloc(n, sizeof(double));

	if(argc != 2)
		exit(-1);
	
	path = (char*)calloc(2000, sizeof(char));
	strcpy(path, argv[1]);

	if(!CreateDirectory(path, NULL))
	{
			perror(path);
	        exit(-1);
	}

	// Llena matriz 1 y matriz 2
	llenar(matriz1, n);
	llenar(matriz2, n);

	printf("MATRIZ 1\n"); imprimir(matriz1, n);
	printf("MATRIZ 2\n"); imprimir(matriz2, n);

	// --------------- HILOS -----------------
	HANDLE manHilo[7];	// Manejador del Hilo
	DWORD idHilo[7]; // Identificador de los hilos
	
	for(i = 0; i < 5; i++)
	{
		// Lleva el control del hilo a realizar
		int *opcion = malloc(sizeof(int));
		*opcion = i;
		// Creacion del hilo [i]
		manHilo[i] = CreateThread(NULL, 0, funcionHilo, opcion, 0, &idHilo[i]);
	}

	// Espera la finalizacion del hilo
	for(i = 0; i < 5; i++)
	{
		WaitForSingleObject(manHilo[i], INFINITE);
		// Cierre del manejador del hilo creado
		CloseHandle(manHilo[i]);
	}
	
	// Ultimo HILO - Leer archivos
	int *opc = malloc(sizeof(int));
	*opc = 5;
	manHilo[5] = CreateThread(NULL, 0, funcionHilo, opc, 0, &idHilo[5]);
	WaitForSingleObject(manHilo[5], INFINITE);
	// Cierre del manejador del hilo creado
	CloseHandle(manHilo[5]);

    tiempo_final = clock();
    segundos = (double)(tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC;
    //Cálculo del tiempo de ejecución del programa
	printf("\n\nTiempo ejecucion:  %.4f s\n",  segundos);

	return 0;
}

DWORD WINAPI funcionHilo(LPVOID lpParam)
{
	int opc = *(int *)lpParam;
	int i;
	if(opc == 0)
	{
		//HILO SUMA
		suma = (double**)calloc(n,sizeof(double*));
		for (i = 0; i < n; i++)
			suma[i] = (double*)calloc(n,sizeof(double));

		printf("Soy el hilo calculando la SUMA\n"); sumar(matriz1, matriz2, suma, n); 
		crearArchivo(suma, path, "suma.txt");
	}
	else if(opc == 1)
	{
		//HILO RESTA
		printf("Soy el hilo calculando la RESTA\n"); restar(matriz1, matriz2, resta, n); 
		crearArchivo(resta, path, "resta.txt");
	}
	else if(opc == 2)
	{
		//HILO MULTIPLICACION
		printf("Soy el hilo calculando la MULTIPLICAR\n"); multiplicar(matriz1, matriz2, mul, n); 
		crearArchivo(mul, path, "multiplicacion.txt");
	}
	else if(opc == 3)
	{
		//HILO TRANSPUESTA
		printf("Soy el hilo calculando la TRANSPUESTA MATRIZ 1\n"); transpuesta(matriz1, tran1, n); 
		crearArchivo(tran1, path, "transpuesta_1.txt");
		
		printf("Soy el hilo calculando la TRANSPUESTA MATRIZ 2\n"); transpuesta(matriz2, tran2, n); 
		crearArchivo(tran2, path, "transpuesta_2.txt");
	}
	else if(opc == 4)
	{
		//HILO INVERSA
		printf("Soy el hilo calculando la INVERSA MATRIZ 1\n"); 
		//Revisamos si la maztriz tiene inversa
		if(inversa(matriz1, inv1, n) != 0) 
			crearArchivo(inv1, path, "inversa_1.txt");

		printf("Soy el hilo calculando la INVERSA MATRIZ 2\n"); 
		if(inversa(matriz2, inv2, n) != 0) 
			crearArchivo(inv2, path, "inversa_2.txt");
	}
	else
	{
		//HILO RESULTADO
		printf(" -------------------------------------\n");
		printf(" -- Soy el hilo leeyendo RESULTADOS --\n");
		printf(" -------------------------------------\n");

		printf("SUMA\n"); imprimirArchivo(path, "suma.txt");
		printf("\nRESTA\n"); imprimirArchivo(path, "resta.txt");
		printf("\nMULTIPLICAR\n"); imprimirArchivo(path, "multiplicacion.txt");
		printf("\nTRANSPUESTA MATRIZ 1\n"); imprimirArchivo(path, "transpuesta_1.txt");
		printf("\nTRANSPUESTA MATRIZ 2\n"); imprimirArchivo(path, "transpuesta_2.txt");
		printf("\nINVERSA MATRIZ 1\n"); imprimirArchivo(path, "inversa_1.txt");
		printf("\nINVERSA MATRIZ 2\n"); imprimirArchivo(path, "inversa_2.txt");	
	}
	return 0;
}