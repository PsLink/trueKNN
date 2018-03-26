#include "header.h"
// #include <float.h>


#ifndef FLT_MAX
#define FLT_MAX 3.40282347e+38
#endif


__device__ inline float euclid_dist_2(
    int clusterId,
    int dim,
    const short int* __restrict__ query,
    const float* __restrict__ clusters) 
{
    float ans = 0.0;

    for (int i = 0; i < dim; i++) {
        float diff = query[i] - clusters[i + clusterId * dim];
        ans += diff * diff;
    }

    return ans;
}

__global__ void calDistance (
    int dim, 
    int k, 
    const short int* __restrict__ query, 
    const float* __restrict__ clusters, 
    float* __restrict__ dis)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < k) {
        dis[tid] = euclid_dist_2(tid, dim, query, clusters);
    }
}


void dis(
    std::vector<short int> &query,
    std::vector<std::vector<short int> > &Centroids,
    std::vector<float> &distance,
    int range_start,
    int batch_size) 
{
    int dim = query.size(), k = batch_size;

    float *cluster;
    cluster = (float *)calloc(k * dim, sizeof(float));

    for (int i = 0; i < k; i++)
        for (int j = 0; j < dim; j++) {
            cluster[i * dim + j] = Centroids[i + range_start][j];
        }

    distance.resize(k);

    cudaSetDevice(DEVICE_LOCAL_RANK);

    short int *d_query;
    cudaMalloc((void**)&d_query, dim * sizeof(short int));
    cudaMemcpy(d_query, query.data(), dim * sizeof(short int), cudaMemcpyDefault);

    float *d_dis;
    cudaMalloc((void**)&d_dis, k * sizeof(float));
    cudaMemset(d_dis, 0, k * sizeof(float));

    float *d_clusters;
    cudaMalloc((void**)&d_clusters, k * dim * sizeof(float));
    cudaMemcpy(d_clusters, cluster, k * dim * sizeof(float), cudaMemcpyDefault);

    int blockSize = 256;
    int gridSize = (k + blockSize - 1) / blockSize;
    // cudaSetDevice(DEVICE_LOCAL_RANK);
    calDistance <<< gridSize, blockSize>>>(dim, k, d_query, d_clusters, d_dis);
    cudaMemcpy(distance.data(), d_dis, k * sizeof(float), cudaMemcpyDefault);

    cudaFree(d_query);
    cudaFree(d_dis);
    cudaFree(d_clusters);
    
    free(cluster);
}