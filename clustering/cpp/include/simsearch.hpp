#ifndef _SIM_SEARCH_HPP
#define _SIM_SEARCH_HPP

#include <cstddef>

class Matrix;

class SimSearch{
private:
    const Matrix &mat_data; // data to be processed
    const Matrix &mat_clusters; // centroids to be used

public:
    SimSearch(const Matrix& data, const Matrix& centroids);

    std::vector<std::size_t>
    search_with_clusters(const float* query, std::size_t top_k) const;

    std::vector<size_t> search_without(const float *query, top_k);
}

#endif 