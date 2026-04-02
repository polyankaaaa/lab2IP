#pragma once

#include "graph.hpp"

#include <cstddef>
#include <optional>

namespace gd4 {

class GraphMetrics {
public:
    explicit GraphMetrics(const Graph& graph) : graph_(graph) {}

    double density();
    std::size_t diameter();
    double transitivity();
    std::size_t connected_components_count();
    std::size_t articulation_points_count();
    std::size_t bridges_count();
    bool is_bipartite();
    std::size_t greedy_chromatic_upper_bound();

private:
    const Graph& graph_;
    std::optional<double> density_cache_;
    std::optional<std::size_t> diameter_cache_;
    std::optional<double> transitivity_cache_;
    std::optional<std::size_t> cc_cache_;
    std::optional<std::size_t> articulation_cache_;
    std::optional<std::size_t> bridges_cache_;
    std::optional<bool> bipartite_cache_;
    std::optional<std::size_t> chromatic_cache_;
};

}  
