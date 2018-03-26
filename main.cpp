#include "header.h"

bool Compare(const Record& l, const Record& r)
{
  return l.dist < r.dist;
}

int main(int argc, char **argv) {
	clock_t start, end;
	// Data storage
	std::vector<std::vector<short int> > DataSet; 
	std::vector<std::vector<short int> > Queries;

	// Init parameters
	int numOfData =  100000000;
	int batch_size = 20000000;
	// int numOfData =  10000;
	// int batch_size = 1000;
	int numOfQuery = 1000;
	int queryFileID = 1;
	int dim = 128;
	int candidate_size = 10;

	int world_rank;
	int world_size;
	int name_len;
	char processor_name[100];

	char dataFile[100] = "/home/pingyi/DATASET/learn.bvecs";
	char queryFile[100] = "/home/pingyi/DATASET/QUERY/query.csv";
	char outputFile[100];

	// FILE *fout = fopen("topk.query", "w");
	FILE *fout;
	FILE *ftout = fopen("times.txt","w");

	// --------------------------------------------------------------------------
	// initialization
	srand (time(NULL));

	// parse_command_line(argc, argv, queryFile, numOfData, dim);
	init_data(numOfQuery, dim, Queries, queryFile);
	printf("succesfully read query.\n");

	init_binary_data(numOfData, dim, DataSet, dataFile);
	printf("succesfully read data.\n");

	std::vector<short int> query;
	std::vector<float> distance;

	std::vector<Record> ranking;
	std::vector<Record> answers;

	query.resize(dim);

	initMPIComm(world_rank, world_size, name_len, processor_name);

	batch_size = numOfData / world_size;

	for (int qryID = 0; qryID < numOfQuery; qryID++){

		printf("Query %d.\n", qryID);
		start = clock();

		for (int i=0; i<dim; i++)
			query[i] = Queries[qryID][i];

		answers.clear();
		distance.clear();
		ranking.clear();

		if (world_rank == world_size - 1) {
			dis(query, DataSet, distance, world_rank*batch_size, numOfData - world_rank*batch_size);
		} else {
			dis(query, DataSet, distance, world_rank*batch_size, batch_size);
		}

		for (int i=0; i<batch_size; i++) {
			Record tmp;
			tmp.dist = sqrt(distance[i]);
			tmp.pID = world_rank*batch_size + i;
			ranking.push_back(tmp);
		}

		std::sort(ranking.begin(),ranking.end(), &Compare);

		for (int i=0; i<=candidate_size; i++)
			answers.push_back(ranking[i]);
		// }

		mergeAnswers(answers,candidate_size);

		std::sort(answers.begin(),answers.end(), &Compare);

		// record top k
		strcpy(outputFile,"/home/pingyi/kNNs-1/query");
		sprintf(outputFile, "%s_%d.csv", outputFile, qryID + queryFileID * 250);

		fout = fopen(outputFile,"w");

		std::sort(answers.begin(),answers.end(), &Compare);
		for (int i=0; i<=candidate_size; i++)
			fprintf(fout, "%d %.5f \n", answers[i].pID,answers[i].dist);
		fclose(fout);

		end = clock();

		fprintf(ftout, "%.5f \n", (double)(end - start) / CLOCKS_PER_SEC);
		answers.clear();
	}

	finalizeMPIComm(world_rank,world_size,name_len,processor_name);
	fclose(ftout);
	return 0;
}
