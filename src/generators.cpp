#include "generators.hpp"

#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

namespace gd4 {

namespace {
Graph make_n(std::size_t n) {
    Graph g;
    for (std::size_t i = 0; i < n; ++i) g.add_vertex();
    return g;
}
}  // namespace

Graph GraphGenerator::complete_graph(std::size_t n) {
    Graph g = make_n(n);
    for (Vertex u = 0; u < n; ++u) for (Vertex v = u + 1; v < n; ++v) g.add_edge(u, v);
    return g;
}

Graph GraphGenerator::complete_bipartite_graph(std::size_t n, std::size_t m) {
    Graph g = make_n(n + m);
    for (Vertex u = 0; u < n; ++u) for (Vertex v = n; v < n + m; ++v) g.add_edge(u, v);
    return g;
}

Graph GraphGenerator::tree_graph(std::size_t n, unsigned seed) {
    Graph g = make_n(n);
    if (n <= 1) return g;
    std::mt19937 rng(seed);
    for (Vertex v = 1; v < n; ++v) {
        std::uniform_int_distribution<Vertex> pick(0, v - 1);
        g.add_edge(v, pick(rng));
    }
    return g;
}

Graph GraphGenerator::star_graph(std::size_t n) {
    Graph g = make_n(n);
    for (Vertex v = 1; v < n; ++v) g.add_edge(0, v);
    return g;
}

Graph GraphGenerator::cycle_graph(std::size_t n) {
    Graph g = make_n(n);
    if (n < 3) return g;
    for (Vertex v = 0; v < n; ++v) g.add_edge(v, (v + 1) % n);
    return g;
}

Graph GraphGenerator::path_graph(std::size_t n) {
    Graph g = make_n(n);
    for (Vertex v = 1; v < n; ++v) g.add_edge(v - 1, v);
    return g;
}

Graph GraphGenerator::wheel_graph(std::size_t n) {
    if (n < 4) throw std::invalid_argument("wheel requires n >= 4");
    Graph g = make_n(n);
    for (Vertex v = 1; v < n; ++v) {
        g.add_edge(0, v);
        g.add_edge(v, v == n - 1 ? 1 : v + 1);
    }
    return g;
}

Graph GraphGenerator::erdos_renyi(std::size_t n, double p, unsigned seed) {
    Graph g = make_n(n);
    std::mt19937 rng(seed);
    std::bernoulli_distribution edge(p);
    for (Vertex u = 0; u < n; ++u) {
        for (Vertex v = u + 1; v < n; ++v) if (edge(rng)) g.add_edge(u, v);
    }
    return g;
}

Graph GraphGenerator::random_cubic(std::size_t n, unsigned seed) {
    if (n % 2 != 0 || n < 4) throw std::invalid_argument("cubic graph requires even n >= 4");
    Graph g = make_n(n);
    std::mt19937 rng(seed);
    std::vector<Vertex> stubs;
    for (Vertex v = 0; v < n; ++v) for (int d = 0; d < 3; ++d) stubs.push_back(v);
    for (int tries = 0; tries < 1000; ++tries) {
        Graph cur = make_n(n);
        auto tmp = stubs;
        std::shuffle(tmp.begin(), tmp.end(), rng);
        bool ok = true;
        for (std::size_t i = 0; i < tmp.size(); i += 2) {
            if (!cur.add_edge(tmp[i], tmp[i + 1])) {
                ok = false;
                break;
            }
        }
        if (ok) return cur;
    }
    return g;
}

Graph GraphGenerator::with_components(std::size_t n, std::size_t components, unsigned seed) {
    if (components == 0 || components > n) throw std::invalid_argument("invalid component count");
    Graph g = make_n(n);
    std::mt19937 rng(seed);
    std::vector<std::size_t> sizes(components, 1);
    for (std::size_t i = components; i < n; ++i) sizes[rng() % components]++;
    Vertex base = 0;
    for (std::size_t s : sizes) {
        for (Vertex v = 1; v < s; ++v) g.add_edge(base + v - 1, base + v);
        base += s;
    }
    return g;
}

Graph GraphGenerator::with_bridges(std::size_t n, std::size_t bridges, unsigned seed) {
    Graph g = path_graph(n);
    std::mt19937 rng(seed);
    while (g.edge_count() > bridges && n > 1) {
        Vertex e = rng() % (n - 1);
        g.remove_edge(e, e + 1);
    }
    return g;
}

Graph GraphGenerator::with_articulation_points(std::size_t n, std::size_t points, unsigned) {
    if (n < points + 1) throw std::invalid_argument("n is too small");
    Graph g = make_n(n);
    for (Vertex v = 1; v < points + 1; ++v) g.add_edge(v - 1, v);
    for (Vertex v = points + 1; v < n; ++v) g.add_edge(points, v);
    return g;
}

Graph GraphGenerator::with_two_bridges(std::size_t n, std::size_t two_bridges, unsigned) {
    if (n < 4) return path_graph(n);
    Graph g = cycle_graph(n);
    for (std::size_t i = 0; i < two_bridges && i + 3 < n; ++i) {
        g.remove_edge(i, i + 1);
        g.add_edge(i, i + 2);
    }
    return g;
}

Graph GraphGenerator::halin_graph(std::size_t leaf_cycle_size) {
    if (leaf_cycle_size < 4) leaf_cycle_size = 4;
    Graph g = make_n(leaf_cycle_size + 1);
    for (Vertex v = 1; v <= leaf_cycle_size; ++v) {
        g.add_edge(0, v);
        g.add_edge(v, v == leaf_cycle_size ? 1 : v + 1);
    }
    return g;
}

}  
