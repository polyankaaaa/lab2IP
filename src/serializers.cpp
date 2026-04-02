#include "serializers.hpp"

#include <set>
#include <utility>

namespace gd4 {

void GraphVizSerializer::serialize(const Graph& graph, std::ostream& out) const {
    std::set<std::pair<Vertex, Vertex>> tree_edges;
    for (std::size_t i = 0; i + 1 < options_.highlighted_tree_edges.size(); i += 2) {
        auto u = options_.highlighted_tree_edges[i];
        auto v = options_.highlighted_tree_edges[i + 1];
        if (u > v) std::swap(u, v);
        tree_edges.insert({u, v});
    }

    std::set<std::pair<Vertex, Vertex>> cycle_edges;
    for (std::size_t i = 1; i < options_.highlighted_cycle.size(); ++i) {
        auto u = options_.highlighted_cycle[i - 1];
        auto v = options_.highlighted_cycle[i];
        if (u > v) std::swap(u, v);
        cycle_edges.insert({u, v});
    }

    out << "graph G {\n";
    for (Vertex v = 0; v < graph.vertex_count(); ++v) {
        out << "  " << v << ";\n";
    }
    for (Vertex u = 0; u < graph.vertex_count(); ++u) {
        for (Vertex v : graph.neighbors(u)) {
            if (u >= v) continue;
            std::string style;
            if (cycle_edges.contains({u, v})) {
                style = " [color=red, penwidth=2.0]";
            } else if (tree_edges.contains({u, v})) {
                style = " [color=blue, penwidth=2.0]";
            }
            out << "  " << u << " -- " << v << style << ";\n";
        }
    }
    out << "}\n";
}

void EdgesSerializer::serialize(const Graph& graph, std::ostream& out) const {
    out << graph.vertex_count() << " " << graph.edge_count() << "\n";
    for (Vertex u = 0; u < graph.vertex_count(); ++u) {
        for (Vertex v : graph.neighbors(u)) {
            if (u < v) out << u << " " << v << "\n";
        }
    }
}

}  
