#include "../include/simsearch.hpp"
#include "../include/matrix.hpp"
#include "../include/utils.hpp" //Para usar argsort()


std::size_t dis(const std::size_t * a, std::size_t * b){
    std::size_t dx = a[0] - b[0];
    std::size_t dy = a[1] - b[1];

    return dx*dx - dy*dy;
}

SimSearch::SimSearch(const Matrix &data, const Matrix &clusters)
    : mat_data(data), mat_clusters(clusters) {}



std::vector<std::size_t> SimSearch::search_without(const float* query, std::size_t top_k) const{
    std::vector<float> dist(mat_data.getN());

    for(std::size_t i=0; i < mat_data.getN(); i++){
        const float* row = mat_data.getRow(i);
        dist[i] = dis(query,  row);
    }
    return {};
    }