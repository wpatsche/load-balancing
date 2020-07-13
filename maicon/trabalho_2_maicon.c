//mpirun -np 2 --host Aspire,Aspire m
//mpirun -np 4 --hostfile my-hosts m
#include "mpi.h"
#include <stdio.h>
#include "tempo.h"
#include <stdlib.h>
#include <math.h>

int fprimo(int);

int main(int argc, char *argv[])
{	
	MPI_Status status;
	int myid, numprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	tempo1();

	if (myid == 0)	/*mestre*/
	{
		int numero_limite = 0;
		int numero_calculado = 0;
		int total_primos_mestre = 0;
		int total_primos_escravo = 0;
		int total_primos_final = 0;
		int total = 0;
		int parte = 0;
		int y = 0;
		int i = 1;
		int m;

		printf("\nDigite um número para calcular os primos: \n");
		scanf("%d", &numero_limite);
		//variavel para saber a parte que cada escravo vai ter para calcular
		parte = ceil((int)numero_limite / numprocs);

		//mestre 0 envia para todos (1 ate n-1) escravos em ordem de i
		for (i = 1; i < numprocs; i++)
		{
			//var qtde tipo dest etiq
			MPI_Send(&numero_limite, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
		}
		//loop para controlar os números calculados
		while (numero_calculado < numero_limite)
		{
			numero_calculado++;
			total = fprimo(numero_calculado);
			total_primos_mestre = total_primos_mestre + total;

			//loop para controlar os processos
			while (i < numprocs)
			{
				numero_calculado++;
				//comparação para ver se o número calculado não é maior que o número limite
				if (numero_calculado > numero_limite)
				{
					break;
				}
				else
				{
					//var qtde tipo dest etiq
					MPI_Send(&numero_calculado, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
					i++;
				}
			}
			i = 1;
		}
		tempo2();
		printf("\n O MESTRE %d terminou a execução em ", myid);
		tempoFinal("mili segundos", argv[0], MSGLOG);

		//mestre recebe de todos os escravos em qualquer ordem
		for (i = 1; i < numprocs; i++)
		{
			MPI_Recv(&m, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
			total_primos_escravo = total_primos_escravo + m;
			y++;
		}
		//somatório final do número de primos calculados pelos escravos e pelo mestre
		total_primos_final = total_primos_escravo + total_primos_mestre;
		printf("\n\nTOTAL de primos é : %d \n\n", total_primos_final);
	}
	else{	/*escravo*/
		
		int numero_recebido;
		int limite_escravo;
		int contador = 0;
		int parte_escravo = 0;
		int x = 0;
		int limite = 0;

		MPI_Recv(&limite_escravo, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
		parte_escravo = ceil((int)limite_escravo / numprocs);
		
		//loop para saber qual parte cada escravo deve calcular 
		while (limite < (parte_escravo - 1))
		{
			MPI_Recv(&numero_recebido, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
			//	printf("\n escravo %d recebeu = %6d \n", myid, numero_recebido);
			x = fprimo(numero_recebido);
			contador += x;
			limite++;

		}

		//var qtde tipo dest etiq
		MPI_Send(&contador, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		tempo2();

		printf("\n O ESCRAVO %d terminou a execução em ", myid);
		tempoFinal("mili segundos", argv[0], MSGLOG);
	}
	MPI_Finalize();
	return 0;
}

//função que testa se o número é primo ou não
int fprimo(int numero_testado)
{
	int i;
	int div;
	div = 0;

	for (i = 1; i <= numero_testado; i++)
	{
		if (numero_testado % i == 0)
		{
			div++;
		}
		if (div > 2)
		{
			break;
		}
	}
	if (div == 2)
	{
		return 1;
	}
	return 0;
}


/*
                N  PRIME_NUMBER
                1           0
               10           4
              100          25
            1,000         168
           10,000       1,229
          100,000       9,592
        1,000,000      78,498
       10,000,000     664,579
      100,000,000   5,761,455
    1,000,000,000  50,847,534
*/