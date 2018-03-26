#ifndef BASICDEFINITIONS_INCLUDED
#define BASICDEFINITIONS_INCLUDED

#define IDX2C(i,j,ld) (((j)*(ld))+(i))

const uint8_t DEVICE_LOCAL_RANK = atoi(std::getenv("OMPI_COMM_WORLD_LOCAL_RANK"));

struct Record
{
  float dist;
  int pID;  
};

#endif

