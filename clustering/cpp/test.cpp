#include "include/matrix.hpp"
#include "include/cluster.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <limits>

static inline float l2_sq(const float* a, const float* b, std::size_t d) {
    float acc = 0.f;
    for (std::size_t j = 0; j < d; ++j) { float diff = a[j] - b[j]; acc += diff*diff; }
    return acc;
}

int main() {
    using clk = std::chrono::high_resolution_clock;

    std::string str_data = "../data_eda.npy";
    Matrix mat(str_data);

    const std::size_t n   = mat.getN();
    const std::size_t dim = mat.getDim();
    if (n == 0 || dim == 0) {
        std::cerr << "No se pudo cargar el dataset: " << str_data << "\n";
        return 1;
    }

    const std::size_t ks[] = {0, 8, 16, 32, 64, 128};
    const std::size_t ms[] = {16, 32, 64, 128};
    const bool exclude_self = true;

    for (std::size_t k : ks) {


        std::vector<std::vector<std::size_t>> buckets;
        std::vector<std::vector<float>> centroids;
        std::vector<int> label;

        if (k > 0) {
            Cluster cl(mat, k);
            cl.applyClustering();

            buckets.resize(k);
            label.assign(n, -1);

            for (std::size_t c = 0; c < k; ++c) {
                buckets[c] = cl.getInds(c);
                for (auto id : buckets[c]) label[id] = (int)c;
            }

            centroids.assign(k, std::vector<float>(dim, 0.f));
            for (std::size_t c = 0; c < k; ++c) {
                if (buckets[c].empty()) continue;
                for (auto id : buckets[c]) {
                    const float* row = mat.getRow(id);
                    for (std::size_t j = 0; j < dim; ++j) centroids[c][j] += row[j];
                }
                const float inv = 1.0f / float(buckets[c].size());
                for (std::size_t j = 0; j < dim; ++j) centroids[c][j] *= inv;
            }
        }

        for (std::size_t m : ms) {

            double sum_comps = 0.0;
            double sum_time_dist_ms = 0.0;
            double sum_time_sort_ms = 0.0;

            for (std::size_t q = 0; q < n; ++q) {
                const float* query = mat.getRow(q);

                std::vector<std::size_t> cand;
                if (k == 0) {
                    cand.reserve(n);
                    for (std::size_t i = 0; i < n; ++i) {
                        if (exclude_self && i == q) continue;
                        cand.push_back(i);
                    }
                } else {
                    std::vector<std::pair<float,std::size_t>> cd; cd.reserve(k);
                    for (std::size_t c = 0; c < k; ++c) {
                        if (buckets[c].empty()) continue;
                        cd.emplace_back(l2_sq(query, centroids[c].data(), dim), c);
                    }
                    std::sort(cd.begin(), cd.end(),
                              [](auto& a, auto& b){ return a.first < b.first; });

                    cand.reserve(std::min<std::size_t>(m*2, n));
                    for (auto &p : cd) {
                        const auto& B = buckets[p.second];
                        for (auto idx : B) {
                            if (exclude_self && idx == q) continue;
                            cand.push_back(idx);
                        }
                        if (cand.size() >= m) break;
                    }
                    if (cand.size() < m) {
                        for (std::size_t i = 0; i < n && cand.size() < m; ++i) {
                            if (exclude_self && i == q) continue;
                            cand.push_back(i);
                        }
                    }
                }

                sum_comps += double(cand.size());

                std::vector<std::pair<float,std::size_t>> dists;
                dists.resize(cand.size());
                auto t0 = clk::now();
                for (std::size_t t = 0; t < cand.size(); ++t) {
                    const std::size_t idx = cand[t];
                    dists[t] = { l2_sq(query, mat.getRow(idx), dim), idx };
                }
                auto t1 = clk::now();
                sum_time_dist_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();

                const std::size_t mk = std::min<std::size_t>(m, dists.size());
                t0 = clk::now();
                std::partial_sort(dists.begin(), dists.begin()+mk, dists.end(),
                                  [](const auto& a, const auto& b){ return a.first < b.first; });
                t1 = clk::now();
                sum_time_sort_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();
            }

            const double avg_comps = sum_comps / double(n);
            const double avg_time_no_sort = sum_time_dist_ms / double(n);
            const double avg_time_with_sort = (sum_time_dist_ms + sum_time_sort_ms) / double(n);
            const double error_ms = avg_time_with_sort - avg_time_no_sort;

            std::cout << "#comparaciones (k=" << k << ", m=" << m << "): " << avg_comps
                      << " | Sin ordenación: " << std::fixed << std::setprecision(3) << avg_time_no_sort << " ms"
                      << " | Con ordenación: " << std::fixed << std::setprecision(3) << avg_time_with_sort << " ms"
                      << " | Error (con - sin): " << std::fixed << std::setprecision(3) << error_ms << " ms\n";
        }
    }

    return 0;
}
