#include "metrics.hpp"

#include "algorithms.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_set>
#include <vector>

namespace gd4 {

double GraphMetrics::density() {
    if (density_cache_) return *density_cache_;
    const auto n = static_cast<double>(graph_.vertex_count());
    density_cache_ = n < 2.0 ? 0.0 : (2.0 * static_cast<double>(graph_.edge_count())) / (n * (n - 1.0));
    return *density_cache_;
}

std::size_t GraphMetrics::diameter() {
    if (diameter_cache_) return *diameter_cache_;
    const std::size_t n = graph_.vertex_count();
    std::size_t diam = 0;
    for (Vertex s = 0; s < n; ++s) {
        std::vector<int> dist(n, -1);
        std::queue<Vertex> q;
        q.push(s);
        dist[s] = 0;
        while (!q.empty()) {
            const Vertex u = q.front();
            q.pop();
            for (Vertex v : graph_.neighbors(u)) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        for (int d : dist) if (d > static_cast<int>(diam)) diam = static_cast<std::size_t>(d);
    }
    diameter_cache_ = diam;
    return diam;
}

double GraphMetrics::transitivity() {
    if (transitivity_cache_) return *transitivity_cache_;
    double triangles3 = 0.0;
    double triads = 0.0;
    for (Vertex u = 0; u < graph_.vertex_count(); ++u) {
        const auto neigh = graph_.neighbors(u);
        const auto deg = neigh.size();
        triads += static_cast<double>(deg) * static_cast<double>(deg - 1) / 2.0;
        for (std::size_t i = 0; i < neigh.size(); ++i) {
            for (std::size_t j = i + 1; j < neigh.size(); ++j) {
                if (graph_.has_edge(neigh[i], neigh[j])) triangles3 += 1.0;
            }
        }
    }
    transitivity_cache_ = triads == 0.0 ? 0.0 : triangles3 / triads;
    return *transitivity_cache_;
}

std::size_t GraphMetrics::connected_components_count() {
    if (cc_cache_) return *cc_cache_;
    cc_cache_ = connected_components(graph_).size();
    return *cc_cache_;
}

std::size_t GraphMetrics::articulation_points_count() {
    if (articulation_cache_) return *articulation_cache_;
    const std::size_t n = graph_.vertex_count();
    std::vector<int> tin(n, -1), low(n, -1);
    std::vector<bool> vis(n, false), is_cut(n, false);
    int timer = 0;
    std::function<void(Vertex, Vertex)> dfs = [&](Vertex u, Vertex p) {
        vis[u] = true;
        tin[u] = low[u] = timer++;
        int children = 0;
        for (Vertex v : graph_.neighbors(u)) {
            if (v == p) continue;
            if (vis[v]) {
                low[u] = std::min(low[u], tin[v]);
            } else {
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (low[v] >= tin[u] && p != u) is_cut[u] = true;
                ++children;
            }
        }
        if (p == u && children > 1) is_cut[u] = true;
    };
    for (Vertex v = 0; v < n; ++v) if (!vis[v]) dfs(v, v);
    articulation_cache_ = std::count(is_cut.begin(), is_cut.end(), true);
    return *articulation_cache_;
}

std::size_t GraphMetrics::bridges_count() {
    if (bridges_cache_) return *bridges_cache_;
    const std::size_t n = graph_.vertex_count();
    std::vector<int> tin(n, -1), low(n, -1);
    std::vector<bool> vis(n, false);
    std::size_t bridges = 0;
    int timer = 0;
    std::function<void(Vertex, Vertex)> dfs = [&](Vertex u, Vertex p) {
        vis[u] = true;
        tin[u] = low[u] = timer++;
        for (Vertex v : graph_.neighbors(u)) {
            if (v == p) continue;
            if (vis[v]) {
                low[u] = std::min(low[u], tin[v]);
            } else {
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > tin[u]) ++bridges;
            }
        }
    };
    for (Vertex v = 0; v < n; ++v) if (!vis[v]) dfs(v, v);
    bridges_cache_ = bridges;
    return *bridges_cache_;
}

bool GraphMetrics::is_bipartite() {
    if (bipartite_cache_) return *bipartite_cache_;
    std::vector<int> color(graph_.vertex_count(), -1);
    bool ok = true;
    for (Vertex s = 0; s < graph_.vertex_count() && ok; ++s) {
        if (color[s] != -1) continue;
        std::queue<Vertex> q;
        q.push(s);
        color[s] = 0;
        while (!q.empty() && ok) {
            Vertex u = q.front();
            q.pop();
            for (Vertex v : graph_.neighbors(u)) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;
                    q.push(v);
                } else if (color[v] == color[u]) {
                    ok = false;
                    break;
                }
            }
        }
    }
    bipartite_cache_ = ok;
    return ok;
}

std::size_t GraphMetrics::greedy_chromatic_upper_bound() {
    if (chromatic_cache_) return *chromatic_cache_;
    std::vector<Vertex> order(graph_.vertex_count());
    for (Vertex v = 0; v < order.size(); ++v) order[v] = v;
    std::sort(order.begin(), order.end(), [&](Vertex a, Vertex b) {
        return graph_.neighbors(a).size() > graph_.neighbors(b).size();
    });

    std::vector<int> color(graph_.vertex_count(), -1);
    int max_color = -1;
    for (Vertex u : order) {
        std::unordered_set<int> used;
        for (Vertex v : graph_.neighbors(u)) if (color[v] != -1) used.insert(color[v]);
        int c = 0;
        while (used.contains(c)) ++c;
        color[u] = c;
        max_color = std::max(max_color, c);
    }
    chromatic_cache_ = static_cast<std::size_t>(max_color + 1);
    return *chromatic_cache_;
}

}  
