#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define MAX 100

int main(int argc, char** argv) {

	if(argc != 2){
		printf("Error! ./e2s Dimension_Matrices\n");
		return(-1);
	}
	//Inicializar el tiempo de ejecución
	double t;
        struct timeval stop, start;
	gettimeofday(&start, NULL);

	//Inicializar matrices
	int dim = atoi(argv[1]);
	int *m1, *m2, *res;
	m1 = malloc(dim*dim*sizeof(int));
	m2 = malloc(dim*dim*sizeof(int));
	res = malloc(dim*dim*sizeof(int));

	//Llenar matriz1 con valores aleatorios
	for(int i=0; i<dim*dim; i++){
		m1[i] = rand() % MAX;
	}

	//Llenar matriz2 con valores aleatorios
        for(int i=0; i<dim*dim; i++){
                m2[i] = rand() % MAX;
        }

	//Multiplicar matrices
	//t1 = time(NULL);
	int lag;
	for(int i=0; i<dim; i++){
		for (int j=0; j<dim; j++){
			lag = 0;
			for (int k=0; k<dim; k++){
				lag += m1[i+(j*dim)] * m2[k+(j*dim)];
			}
			res[i+(j*dim)] = lag;
		}
	}

	//Tomar el tiempo de ejecución
	gettimeofday(&stop, NULL);
	t = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

	//Visualizar valores de la matriz
	printf("\n");
	printf("Resultado: \n");
	for(int i=0; i<dim*dim; i++){
		printf("[%d]", res[i]);
		if((i+1)%dim==0){
			printf("\n");
		}
	}

        printf("\n");
	printf("Tiempo de multiplicacion: %f s \n", t);
        printf("\n");

	//Liberar la memoria
	free(m1);
	free(m2);
	free(res);
}
