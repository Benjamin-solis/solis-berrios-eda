#include "include/utils.hpp"
#include "include/matrix.hpp"
#include "include/cluster.hpp"
#include "include/simsearch.hpp"



 void print_vector(const std::vector<size_t> &v){
    for (size_t i = 0; i < v.size() ; i++){
        std::cout<<v[i]<<",";
    }
    std::cout<<std::endl;
 }

int main(int args, char* vargs[]){
    std::string str_data("/home/jmsaavedrar/Research/git/eda_cpp/clustering/data_eda.npy");
    Matrix mat(str_data);
    const float *row = mat.getRow(999);
    
    print_array(row, mat.getDim());
    Cluster cluster(mat, 8);
    cluster.applyClustering();    
    std::cout << "--- print clusters" << std::endl;
    cluster.print_clusters();
    std::vector<size_t> inds_0= cluster.getInds(0);
    std::vector<size_t> inds_1= cluster.getInds(1);
    print_vector(inds_0);
    print_vector(inds_1);
    
    
    return 0;
}