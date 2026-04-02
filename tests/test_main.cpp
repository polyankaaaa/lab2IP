#include "generators.hpp"
#include "metrics.hpp"
#include "parsers.hpp"
#include "serializers.hpp"

#include <cassert>
#include <sstream>

using namespace gd4;

int main() {
    {
        Graph g;
        for (int i = 0; i < 5; ++i) g.add_vertex();
        assert(g.add_edge(0, 1));
        assert(g.has_edge(1, 0));
        assert(!g.add_edge(0, 1));
        assert(g.edge_count() == 1);
        assert(g.is_leaf(0));
    }
     {
        auto g = GraphGenerator::complete_graph(6);
        GraphMetrics m(g);
        assert(g.vertex_count() == 6);
        assert(g.edge_count() == 15);
        assert(m.connected_components_count() == 1);
        assert(m.is_bipartite() == false);
    }
    {
        auto g = GraphGenerator::path_graph(8);
        GraphMetrics m(g);
        assert(m.bridges_count() == 7);
        assert(m.articulation_points_count() == 6);
        assert(m.is_bipartite());
    }
    {
        std::istringstream in("4\n0 1\n1 2\n2 3\n");
        auto g = EdgeListParser().parse(in);
        assert(g.vertex_count() == 4);
        assert(g.edge_count() == 3);
    }
    {
        auto g = GraphGenerator::cycle_graph(5);

        std::ostringstream out_edges;
        EdgesSerializer().serialize(g, out_edges);

        std::string expected_edges =
            "5 5\n"
            "0 1\n"
            "0 4\n"
            "1 2\n"
            "2 3\n"
            "3 4\n";

        assert(out_edges.str() == expected_edges);
    }
    {
        auto g = GraphGenerator::cycle_graph(4);

        GraphVizOptions opt;
        opt.highlighted_cycle = {0, 1, 2, 3, 0};

        std::ostringstream out_dot;
        GraphVizSerializer(opt).serialize(g, out_dot);
        std::string s = out_dot.str();

        assert(s.find("graph G") != std::string::npos);
        assert(s.find("0;") != std::string::npos);
        assert(s.find("1;") != std::string::npos);
        assert(s.find("2;") != std::string::npos);
        assert(s.find("3;") != std::string::npos);

        assert(s.find("0 -- 1 [color=red, penwidth=2.0];") != std::string::npos);
        assert(s.find("1 -- 2 [color=red, penwidth=2.0];") != std::string::npos);
        assert(s.find("2 -- 3 [color=red, penwidth=2.0];") != std::string::npos);
        assert(s.find("0 -- 3 [color=red, penwidth=2.0];") != std::string::npos);
    }
    {
        auto g = GraphGenerator::path_graph(4);

        GraphVizOptions opt;
        opt.highlighted_tree_edges = {0, 1, 1, 2};

        std::ostringstream out_dot;
        GraphVizSerializer(opt).serialize(g, out_dot);
        std::string s = out_dot.str();

        assert(s.find("0 -- 1 [color=blue, penwidth=2.0];") != std::string::npos);
        assert(s.find("1 -- 2 [color=blue, penwidth=2.0];") != std::string::npos);
    }
    return 0;
}