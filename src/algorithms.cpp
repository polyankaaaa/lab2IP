#include "algorithms.hpp"

#include <algorithm>
#include <functional>
#include <queue>
#include <random>
#include <stack>
#include <unordered_map>

namespace gd4 {

void depth_first_search(const Graph& graph, DfsVisitor& visitor) {
    const std::size_t n = graph.vertex_count();
    std::vector<int> color(n, 0);
    std::function<void(Vertex, Vertex)> dfs = [&](Vertex u, Vertex parent) {
        color[u] = 1;
        visitor.discover_vertex(u);
        for (Vertex v : graph.neighbors(u)) {
            if (v == parent) continue;
            if (color[v] == 0) {
                visitor.tree_edge(u, v);
                dfs(v, u);
            } else if (color[v] == 1) {
                visitor.back_edge(u, v);
            }
        }
        color[u] = 2;
        visitor.finish_vertex(u);
    };
    for (Vertex v = 0; v < n; ++v) if (color[v] == 0) dfs(v, v);
}

std::vector<std::vector<Vertex>> connected_components(const Graph& graph) {
    const std::size_t n = graph.vertex_count();
    std::vector<bool> used(n, false);
    std::vector<std::vector<Vertex>> out;
    for (Vertex s = 0; s < n; ++s) {
        if (used[s]) continue;
        std::queue<Vertex> q;
        q.push(s);
        used[s] = true;
        out.emplace_back();
        while (!q.empty()) {
            const Vertex u = q.front();
            q.pop();
            out.back().push_back(u);
            for (Vertex v : graph.neighbors(u)) {
                if (!used[v]) {
                    used[v] = true;
                    q.push(v);
                }
            }
        }
    }
    return out;
}

std::vector<Vertex> random_spanning_tree_edges(const Graph& graph, unsigned seed) {
    std::mt19937 rng(seed);
    const auto comps = connected_components(graph);
    std::vector<Vertex> packed_edges;
    for (const auto& comp : comps) {
        if (comp.empty()) continue;
        std::unordered_map<Vertex, bool> seen;
        std::vector<Vertex> order = comp;
        std::shuffle(order.begin(), order.end(), rng);
        std::queue<Vertex> q;
        q.push(order.front());
        seen[order.front()] = true;
        while (!q.empty()) {
            const Vertex u = q.front();
            q.pop();
            auto neigh = graph.neighbors(u);
            std::shuffle(neigh.begin(), neigh.end(), rng);
            for (Vertex v : neigh) {
                if (seen[v]) continue;
                seen[v] = true;
                q.push(v);
                packed_edges.push_back(u);
                packed_edges.push_back(v);
            }
        }
    }
    return packed_edges;
}

std::vector<Vertex> random_cycle(const Graph& graph, unsigned seed) {
    std::mt19937 rng(seed);
    std::vector<Vertex> vertices(graph.vertex_count());
    for (Vertex v = 0; v < vertices.size(); ++v) vertices[v] = v;
    std::shuffle(vertices.begin(), vertices.end(), rng);

    std::vector<int> color(graph.vertex_count(), 0);
    std::vector<Vertex> parent(graph.vertex_count(), static_cast<Vertex>(-1));
    std::vector<Vertex> cycle;

    std::function<bool(Vertex)> dfs = [&](Vertex u) {
        color[u] = 1;
        auto neigh = graph.neighbors(u);
        std::shuffle(neigh.begin(), neigh.end(), rng);
        for (Vertex v : neigh) {
            if (parent[u] == v) continue;
            if (color[v] == 0) {
                parent[v] = u;
                if (dfs(v)) return true;
            } else if (color[v] == 1) {
                cycle.push_back(v);
                for (Vertex x = u; x != v; x = parent[x]) cycle.push_back(x);
                cycle.push_back(v);
                return true;
            }
        }
        color[u] = 2;
        return false;
    };

    for (Vertex s : vertices) if (color[s] == 0 && dfs(s)) break;
    std::reverse(cycle.begin(), cycle.end());
    return cycle;
}

}
