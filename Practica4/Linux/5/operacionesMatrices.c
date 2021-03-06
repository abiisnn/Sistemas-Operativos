//	Compilación:
//	gcc tiempo.c -c
//	gcc operacionesMatrices.c tiempo.o -o o

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "tiempo.h"

// Declaracion de funciones
char* leerDirectorio();
int potencia(int base, int pot);
void imprimir(double **m, int n);
void llenar(double **m, int n);
void sumar(double **m1, double **m2, double **resultado, int n);
void restar(double **m1, double **m2, double **resultado, int n);
void multiplicar(double **m1, double **m2, double **resultado, int n);
void transpuesta(double **m, double **resultado, int n);
int inversa(double **matriz, double **resultado, int n);
double determinante(double **matriz, int n);
void crearArchivo(double **matriz, int n, char *nombre, char *directorio);
void imprimirArchivo(char *directorio, char *nombre);

int main(int argc, char const *argv[])
{
	// CREAR DIRECTORIO
	char* path = leerDirectorio();//Obtenemos el directorio desde la entrada de teclado
	
	//Variables para medición de tiempos
	double utime0, stime0, wtime0,utime1, stime1, wtime1; 
	uswtime(&utime0, &stime0, &wtime0);

	int i, n;
	double **matriz1, **matriz2, **suma, **resta, **mul, **tran1, **tran2, **inv1, **inv2;
	time_t t;
	srand((unsigned) time(&t));
	n = 10;

	// Inicializa las matrices.
	matriz1 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		matriz1[i] = (double*)calloc(n,sizeof(double));

	matriz2 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		matriz2[i] = (double*)calloc(n,sizeof(double));
	
	suma = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		suma[i] = (double*)calloc(n,sizeof(double));

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
		inv1[i] = (double*)calloc(n,sizeof(double));

	inv2 = (double**)calloc(n,sizeof(double*));
	for (i = 0; i < n; i++)
		inv2[i] = (double*)calloc(n,sizeof(double));

	//Llamda al sistema mkdir recibe la ruta del directorio a crear, y los permisos de escritura, lectura y ejecucion para cada tipo de usuario
	//Retorna -1 si ocurrieron errores
	if(mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) ==-1)
	{
		perror(path);
        exit(EXIT_FAILURE);
    }
    else
    {

		// Llena matriz 1 y matriz 2
		llenar(matriz1, n);
		llenar(matriz2, n);

		printf("MATRIZ 1\n"); imprimir(matriz1, n);
		printf("MATRIZ 2\n"); imprimir(matriz2, n);

		printf("SUMA\n"); sumar(matriz1, matriz2, suma, n); 
		crearArchivo(suma, n, "/suma.txt", path);

		printf("RESTA\n"); restar(matriz1, matriz2, resta, n); 
		crearArchivo(resta, n, "/resta.txt", path);

		printf("MULTIPLICAR\n"); multiplicar(matriz1, matriz2, mul, n); 
		crearArchivo(mul, n, "/mul.txt", path);
		
		printf("TRANSPUESTA MATRIZ 1\n"); transpuesta(matriz1, tran1, n); 
		crearArchivo(tran1, n, "/tran1.txt", path);

		printf("TRANSPUESTA MATRIZ 2\n"); transpuesta(matriz2, tran2, n); 
		crearArchivo(tran2, n, "/tran2.txt", path);

		printf("INVERSA MATRIZ 1\n"); 
		//Revisamos si la maztriz tiene inversa
		if(inversa(matriz1, inv1, n) != 0) 
			crearArchivo(inv1, n, "/inversa_1.txt", path);

		printf("INVERSA MATRIZ 2\n"); 
		if(inversa(matriz2, inv2, n) != 0) 
			crearArchivo(inv2, n, "/inversa_2.txt", path);

		printf(" ----------------------------------\n");
		printf(" ----------- RESULTADOS -----------\n");
		printf(" ----------------------------------\n");
				
		printf("SUMA\n"); imprimirArchivo(path, "/suma.txt");
		printf("\nRESTA\n"); imprimirArchivo(path, "/resta.txt");
		printf("\nMULTIPLICAR\n"); imprimirArchivo(path, "/mul.txt");
		printf("\nTRANSPUESTA MATRIZ 1\n"); imprimirArchivo(path, "/tran1.txt");
		printf("\nTRANSPUESTA MATRIZ 2\n"); imprimirArchivo(path, "/tran2.txt");
		printf("\nINVERSA MATRIZ 1\n"); imprimirArchivo(path, "/inversa_1.txt");
		printf("\nINVERSA MATRIZ 2\n"); imprimirArchivo(path, "/inversa_2.txt");
	}

	uswtime(&utime1, &stime1, &wtime1);

	//Cálculo del tiempo de ejecución del programa
	printf("\n\nTiempo ejecucion:  %.4f s\n",  wtime1 - wtime0);

	return 0;
}

