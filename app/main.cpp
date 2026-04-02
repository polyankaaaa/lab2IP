#include "algorithms.hpp"
#include "generators.hpp"
#include "metrics.hpp"
#include "parsers.hpp"
#include "serializers.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using namespace gd4;

namespace {
void print_usage() {
    std::cout << "Usage:\n"
              << "  graph_cli generate <kind> [args]\n"
              << "  graph_cli parse <format> <input-file>\n";
}
Graph generate_by_name(const std::string& kind, int argc, char** argv) {
    if (kind == "kn" && argc >= 4) return GraphGenerator::complete_graph(std::stoull(argv[3]));
    if (kind == "knm" && argc >= 5) return GraphGenerator::complete_bipartite_graph(std::stoull(argv[3]), std::stoull(argv[4]));
    if (kind == "tree" && argc >= 4) return GraphGenerator::tree_graph(std::stoull(argv[3]));
    if (kind == "star" && argc >= 4) return GraphGenerator::star_graph(std::stoull(argv[3]));
    if (kind == "cycle" && argc >= 4) return GraphGenerator::cycle_graph(std::stoull(argv[3]));
    if (kind == "path" && argc >= 4) return GraphGenerator::path_graph(std::stoull(argv[3]));
    if (kind == "wheel" && argc >= 4) return GraphGenerator::wheel_graph(std::stoull(argv[3]));
    if (kind == "gnp" && argc >= 5) return GraphGenerator::erdos_renyi(std::stoull(argv[3]), std::stod(argv[4]));
    if (kind == "cubic" && argc >= 4) return GraphGenerator::random_cubic(std::stoull(argv[3]));
    if (kind == "components" && argc >= 5) return GraphGenerator::with_components(std::stoull(argv[3]), std::stoull(argv[4]));
    if (kind == "bridges" && argc >= 5) return GraphGenerator::with_bridges(std::stoull(argv[3]), std::stoull(argv[4]));
    if (kind == "artpoints" && argc >= 5) return GraphGenerator::with_articulation_points(std::stoull(argv[3]), std::stoull(argv[4]));
    if (kind == "two_bridges" && argc >= 5) return GraphGenerator::with_two_bridges(std::stoull(argv[3]), std::stoull(argv[4]));
    if (kind == "halin" && argc >= 4) return GraphGenerator::halin_graph(std::stoull(argv[3]));
    throw std::invalid_argument("bad generate args");
}

void print_metrics(const Graph& g) {
    GraphMetrics m(g);
    std::cout << "n=" << g.vertex_count() << " m=" << g.edge_count() << "\n";
    std::cout << "density=" << m.density() << "\n";
    std::cout << "diameter=" << m.diameter() << "\n";
    std::cout << "transitivity=" << m.transitivity() << "\n";
    std::cout << "components=" << m.connected_components_count() << "\n";
    std::cout << "articulation_points=" << m.articulation_points_count() << "\n";
    std::cout << "bridges=" << m.bridges_count() << "\n";
    std::cout << "bipartite=" << (m.is_bipartite() ? "yes" : "no") << "\n";
    std::cout << "greedy_chromatic_upper_bound=" << m.greedy_chromatic_upper_bound() << "\n";
}
}  
int main(int argc, char** argv) {
    try {
        if (argc < 3) {
            print_usage();
            return 1;
        }

        Graph g;
        const std::string mode = argv[1];
        if (mode == "generate") {
            g = generate_by_name(argv[2], argc, argv);
        } else if (mode == "parse" && argc >= 4) {
            std::ifstream in(argv[3]);
            if (!in) {
                std::cerr << "cannot open input file\n";
                return 2;
            }
            g = create_parser(argv[2])->parse(in);
        } else {
            print_usage();
            return 1;
        }
        print_metrics(g);

        GraphVizSerializer dot({.highlighted_cycle = random_cycle(g), .highlighted_tree_edges = random_spanning_tree_edges(g)});
        std::ofstream("graph.dot") << [&]() {
            std::ostringstream oss;
            dot.serialize(g, oss);
            return oss.str();
        }();
        std::ofstream edges("graph.edges");
        EdgesSerializer().serialize(g, edges);
        std::cout << "written: graph.dot, graph.edges\n";
    } catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 3;
    }
    return 0;
}
