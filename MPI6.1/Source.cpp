
#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <random>
#include <iostream>
#define N 1
using namespace std;

//mpiexec -n 7 MPI6.1.exe

int main(int* argc, char** argv)
{
	int rank, size, dims[N];
	int periods[N], source, dest;
	int reorder = 0;

	MPI_Comm comm_cart;
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm comm;
	MPI_Comm comm1;
	MPI_Comm comm2;
	MPI_Comm StarComm;
	MPI_Group group;
	MPI_Group group1;
	MPI_Group group2;
	int size1 = 7;

	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_group(comm, &group);

	int* ranks = new int[7];
	for (int i = 0; i < 7; i++) {
		ranks[i] = i;
	}

	int* ranks1 = new int[6];
	for (int i = 0; i < 6; i++) {
		ranks1[i] = 7+i;
	}
	
	MPI_Group_incl(group, 7, ranks, &group1);
	MPI_Comm_create(comm, group1, &comm1);

	MPI_Group_incl(group, 6, ranks1, &group2);
	MPI_Comm_create(comm, group2, &comm2);
	
	int* arr1, * arr_res1;
	int* arr, * arr_res;
	int n = 5;


	int rank1[1];
	int rank2[6];
	int rank0[6];

	int index[] = { 1, 2, 3, 4, 5, 10 };

	int edges[] = { 5, 5, 5, 5, 5, 0, 1, 2, 3, 4 };

	//int edges[] = { 12, 12, 12, 12, 12, 7, 8, 9, 10, 11 };
	

	arr1 = (int*)malloc(size1 * sizeof(int));
	for (int i = 0; i < size1; i++)
	{
		arr1[i] = 0;
	}

	 
	for (int i = 0; i < N; i++) { 
		dims[i] = 0; 
		periods[i] = 1; 
	}

	if (rank < 7 )
	{

		MPI_Dims_create(size1, N, dims);
		MPI_Cart_create(comm1, N, dims, periods, reorder, &comm_cart);
		MPI_Cart_shift(comm_cart, 0, 1, &source, &dest);



		


		if (rank == 0)
		{
			MPI_Send(&size1, 1, MPI_INT, dest, 1, comm_cart);
			MPI_Send(arr1, size1, MPI_INT, dest, 1, comm_cart);

			MPI_Recv(&size, 1, MPI_INT, source, 1, comm_cart, &status);
			arr_res1 = (int*)malloc(size1 * sizeof(int));
			MPI_Recv(arr_res1, size1, MPI_INT, source, 1, comm_cart, &status);

			cout << " arr_res1\n";

			for (int i = 0; i < size1; i++) {
				cout << arr_res1[i] << " ";
			}
			cout << endl << endl;

		}

		else
		{

			MPI_Recv(&size1, 1, MPI_INT, source, 1, comm_cart, &status);
			MPI_Recv(arr1, size1, MPI_INT, source, 1, comm_cart, &status);
			for (int i = 0; i < size1; i++)
			{
				if (i == rank)
					arr1[i] = rank;

			}


			MPI_Send(&size1, 1, MPI_INT, dest, 1, comm_cart);
			MPI_Send(arr1, size1, MPI_INT, dest, 1, comm_cart);

		}
	}

	else {

		rank = rank - 7;
		MPI_Graph_create(comm2, 6, index, edges, 1, &StarComm);
		MPI_Graphdims_get(StarComm, index, edges);

	
		

		MPI_Graph_neighbors(StarComm, rank, 5, rank2);
		

		if (rank == 5) {

			arr = (int*)malloc(n * sizeof(int));

			cout << " 5 process sends arr\n";

			for (int i = 0; i < n; i++)
			{
				arr[i] = 5;
				cout << arr[i] << " ";
			}
			cout << endl;
			// MPI_Bcast(arr, n, MPI_INT, 0, StarComm);



			MPI_Graph_neighbors(StarComm, rank, 5, rank0);

			for (int i = 0; i < 5; i++)
			{

				MPI_Send(arr, n, MPI_INT, rank0[i], 1, StarComm);

			}

			arr_res = (int*)malloc(n * sizeof(int));


			cout << "All processes send a response to process 5\n";
			for (int i = 0; i < 5; i++)
			{
				cout << rank0[i] << endl;

				MPI_Recv(arr_res, n, MPI_INT, rank0[i], 1, StarComm, &status);

				for (int i = 0; i < n; i++)
				{
					cout << arr_res[i] << " ";

				}
				cout << endl;

			}




		}

		else {


			arr = (int*)malloc(n * sizeof(int));

			//MPI_Bcast(arr, n, MPI_INT, 0, StarComm);



			MPI_Graph_neighbors(StarComm, rank, 1, rank1);

			MPI_Recv(arr, n, MPI_INT, rank1[0], 1, StarComm, &status);


			for (int i = 0; i < n; i++)
			{
				if (i == rank)
					arr[i] = rank;

			}


			MPI_Send(arr, 5, MPI_INT, rank1[0], 1, StarComm);




		}
	}


MPI_Comm_free(&comm);
//MPI_Comm_free(&comm_cart);
 MPI_Finalize();

 return 0;

}


