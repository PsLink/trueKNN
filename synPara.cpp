#include "header.h"

using namespace std;

#define IDX2C(i,j,ld) (((j)*(ld))+(i))

void initMPIComm(
	int &world_rank,
	int &world_size,
	int &name_len,
	char *processor_name)
{
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Get_processor_name(processor_name, &name_len);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	printf("Processor %s, rank %d finish initilaztion.\n", processor_name, world_rank);
}

void finalizeMPIComm(
	const int world_rank,
	const int &world_size,
	const int name_len,
	const char *processor_name)
{
	MPI_Finalize();
	printf("Processor %s, rank %d communication ends.\n", processor_name, world_rank);
}

void mergeAnswers(
	std::vector<Record> &answer,
	int candidate_size)
{
	int world_rank,world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int *buff_ID = new int[candidate_size];
	float *buff_dis = new float[candidate_size];
	int *buff_ID_all = new int[candidate_size * world_size];
	float *buff_dis_all = new float[candidate_size * world_size];

	for (int i=0; i<candidate_size; i++) {
		buff_ID[i] = answer[i].pID;
		buff_dis[i] = answer[i].dist;
	}

	// for (int i=0; i<candidate_size; i++) {
	// 	printf("%d ", buff_ID[i]);
	// }
	// printf("\n");
	// for (int i=0; i<candidate_size; i++) {
	// 	printf("%.3f ", buff_dis[i]);
	// }
	// printf("\n");

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Allgather(buff_ID, candidate_size, MPI_INT, buff_ID_all, candidate_size, MPI_INT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Allgather(buff_dis, candidate_size, MPI_FLOAT, buff_dis_all, candidate_size, MPI_FLOAT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	// for (int i=0; i<candidate_size * world_size; i++) {
	// 	printf("%d ", buff_ID_all[i]);
	// }
	// printf("\n");
	// for (int i=0; i<candidate_size * world_size; i++) {
	// 	printf("%.3f ", buff_dis_all[i]);
	// }
	// printf("\n");

	//merge to answer
	answer.clear();
	for (int i=0; i<candidate_size*world_size; i++) {
		Record tmp;
		tmp.dist = buff_dis_all[i];
		tmp.pID = buff_ID_all[i];
		answer.push_back(tmp);
	}

	delete[] buff_ID;
	delete[] buff_dis;
	delete[] buff_ID_all;
	delete[] buff_dis_all;
}

