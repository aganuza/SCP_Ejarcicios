#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

#define MAX 100

int main(int argc, char** argv) {

	//Declaración de variables MPI
	int world_rank, world_size, i, j, k;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	//Comprobación de cantidad correcta de procesadores
	if (world_size < 2) {
  		fprintf(stderr, "Numero de procesadores debe ser mayor o igual que 2%s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	//Comprobación de que el número de procesadores no supera la DIMensión
	int DIM = atoi(argv[1]);
	if(DIM<=world_size){
		printf("Error! La cantidad de procesadores no puede superar las DIMensiones de la matriz");
		return(-1);
	}

	//Comprobacion de parametros
	if(argc != 2){
		printf("Error! ./e2s DIMension_Matrices\n");
		return(-1);
	}

	//Inicializar el tiempo de ejecución
	double t;
        struct timeval stop, start;
	gettimeofday(&start, NULL);

	//Inicializar matrices
	int *m1, *m2, *res, *mL1, *mL2;
	m1 = malloc(DIM*DIM*sizeof(int));
	m2 = malloc(DIM*DIM*sizeof(int));
	res = malloc(DIM*DIM*sizeof(int));

	//Llenar matriz1 con valores aleatorios
	if(world_rank==0){
		srand(time(0));
		for(i=0; i<DIM*DIM; i++){
			m1[i] = rand() % MAX;
		}
	}

	//Llenar matriz2 con valores aleatorios
        if(world_rank==0){
		srand(time(0));
                for(i=0; i<DIM*DIM; i++){
                        m2[i] = rand() % MAX;
                }
        }

	//Declaracion de matrices temporales
	mL1=malloc(((DIM*DIM)/world_size)*sizeof(int));
	mL2=malloc(((DIM*DIM)/world_size)*sizeof(int));

	//Scatter de las filas de la matriz1 y Broadcast de la matriz2 entera
	MPI_Scatter(&m1[0], (DIM*DIM)/world_size, MPI_INT, mL1, (DIM*DIM)/world_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&m2[0], DIM*DIM, MPI_INT, 0, MPI_COMM_WORLD);

	//Multicplicacion de las matrices
	int lag;
	for(i=0; i<DIM/world_size; i++){//Número de filas de matriz1 que se han mandado
		for (j=0; j<DIM; j++){//Puntero del vector-matriz del resultado
			lag = 0;
			for (k=0; k<DIM; k++){//Puntero de localización de la suma
				lag += mL1[i*DIM+k] * m2[j*DIM+k];//Cambiar de fila y correrla entera
			}
			mL2[i*DIM+j] = lag;//Registrar todos los resultados
		}
	}

	//Gather, recuperación de todos los datos mandados de vuelta en el proceso 0
	MPI_Gather(&mL2[0], (DIM*DIM)/world_size, MPI_INT, res, (DIM*DIM)/world_size, MPI_INT, 0, MPI_COMM_WORLD);

	//Tomar el tiempo de ejecución
	gettimeofday(&stop, NULL);
	t = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

	//Visualizar valores de la matriz
	if(world_rank==0){
		printf("\n");
		printf("Resultado: \n");
		for(i=0; i<DIM*DIM; i++){
			printf("[%d]", res[i]);
			if((i+1)%DIM==0){
				printf("\n");
			}
		}
	}

	//Visualizar tiempo de ejecución
        printf("\n");
	printf("Tiempo de multiplicacion: %f s \n", t);
        printf("\n");

	//Liberar la memoria
	free(m1);
	free(m2);
	free(res);
	free(mL1);
	free(mL2);

	MPI_Finalize();
	return(0);
}
