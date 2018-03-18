#include "header.h"

void parse_command_line(
	int argc,
	char **argv,
	char *input_file_name,
	int &numOfPoint,
	int &dim)
{
	int i;
	// parse options
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-') break;
		if (++i >= argc)
			exit(1);
		// printf("i:%d para:%s \n",i,argv[i]);
		switch (argv[i - 1][1]) {
		case 'n':
			numOfPoint = atoi(argv[i]);
			break;
		case 'd':
			dim = atoi(argv[i]);
			break;
		case 'f':
			strcpy(input_file_name, argv[i]);
			break;
		default:
			fprintf(stderr, "Unknown option: -%c\n", argv[i - 1][1]);
		}
	}
	printf("numOfPoint: %d \n", numOfPoint);
	printf("dim: %d \n", dim);
	printf("inputFile: %s \n", input_file_name);
}

void init_data (
	int numOfData,
	int dim,
	std::vector<std::vector<short int> > &Data,
	char* filename) 
{
	Data.clear();
	
	FILE *fin = fopen(filename, "r");
	int test;	
	// printf("reading from %s.\n",filename);
	for (int i = 0; i < numOfData; i++) {
		std::vector<short int> tmpPoint;
		tmpPoint.resize(dim);
		// DataSet[i].resize(dim);
		for (int j = 0; j < dim; j++) {
			test = fscanf(fin, "%d", &tmpPoint[j]);
		}
		Data.push_back(tmpPoint);
	}
	fclose(fin);
}

void init_binary_data(
	int n,
	int dim,
	std::vector<std::vector<short int> > &Data,
	char* filename) 
{
	Data.clear();

	ios::sync_with_stdio(false);
	cin.tie(NULL);

	ifstream fin;
	// -------------------------------------------------------------------------
	//  read original file
	// -------------------------------------------------------------------------
	fin.open(filename, ifstream::binary);
	int d = -1;
	unsigned char c = -1;
	// unsigned char** data = new unsigned char*[n];
	for (int i = 0; i < n; i++) {
		fin.read((char *) (&d), sizeof(int));
		if (dim != d) {
			cout << "dim = " << dim << ", d = " << d << endl;
			exit(1);
		}
		// data[i] = new unsigned char[d];
		std::vector<short int> data;
		data.clear();
		
		for (int j = 0; j < d; j++) {
			fin.read((char *) (&c), sizeof(unsigned char));
			// data[i][j] = c;
			data.push_back((short int) c);
		}
		Data.emplace_back(data);
	}
	fin.close();
}