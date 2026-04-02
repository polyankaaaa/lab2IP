#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace gd4 {

using Vertex = std::size_t;

class IGraphStorage {
public:
    virtual ~IGraphStorage() = default;
    virtual std::unique_ptr<IGraphStorage> clone() const = 0;
    virtual Vertex add_vertex() = 0;
    virtual bool has_vertex(Vertex v) const = 0;
    virtual std::size_t vertex_count() const = 0;
    virtual bool add_edge(Vertex u, Vertex v) = 0;
    virtual bool has_edge(Vertex u, Vertex v) const = 0;
    virtual std::size_t edge_count() const = 0;
    virtual std::vector<Vertex> neighbors(Vertex v) const = 0;
    virtual void remove_edge(Vertex u, Vertex v) = 0;
};

class AdjacencyListStorage final : public IGraphStorage {
public:
    std::unique_ptr<IGraphStorage> clone() const override;
    Vertex add_vertex() override;
    bool has_vertex(Vertex v) const override;
    std::size_t vertex_count() const override;
    bool add_edge(Vertex u, Vertex v) override;
    bool has_edge(Vertex u, Vertex v) const override;
    std::size_t edge_count() const override;
    std::vector<Vertex> neighbors(Vertex v) const override;
    void remove_edge(Vertex u, Vertex v) override;

private:
    std::vector<std::unordered_set<Vertex>> adj_;
    std::size_t edge_count_{0};
};

class AdjacencyMatrixStorage final : public IGraphStorage {
public:
    std::unique_ptr<IGraphStorage> clone() const override;
    Vertex add_vertex() override;
    bool has_vertex(Vertex v) const override;
    std::size_t vertex_count() const override;
    bool add_edge(Vertex u, Vertex v) override;
    bool has_edge(Vertex u, Vertex v) const override;
    std::size_t edge_count() const override;
    std::vector<Vertex> neighbors(Vertex v) const override;
    void remove_edge(Vertex u, Vertex v) override;

private:
    std::vector<std::vector<bool>> matrix_;
    std::size_t edge_count_{0};
};

class Graph {
public:
    enum class Backend { AdjacencyList, AdjacencyMatrix };

    explicit Graph(Backend backend = Backend::AdjacencyList);
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    Graph(Graph&&) noexcept = default;
    Graph& operator=(Graph&&) noexcept = default;

    Vertex add_vertex();
    bool has_vertex(Vertex v) const;
    std::size_t vertex_count() const;
    bool add_edge(Vertex u, Vertex v);
    bool has_edge(Vertex u, Vertex v) const;
    std::size_t edge_count() const;
    std::vector<Vertex> neighbors(Vertex v) const;
    bool is_leaf(Vertex v) const;
    void relabel_vertices(const std::unordered_map<Vertex, Vertex>& mapping);
    void union_with(const Graph& other);
    void set_backend(Backend backend);
    Backend backend() const { return backend_; }
    void remove_edge(Vertex u, Vertex v);

private:
    std::unique_ptr<IGraphStorage> storage_;
    Backend backend_;
};

}  
