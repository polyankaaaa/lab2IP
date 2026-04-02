#pragma once

#include "graph.hpp"

#include <istream>
#include <memory>
#include <string>

namespace gd4 {

class IGraphParser {
public:
    virtual ~IGraphParser() = default;
    virtual Graph parse(std::istream& in) const = 0;
};

class EdgeListParser final : public IGraphParser {
public:
    Graph parse(std::istream& in) const override;
};

class AdjacencyMatrixParser final : public IGraphParser {
public:
    Graph parse(std::istream& in) const override;
};

class DimacsColoringParser final : public IGraphParser {
public:
    Graph parse(std::istream& in) const override;
};

class SnapParser final : public IGraphParser {
public:
    Graph parse(std::istream& in) const override;
};

std::unique_ptr<IGraphParser> create_parser(const std::string& format);

}  
