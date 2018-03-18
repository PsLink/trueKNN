#include "header.h"

struct Record
{
	float dist;
	int pID;	
};

bool Compare(const Record& l, const Record& r)
{
  return l.dist < r.dist;
}

int main(int argc, char **argv) {
	// Data storage
	std::vector<std::vector<short int> > DataSet; 
	std::vector<std::vector<short int> > Queries;

	// Init parameters
	int numOfData =  100000000;
	int batch_size = 10000000;
	// int numOfData =  10000;
	// int batch_size = 1000;
	int numOfQuery = 1;
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

	std::vector<Record> ranking;
	std::vector<Record> answers;

	std::vector<int> groupSize;

	groupSize.clear();
	answers.clear();
	query.resize(dim);

	for (int qryID = 0; qryID < numOfQuery; qryID++){

		printf("Query %d.\n", qryID);

		for (int i=0; i<dim; i++)
			query[i] = Queries[qryID][i];

		for (int batchID = 0; batchID < numOfData; batchID += batch_size) {
			printf("batch %d.\n", batchID);
			distance.clear();
			dis(query, DataSet, distance, batchID, batch_size);
			// std::sort(distance.begin(),distance.end());
			for (int i=0; i<batch_size; i++) {
				Record tmp;
				tmp.dist = sqrt(distance[i]);
				tmp.pID = batchID + i;
				ranking.push_back(tmp);
			}

			std::sort(ranking.begin(),ranking.end(), &Compare);

			for (int i=0; i<100; i++)
				answers.push_back(ranking[i]);

			//for (int i=0; i<batch_size; i++) {
			//	printf("%d %0.5f \n", ranking[i].pID, ranking[i].dist);
			//}
		}

		// record top k
		std::sort(answers.begin(),answers.end(), &Compare);
		fprintf(fout, "%d %.5f \n", answers[100].pID,answers[100].dist);
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
