

//mpirun -np 2 --host Aspire,Aspire m
//mpirun -np 4 --hostfile my-hosts m


#include "mpi.h"
#include <stdio.h>
#include "tempo.h"


int fprimo(int, int);


int main(int argc,char *argv[])
{

    MPI_Status status;
    int  myid, numprocs;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
 


	tempo1();


    if (myid == 0) 
    {

    		int inicio = 0;
    		int fim;	
    		int m; 
    		int i;
	        int n;


	        printf("\nDigite um numero limite: ") ;
	        scanf("%d", &n);	


	        int parte = n / numprocs;


			//printf("\nNumero total de processos: %d\n\n", numprocs);
			//mestre 0 envia para todos (1 ate n-1) escravos em ordem de i 
			for (i=1; i < numprocs; i++){
					MPI_Send(&inicio, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
 					fim = inicio + parte;
					MPI_Send(&fim, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
					inicio = fim;
			}


			fim = inicio + parte;
			int total = fprimo(inicio, fim); 
            tempo2();
            tempoFinal("mili segundos", argv[0], MSGLOG);



			//mestre recebe de todos os escravos em qualquer ordem
			for (i=1; i < numprocs; i++){

				    MPI_Recv(&m, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
					printf("mestre recebeu = %d  do slave %d\n", m, status.MPI_SOURCE);
					total = total + m;
			}
	printf("\n\nTOTAL de primos é %d \n\n", total);
    }else{

    		int  inicio, fim;
            MPI_Recv(&inicio, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(&fim, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
			printf("escravo %d recebeu = %6d ---  %d\n", myid, inicio, fim);


			int x = fprimo(inicio, fim);
            tempo2();
            tempoFinal("mili segundos", argv[0], MSGLOG);

            MPI_Send(&x, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;


}


 


int fprimo(int inicio, int fim)
{	
	int i;
	int x;
	int primo;
	int div;
	int total = 0;

	//printf("\nNumeros primos: ");


	for ( x = inicio; x <= fim; x++ )
	{
		div = 0;
		for (i = 1; i <= x; i++) {
	    	if (x % i == 0) { 
	     		div++;
	    	}
	  	}

	    if (div == 2){
	    	//printf("%d ", x);
	    	total = total + 1;
		}
	}

	printf("\n\nQtde de primos entre %d e %d é %d \n", inicio, fim, total);

	return total;
}

