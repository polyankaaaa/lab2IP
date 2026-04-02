#include "parsers.hpp"

#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace gd4 {

Graph EdgeListParser::parse(std::istream& in) const {
    Graph g;
    std::size_t n = 0;
    in >> n;
    for (std::size_t i = 0; i < n; ++i) g.add_vertex();
    Vertex u = 0, v = 0;
    while (in >> u >> v) g.add_edge(u, v);
    return g;
}

Graph AdjacencyMatrixParser::parse(std::istream& in) const {
    Graph g;
    std::size_t n = 0;
    in >> n;
    for (std::size_t i = 0; i < n; ++i) g.add_vertex();
    for (Vertex i = 0; i < n; ++i) {
        for (Vertex j = 0; j < n; ++j) {
            int x = 0;
            in >> x;
            if (i < j && x) g.add_edge(i, j);
        }
    }
    return g;
}

Graph DimacsColoringParser::parse(std::istream& in) const {
    Graph g;
    std::string line;
    std::size_t n = 0;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == 'c') continue;
        std::istringstream iss(line);
        char kind = 0;
        iss >> kind;
        if (kind == 'p') {
            std::string name;
            std::size_t m = 0;
            iss >> name >> n >> m;
            (void)m;
            for (std::size_t i = 0; i < n; ++i) g.add_vertex();
        } else if (kind == 'e') {
            Vertex u = 0, v = 0;
            iss >> u >> v;
            if (u > 0 && v > 0) g.add_edge(u - 1, v - 1);
        }
    }
    return g;
}

Graph SnapParser::parse(std::istream& in) const {
    Graph g;
    std::string line;
    std::unordered_map<std::size_t, Vertex> remap;
    auto get_id = [&](std::size_t raw) -> Vertex {
        if (!remap.contains(raw)) remap[raw] = g.add_vertex();
        return remap[raw];
    };
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::size_t a = 0, b = 0;
        if (iss >> a >> b) g.add_edge(get_id(a), get_id(b));
    }
    return g;
}

std::unique_ptr<IGraphParser> create_parser(const std::string& format) {
    if (format == "edge-list") return std::make_unique<EdgeListParser>();
    if (format == "matrix") return std::make_unique<AdjacencyMatrixParser>();
    if (format == "dimacs") return std::make_unique<DimacsColoringParser>();
    if (format == "snap") return std::make_unique<SnapParser>();
    throw std::invalid_argument("Unsupported parser format: " + format);
}

}  