void crearArchivo(double **matriz, int n, char *nombre, char *directorio)
{
    int i,j;
    char* dir = (char *)calloc(2000, sizeof(char));
    char* aux = (char *)calloc(2000, sizeof(char));
    char num[15];
    strcpy(aux, directorio);
    strcat(directorio, nombre);
    strcpy(dir, directorio);

    // LLamada al sistema cret, recibe la ruta del archivo a crear y los permisos
    // Retorna -1 si hay errores
    if(creat(directorio, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        perror(directorio);
        exit(EXIT_FAILURE);
    }
    else
    {
    	int a = open(directorio, O_WRONLY | O_APPEND);
    	if(a == -1)
    	{
    		perror(directorio);
        	exit(EXIT_FAILURE);
    	}
    	else
    	{
    		for (i = 0; i < n; i++)
        	{
            	for(j = 0; j< n; j++)
            	{
            		sprintf(num, "%.3f\t", matriz[i][j]);
                	if(!write(a, num, strlen(num)) == strlen (num))
                	{
                		perror(directorio);
        				exit(EXIT_FAILURE);
                	}
            	}
            	if(!write(a, "\n", strlen("\n")) == strlen ("\n"))
                	{
                		perror(directorio);
        				exit(EXIT_FAILURE);
                	}
        	}
    	}
    	close(a);
    }
    strcpy(directorio, aux);
    free(aux); free(dir); 
}

void imprimirArchivo(char *directorio, char *nombre)
{
	char* dir = (char *)calloc(2000, sizeof(char));
    char* aux = (char *)calloc(2000, sizeof(char));
   	strcpy(aux, directorio);
    strcpy(dir, directorio);
    strcat(dir, nombre);

    int archivo = open(dir, O_RDONLY);
    if(archivo == -1)
    {
    	perror(dir);
    	exit(EXIT_FAILURE);
    }
    struct stat sb;
    if(stat(dir, &sb) == -1)
    {
    	perror(dir);
    	exit(EXIT_FAILURE);
    }
   	long long longitud = (long long) sb.st_size;
   	char *contenido = (char *)calloc(longitud, sizeof(char));
   
   	if(read(archivo, contenido, longitud) == longitud)
   	{
   		printf("%s", contenido);
   	}
   	if(close(archivo) == -1)
   	{
   		perror(dir);
   		exit(EXIT_FAILURE);
   	}
   	free(contenido);
    strcpy(directorio, aux); 
}

char* leerDirectorio()
{
	char* directorio = (char*)calloc(2000,sizeof(char));
	printf("Ingrese el nuevo directorio: ");
	scanf("%s", directorio);
	return directorio;
}

void imprimir(double **m, int n)
{
	int i, j;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
			printf("%.3f\t", m[i][j]);					
		printf("\n");
	}
	printf("\n");
}

