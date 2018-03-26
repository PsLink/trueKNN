#ifndef SYN_PARA_H
#define SYN_PARA_H

void initMPIComm(
	int &world_rank,
	int &world_size,
	int &name_len,
	char *processor_name);

void finalizeMPIComm(
	const int world_rank,
	const int &world_size,
	const int name_len,
	const char *processor_name);

void mergeAnswers(
	std::vector<Record> &answer,
	int candidate_size);

#endif
