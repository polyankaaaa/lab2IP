#pragma once

#include "graph.hpp"

#include <vector>

namespace gd4 {

class DfsVisitor {
public:
    virtual ~DfsVisitor() = default;
    virtual void discover_vertex(Vertex) {}
    virtual void tree_edge(Vertex, Vertex) {}
    virtual void back_edge(Vertex, Vertex) {}
    virtual void finish_vertex(Vertex) {}
};

void depth_first_search(const Graph& graph, DfsVisitor& visitor);
std::vector<std::vector<Vertex>> connected_components(const Graph& graph);
std::vector<Vertex> random_spanning_tree_edges(const Graph& graph, unsigned seed = 42);
std::vector<Vertex> random_cycle(const Graph& graph, unsigned seed = 42);

} 
