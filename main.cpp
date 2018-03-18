#include "header.h"

struct Record
{
	float dist;
	int pID;	
};

bool Compare(const record& l, const record& r)
{
  return l.dist < r.dist;
}

int main(int argc, char **argv) {
	// Data storage
	std::vector<std::vector<short int> > DataSet; 
	std::vector<std::vector<short int> > Queries;

	// Init parameters
	// int numOfData =  100000000;
	// int batch_size = 1000000;
	int numOfData =  10000;
	int batch_size = 1000;
	int numOfQuery = 1000;
	int dim = 128;
	char dataFile[100] = "/home/pingyi/DATASET/learn.bvecs";
	char queryFile[100] = "/home/pingyi/DATASET/QUERY/query.csv";

	FILE *fout = fopen("topk.query", "w");

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
	std::vector<float> answer;

	std::vector<Record> ranking;

	std::vector<int> groupSize;
    groupSize.clear();


	query.resize(dim);
	for (int qryID = 0; qryID < numOfQuery; qryID++){

		printf("Query %d.\n", qryID);

		for (int i=0; i<dim; i++)
			query[i] = Queries[qryID][i];

		for (int batchID = 0; batchID < numOfData; batchID += batch_size) {

			distance.clear();
			dis(query, DataSet, distance, batchID, batch_size);
			// std::sort(distance.begin(),distance.end());
			for (int i=0; i<batch_size; i=0; i++) {
				Record tmp;
				tmp.dist = distance[i];
				tmp.pID = batchID + i;
				ranking.push_back(tmp);
			}

			std::sort(ranking.begin(),ranking.end(), &Compare);

			for (int i=0; i<batch_size; i=0; i++) {
				printf("%d %0.5f \n", ranking[i].pID, ranking[i].dist);
			}
		}

		// record top k
		// std::sort(answer.begin(),answer.end());
		// fprintf(fout, "%f %f \n", answer[10],sqrt(answer[10]));
		// fprintf(fout, "%f %f \n", answer[100],sqrt(answer[100]));
		// fprintf(fout, "%f %f \n", answer[1000],sqrt(answer[1000]));
		// fprintf(fout, "\n");
	}

 	fclose(fout);
	return 0;
}


// for( int i=0; i<num_gpus; i++ ) {
// cudaSetDevice( gpu
// [i] );
// kernel<<<..., stream_halo
// [i]>>>( ... );
// kernel<<<..., stream_halo
// [i]>>>( ... );
// cudaStreamQuery( stream_halo
// [i] );
// kernel<<<..., stream_internal
// [i]>>>( ... );
// }
