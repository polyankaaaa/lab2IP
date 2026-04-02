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