#ifndef DIST_INCLUDED
#define DIST_INCLUDED

void dis(
    std::vector<short int> &query,
    std::vector<std::vector<short int> > &Centroids,
    std::vector<float> &distance,
    int range_start,
    int batch_size);

#endif
