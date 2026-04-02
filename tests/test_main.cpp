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