// Llena con numeros random 
void llenar(double **m, int n)
{
	int i, j;
	for (i = 0; i < n; i++) 
	{
		for (j = 0; j < n; j++)
		 {
			m[i][j] = (rand()%11);
		}
	}
}

void sumar(double **m1, double **m2, double **resultado, int n)
{
	int i, j;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
			resultado[i][j] = m1[i][j] + m2[i][j];
	}
}

void restar(double **m1, double **m2, double **resultado, int n)
{
	int i, j;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
			resultado[i][j] = m1[i][j] - m2[i][j];
	}	
}

void multiplicar(double **m1, double **m2, double **resultado, int n)
{
	int i, j, k, aux;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			aux = 0;
			for(k = 0; k < n; k++)
				aux = m1[i][k] * m2[k][j] + aux;
			resultado[i][j] = aux;
		}
	}
}

void transpuesta(double **m, double **resultado, int n)
{
	int i, j;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
			resultado[i][j] = m[j][i];
	}
}

bool esCero(double x)
{
	return fabs(x) < 1e-8;
}

double determinante(double **m, int n)
{  
	double det = 0, aux = 0;
    int c;
    //Si el orden es de 2, multiplica cruzadon directamente
    if(n==2)
        return m[0][0]*m[1][1] - m[1][0]*m[0][1];
    else
	{
		for(int j=0; j<n; j++)
		{
			//Crea arreglo dinamico temporal
			double **menor = (double **)malloc(sizeof(double)*(n-1));
			//Redimensiona
            for(int i=0; i<(n-1); i++) 
				menor[i] = (double *)malloc(sizeof(double)*(n-1));
            for(int k=1; k<n; k++)
			{
                c = 0;
                    for(int l=0; l<n; l++)
					{
                        if(l!=j)
						{
							/*Parte matriz principal en matrices de 3 
							y multiplica cruzado*/
                			menor[k-1][c] = m[k][l];
                            c++;
                        }
                    }
            }
        //Recursividad, repite la funcion
        aux = potencia(-1, 2+j)*m[0][j]*determinante(menor, n-1);
        det += aux;

        for(int x = 0; x<(n-1); x++)
            free(menor[x]);//Libera espacio en memoria     
        free(menor);
        }
    return det;//Devuelve resultado
    }
}

// Usando definicion de la adjunta
int inversa(double **A, double **resultado, int n)
{
	int tieneInversa;
	if(determinante(A, n) == 0)
	{
		tieneInversa=0;
		printf("La matriz no tiene inversa. Determinante = 0\n\n");
	}
	else{
		tieneInversa=1;
		int i, j, k, l; 
		double *tmp;
		tmp = (double*)malloc(sizeof(double)*n);

		for(i = 0; i < n; ++i)
			resultado[i][i] = 1;
		i = 0; j = 0;
		while(i < n && j < n)
		{
			if(esCero(A[i][j]))
			{
				for(k = i + 1; k < n; ++k)
				{
					if(!esCero(A[k][j]))
					{
						tmp = A[i];
						A[i] = A[k];
						A[k] = tmp;
						tmp = resultado[i];
						resultado[i] = resultado[k];
						resultado[k] = tmp;
						break;
					}
				}
			}
			if(!esCero(A[i][j]))
			{
				for(l = 0; l < n; ++l)
					resultado[i][l] /= A[i][j];
				for(l = n - 1; l >= j; --l)
					A[i][l] /= A[i][j];
				for(k = 0; k < n; ++k)
				{
					if(i == k) continue;
					for(l = 0; l < n; ++l)
						resultado[k][l] -= resultado[i][l] * A[k][j];
					for(l = n; l >= j; --l)
						A[k][l] -= A[i][l] * A[k][j];
				}
				++i;
			}
			++j;
		}
	}
	return tieneInversa;
}

int potencia(int base, int pot)
{
	int i, resultado = 1;
	for(i = 0; i < pot; i++)
		resultado = base * resultado;

	return resultado;
}
