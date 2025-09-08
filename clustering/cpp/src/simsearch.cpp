#include "../include/simsearch.hpp"
#include "../include/matrix.hpp"
#include "../include/utils.hpp" //Para usar argsort()
#include "../include/cluster.hpp"
#include <vector>
#include <algorithm>
#include <climits>


static inline float dis(const float * a, const float * b){
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];

    return dx*dx + dy*dy;
}

SimSearch::SimSearch(const Matrix &data, const Matrix &clusters)
    : mat_data(data), mat_clusters(clusters) {}



std::vector<std::size_t> SimSearch::search_without(const float* query, std::size_t top_k) const{
    std::vector<float> dist(mat_data.getN());

    for(std::size_t i=0; i < mat_data.getN(); i++){
        const float* row = mat_data.getRow(i);
        dist[i] = dis(query,  row);
    }
<<<<<<< HEAD
    return {};
    }

std::vector<std::size_t> SimSearch::search_with_clusters(const float* query, std::size_t top_k) const{
    // Primero encontrar el cluster más cercano al query
    float min_cluster_dist = std::numeric_limits<float>::max();
    std::size_t best_cluster = 0;
    
    for(std::size_t i = 0; i < mat_clusters.getN(); i++){
        const float* centroid = mat_clusters.getRow(i);
        float dist = vec_compute_distance(query, centroid, mat_clusters.getDim());
        if(dist < min_cluster_dist){
            min_cluster_dist = dist;
            best_cluster = i;
        }
    }
    
    // Crear un Cluster temporal para obtener los índices del mejor cluster
    Cluster temp_cluster(mat_data, mat_clusters.getN());
    temp_cluster.compute_clusters();
    
    // Obtener los índices de vectores en el cluster más cercano
    std::vector<std::size_t> cluster_indices = temp_cluster.getInds(best_cluster);
    
    // Si no hay suficientes vectores en el cluster, buscar en clusters adyacentes
    if(cluster_indices.size() < top_k){
        // Calcular distancias a todos los clusters y ordenarlos
        std::vector<std::pair<float, std::size_t>> cluster_distances;
        for(std::size_t i = 0; i < mat_clusters.getN(); i++){
            const float* centroid = mat_clusters.getRow(i);
            float dist = vec_compute_distance(query, centroid, mat_clusters.getDim());
            cluster_distances.push_back({dist, i});
        }
        
        std::sort(cluster_distances.begin(), cluster_distances.end());
        
        // Expandir búsqueda a clusters más cercanos hasta tener suficientes vectores
        cluster_indices.clear();
        for(const auto& cluster_pair : cluster_distances){
            std::vector<std::size_t> current_cluster_indices = temp_cluster.getInds(cluster_pair.second);
            cluster_indices.insert(cluster_indices.end(), current_cluster_indices.begin(), current_cluster_indices.end());
            if(cluster_indices.size() >= top_k) break;
        }
    }
    
    // Calcular distancias solo para los vectores en los clusters seleccionados
    std::vector<std::pair<float, std::size_t>> candidate_distances;
    for(std::size_t idx : cluster_indices){
        const float* row = mat_data.getRow(idx);
        float dist = vec_compute_distance(query, row, mat_data.getDim());
        candidate_distances.push_back({dist, idx});
    }
    
    // Ordenar por distancia
    std::sort(candidate_distances.begin(), candidate_distances.end());
    
    // Retornar los top_k primeros
    std::vector<std::size_t> result;
    for(std::size_t i = 0; i < std::min(top_k, candidate_distances.size()); i++){
        result.push_back(candidate_distances[i].second);
    }
    
    return result;
}
    
=======

    std::size_t *indice = argsort(dist.data(), mat_data.getN());


    std::vector<size_t> fin(indice, indice + top_k);
    free(indice);
    return fin;
}
>>>>>>> b547aa77595d096f19a5792b5932d9a8664d9ecb
