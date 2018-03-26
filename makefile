# minHashCluster1: main.cpp minHash.cu MinHashTools.cpp Random.cpp C2LSH.cpp
# 	nvcc -o minHashCluster main.cpp minHash.cu MinHashTools.cpp Random.cpp C2LSH.cpp

# LSH: main.cpp Random.cpp cublasLSH.cu dataProcess.cpp
# 	nvcc -lcublas -o LSH main.cpp Random.cpp cublasLSH.cu dataProcess.cpp

# Compilers

NVCC = nvcc

ALLOBJFILES = main.o gpu_dist.o util.o synPara.o
# ALLOBJFILES = cublasLSH.o dataProcess.o Random.o synPara.o cluster.o util.o minHash.o
# ALLOBJFILES = *.o
CFLAGS = -I/usr/mpi/gcc/openmpi-1.10.4-hfi/include
LDFLAGS = -L/usr/mpi/gcc/openmpi-1.10.4-hfi/lib64
FLAGS = -lmpi_cxx -lmpi -lopen-rte -lopen-pal -ldl -lnsl -lutil -lm

all:$(ALLOBJFILES) makefile
	nvcc -std=c++11 -o findKNN.out -gencode arch=compute_61,code=sm_61 -lm  -lcudart -lcublas $(LDFLAGS) $(FLAGS) $(ALLOBJFILES) 

%.o: %.cu 
	nvcc -std=c++11 -c -O2  -gencode arch=compute_61,code=sm_61  $(CFLAGS) -o $@ $<

%.o: %.cpp 
	nvcc -std=c++11 -c -O2  -gencode arch=compute_61,code=sm_61  $(CFLAGS) -o $@ $<

clean:
	-rm *.o
	-rm findKNN.out
