#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

TEST_CASE("Extractor tests") {
	SECTION("Same graph extractor test") {
		using graph = gdwg::graph<int, int>;
		auto const v = std::vector<graph::value_type>{
		   {4, 1, -4},
		   {3, 2, 2},
		   {2, 4, 2},
		   {2, 1, 1},
		   {6, 2, 5},
		   {6, 3, 10},
		   {1, 5, -1},
		   {3, 6, -8},
		   {4, 5, 3},
		   {5, 2, 7},
		};

		auto g = graph{};
		for (const auto& [from, to, weight] : v) {
			g.insert_node(from);
			g.insert_node(to);
			g.insert_edge(from, to, weight);
		};

		g.insert_node(64);
		auto out = std::ostringstream{};
		out << g;

		auto copy = graph{};
		for (const auto& [from, to, weight] : v) {
			copy.insert_node(from);
			copy.insert_node(to);
			copy.insert_edge(from, to, weight);
		};

		copy.insert_node(64);
		auto copy_out = std::ostringstream{};
		copy_out << copy;

		CHECK(out.str() == copy_out.str()); // output should be the same since same graph
	}
	SECTION("Test provided by spec") {
		using graph = gdwg::graph<int, int>;
		auto const v = std::vector<graph::value_type>{
		   {4, 1, -4},
		   {3, 2, 2},
		   {2, 4, 2},
		   {2, 1, 1},
		   {6, 2, 5},
		   {6, 3, 10},
		   {1, 5, -1},
		   {3, 6, -8},
		   {4, 5, 3},
		   {5, 2, 7},
		};

		auto g = graph{};
		for (const auto& [from, to, weight] : v) {
			g.insert_node(from);
			g.insert_node(to);
			g.insert_edge(from, to, weight);
		};

		g.insert_node(64);
		auto out = std::ostringstream{};
		out << g;
		auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
		CHECK(out.str() == expected_output);
	}

	SECTION("Expected output extractor test") {}
}
