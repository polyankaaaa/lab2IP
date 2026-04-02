#pragma once

#include "graph.hpp"

#include <ostream>
#include <string>
#include <vector>

namespace gd4 {

struct GraphVizOptions {
    std::vector<Vertex> highlighted_cycle;
    std::vector<Vertex> highlighted_tree_edges;  
};

class IGraphSerializer {
public:
    virtual ~IGraphSerializer() = default;
    virtual void serialize(const Graph& graph, std::ostream& out) const = 0;
};

class GraphVizSerializer final : public IGraphSerializer {
public:
    explicit GraphVizSerializer(GraphVizOptions options = {}) : options_(std::move(options)) {}
    void serialize(const Graph& graph, std::ostream& out) const override;

private:
    GraphVizOptions options_;
};

class EdgesSerializer final : public IGraphSerializer {
public:
    void serialize(const Graph& graph, std::ostream& out) const override;
};

}  
