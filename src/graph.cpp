#include "graph.hpp"

#include <algorithm>
#include <stdexcept>

namespace gd4 {

namespace {
std::unique_ptr<IGraphStorage> make_storage(Graph::Backend backend) {
    if (backend == Graph::Backend::AdjacencyMatrix) {
        return std::make_unique<AdjacencyMatrixStorage>();
    }
    return std::make_unique<AdjacencyListStorage>();
}
}  // namespace

std::unique_ptr<IGraphStorage> AdjacencyListStorage::clone() const {
    return std::make_unique<AdjacencyListStorage>(*this);
}

Vertex AdjacencyListStorage::add_vertex() {
    adj_.emplace_back();
    return adj_.size() - 1;
}

bool AdjacencyListStorage::has_vertex(Vertex v) const { return v < adj_.size(); }

std::size_t AdjacencyListStorage::vertex_count() const { return adj_.size(); }

bool AdjacencyListStorage::add_edge(Vertex u, Vertex v) {
    if (u == v || !has_vertex(u) || !has_vertex(v) || has_edge(u, v)) {
        return false;
    }
    adj_[u].insert(v);
    adj_[v].insert(u);
    ++edge_count_;
    return true;
}

bool AdjacencyListStorage::has_edge(Vertex u, Vertex v) const {
    return has_vertex(u) && has_vertex(v) && adj_[u].contains(v);
}

std::size_t AdjacencyListStorage::edge_count() const { return edge_count_; }

std::vector<Vertex> AdjacencyListStorage::neighbors(Vertex v) const {
    if (!has_vertex(v)) return {};
    std::vector<Vertex> out(adj_[v].begin(), adj_[v].end());
    std::sort(out.begin(), out.end());
    return out;
}

void AdjacencyListStorage::remove_edge(Vertex u, Vertex v) {
    if (!has_edge(u, v)) return;
    adj_[u].erase(v);
    adj_[v].erase(u);
    --edge_count_;
}

std::unique_ptr<IGraphStorage> AdjacencyMatrixStorage::clone() const {
    return std::make_unique<AdjacencyMatrixStorage>(*this);
}

Vertex AdjacencyMatrixStorage::add_vertex() {
    const std::size_t n = matrix_.size();
    for (auto& row : matrix_) row.push_back(false);
    matrix_.push_back(std::vector<bool>(n + 1, false));
    return matrix_.size() - 1;
}

bool AdjacencyMatrixStorage::has_vertex(Vertex v) const { return v < matrix_.size(); }
std::size_t AdjacencyMatrixStorage::vertex_count() const { return matrix_.size(); }

bool AdjacencyMatrixStorage::add_edge(Vertex u, Vertex v) {
    if (u == v || !has_vertex(u) || !has_vertex(v) || matrix_[u][v]) return false;
    matrix_[u][v] = matrix_[v][u] = true;
    ++edge_count_;
    return true;
}

bool AdjacencyMatrixStorage::has_edge(Vertex u, Vertex v) const {
    return has_vertex(u) && has_vertex(v) && matrix_[u][v];
}

std::size_t AdjacencyMatrixStorage::edge_count() const { return edge_count_; }

std::vector<Vertex> AdjacencyMatrixStorage::neighbors(Vertex v) const {
    if (!has_vertex(v)) return {};
    std::vector<Vertex> out;
    out.reserve(matrix_.size());
    for (Vertex u = 0; u < matrix_.size(); ++u) {
        if (matrix_[v][u]) out.push_back(u);
    }
    return out;
}

void AdjacencyMatrixStorage::remove_edge(Vertex u, Vertex v) {
    if (!has_edge(u, v)) return;
    matrix_[u][v] = matrix_[v][u] = false;
    --edge_count_;
}

Graph::Graph(Backend backend) : storage_(make_storage(backend)), backend_(backend) {}
Graph::Graph(const Graph& other) : storage_(other.storage_->clone()), backend_(other.backend_) {}
Graph& Graph::operator=(const Graph& other) {
    if (this == &other) return *this;
    backend_ = other.backend_;
    storage_ = other.storage_->clone();
    return *this;
}

Vertex Graph::add_vertex() { return storage_->add_vertex(); }
bool Graph::has_vertex(Vertex v) const { return storage_->has_vertex(v); }
std::size_t Graph::vertex_count() const { return storage_->vertex_count(); }
bool Graph::add_edge(Vertex u, Vertex v) { return storage_->add_edge(u, v); }
bool Graph::has_edge(Vertex u, Vertex v) const { return storage_->has_edge(u, v); }
std::size_t Graph::edge_count() const { return storage_->edge_count(); }
std::vector<Vertex> Graph::neighbors(Vertex v) const { return storage_->neighbors(v); }
void Graph::remove_edge(Vertex u, Vertex v) { storage_->remove_edge(u, v); }

bool Graph::is_leaf(Vertex v) const { return has_vertex(v) && neighbors(v).size() == 1; }

void Graph::relabel_vertices(const std::unordered_map<Vertex, Vertex>& mapping) {
    std::size_t max_v = 0;
    for (const auto& [from, to] : mapping) {
        if (!has_vertex(from)) throw std::invalid_argument("Invalid source vertex in mapping");
        max_v = std::max(max_v, to);
    }
    Graph relabeled(backend_);
    for (std::size_t i = 0; i <= max_v; ++i) relabeled.add_vertex();
    for (Vertex u = 0; u < vertex_count(); ++u) {
        if (!mapping.contains(u)) continue;
        for (Vertex v : neighbors(u)) {
            if (u < v && mapping.contains(v)) relabeled.add_edge(mapping.at(u), mapping.at(v));
        }
    }
    *this = relabeled;
}

void Graph::union_with(const Graph& other) {
    const std::size_t offset = vertex_count();
    for (std::size_t i = 0; i < other.vertex_count(); ++i) add_vertex();
    for (Vertex u = 0; u < other.vertex_count(); ++u) {
        for (Vertex v : other.neighbors(u)) {
            if (u < v) add_edge(u + offset, v + offset);
        }
    }
}

void Graph::set_backend(Backend backend) {
    if (backend == backend_) return;
    Graph converted(backend);
    for (std::size_t i = 0; i < vertex_count(); ++i) converted.add_vertex();
    for (Vertex u = 0; u < vertex_count(); ++u) {
        for (Vertex v : neighbors(u)) if (u < v) converted.add_edge(u, v);
    }
    *this = std::move(converted);
}

} 
