

//mpirun -np 2 --host Aspire,Aspire m
//mpirun -np 4 --hostfile my-hosts m


#include "mpi.h"
#include <stdio.h>
#include "tempo.h"
#include <stdlib.h>
#include <math.h>

int fprimo(int, int);


int main(int argc,char *argv[])
{

    MPI_Status status;
    int  myid, proc;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&proc);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
 


	tempo1();


  if (myid == 0){

		int inicio = 0;
		int fim;	
		int m; 
		int i;
		int j;

		printf("\nDigite um numero limite: ") ;
		scanf("%d", &j);	
		k = ceil((int)limite / proc);

		int parte = j / proc;

		for (i=1; i < proc; i++){
			MPI_Send(&limite, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
		}

		while (num < limite)
		{
			num++;
			total = fprimo(num);
			total_mestre = total_mestre + total;
			while (i < proc)
			{
				num++;
				if (num > limite)
				{
					break;
				}
				else
				{
					MPI_Send(&num, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
					i++;
				}
			}
			i = 1;
		}
		
		tempo2();
		printf("\n O mestre %d terminou a execução em", myid);
		tempoFinal("mili segundos", argv[0], MSGLOG);

		for (i = 1; i < proc; i++)
		{
			MPI_Recv(&m, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
			totale += m;
			w++;
		}
	}else{
		MPI_Recv(&limite_slave, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
		z = ceil((int)limite_slave / proc);

		while (w < (z-1))
		{
			MPI_Recv(&numero_recebido, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
			x = fprimo(numero_recebido);
			count += x;
			w++;
		}

		MPI_Send(&count, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		tempo2();
		printf("\n O escravo %d terminou a execução em", myid);
		tempoFinal("mili segundos", argv[0], MSGLOG);
	}

	MPI_Finalize();
	return 0;
}

int fprimo(int num)
{
	int i;
	int aux;
	aux = 0;

	for (i = 1; i <= num; i++)
	{
		if (num % i == 0)
		{
			aux++;
		}

		if (aux > 2)
		{
			break;
		}
	}

	if (aux == 2)
	{
		return 1;
	}

	return 0;
}



