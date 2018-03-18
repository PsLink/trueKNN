#ifndef MINHASHTOOLS_INCLUDED
#define MINHASHTOOLS_INCLUDED

void parse_command_line(
	int argc, 
	char **argv, 
	char *input_file_name, 
	int &numOfPoint, 
	int &dim);

void init_data (
	int numOfData,
	int dim,
	std::vector<std::vector<short int> > &Data,
	char* filename);

void init_binary_data(
	int n,
	int dim,
	std::vector<std::vector<short int> > &Data,
	char* filename);

#endif
