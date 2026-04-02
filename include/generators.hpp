#pragma once

#include "graph.hpp"

namespace gd4 {

class GraphGenerator {
public:
    static Graph complete_graph(std::size_t n);
    static Graph complete_bipartite_graph(std::size_t n, std::size_t m);
    static Graph tree_graph(std::size_t n, unsigned seed = 42);
    static Graph star_graph(std::size_t n);
    static Graph cycle_graph(std::size_t n);
    static Graph path_graph(std::size_t n);
    static Graph wheel_graph(std::size_t n);
    static Graph erdos_renyi(std::size_t n, double p, unsigned seed = 42);
    static Graph random_cubic(std::size_t n, unsigned seed = 42);
    static Graph with_components(std::size_t n, std::size_t components, unsigned seed = 42);
    static Graph with_bridges(std::size_t n, std::size_t bridges, unsigned seed = 42);
    static Graph with_articulation_points(std::size_t n, std::size_t points, unsigned seed = 42);
    static Graph with_two_bridges(std::size_t n, std::size_t two_bridges, unsigned seed = 42);
    static Graph halin_graph(std::size_t leaf_cycle_size = 6);
};

}  
