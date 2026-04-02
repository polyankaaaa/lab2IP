CXX ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Iinclude

LIB_SRC = src/graph.cpp src/algorithms.cpp src/parsers.cpp src/serializers.cpp src/generators.cpp src/metrics.cpp
LIB_OBJ = $(LIB_SRC:.cpp=.o)

all: graph_cli graph_tests

graph_cli: $(LIB_OBJ) app/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

graph_tests: $(LIB_OBJ) tests/test_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test: graph_tests
	./graph_tests

clean:
	rm -f $(LIB_OBJ) app/main.o tests/test_main.o graph_cli graph_tests