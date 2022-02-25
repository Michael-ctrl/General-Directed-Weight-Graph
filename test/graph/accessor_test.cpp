#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <istream>
#include <vector>

TEST_CASE("Accessors tests") {
	SECTION("is_node tests") {
		auto g = gdwg::graph<int, int>{1, 2, 3};
		CHECK(g.insert_node(5));
		CHECK(g.insert_node(4));
		CHECK(g.is_node(1));
		CHECK(g.is_node(4));
		CHECK(g.is_node(3));
		CHECK(!g.is_node(6));
	}

	SECTION("empty test") {
		auto vec = std::vector<int>{};
		auto li = std::initializer_list<int>{};
		auto g = gdwg::graph<int, int>{};
		auto g1 = gdwg::graph<int, int>(li);
		auto g2 = gdwg::graph<int, int>(vec.begin(), vec.end());
		CHECK(g.empty());
		CHECK(g1.empty());
		CHECK(g2.empty());
	}

	SECTION("is_connected test") {
		auto g = gdwg::graph<int, int>{1, 2, 3};
		CHECK(g.insert_edge(1, 2, 3));
		CHECK(g.insert_edge(1, 2, 2));
		CHECK(g.insert_edge(1, 1, 3));
		CHECK(g.is_connected(1, 2)); // an edge
		CHECK(!g.is_connected(1, 3)); // not an edge
		CHECK(g.is_connected(1, 1)); // always true if there's self-directed edge
		CHECK(!g.is_connected(2, 1)); // directed g so false
	}

	SECTION("nodes tests") {
		SECTION("non-const graph") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_node(5));
			CHECK(g.insert_node(4));
			auto vec = std::vector<int>{1, 2, 3, 4, 5};
			auto g_vec = g.nodes();
			CHECK(g_vec == vec);
			vec[0] = 3;
			CHECK(vec == std::vector<int>{3, 2, 3, 4, 5});
		}

		SECTION("const graph") {
			auto const g = gdwg::graph<int, int>{3, 2, 1};
			auto vec = std::vector<int>{1, 2, 3};
			auto g_vec = g.nodes();
			CHECK(g_vec == vec);
			vec[0] = 3;
			CHECK(vec == std::vector<int>{3, 2, 3});
		}
	}

	SECTION("weights test") {
		auto g = gdwg::graph<int, int>{1, 2, 3};
		CHECK(g.insert_edge(1, 2, 3));
		CHECK(g.insert_edge(1, 2, 2));
		CHECK(g.insert_edge(1, 2, -4));
		CHECK(g.insert_edge(1, 2, 20));
		CHECK(g.insert_edge(2, 1, 20));
		CHECK(g.insert_edge(1, 1, 3));
		CHECK(g.weights(1, 2) == std::vector<int>{-4, 2, 3, 20});
	}

	SECTION("find test") {
		auto g = gdwg::graph<int, int>{1, 2, 3};
		CHECK(g.insert_edge(1, 2, 3));
		CHECK(g.insert_edge(1, 2, 2));
		CHECK(g.insert_edge(1, 1, 3));
		auto result = g.find(1, 2, 2);
		auto it = g.begin();
		it++;
		CHECK(result == it);
	}

	SECTION("connections test") {
		auto g = gdwg::graph<int, int>{1, 2, 3};
		CHECK(g.insert_edge(1, 2, 3));
		CHECK(g.insert_edge(1, 2, 2));
		CHECK(g.insert_edge(1, 1, 3));
		CHECK(g.connections(1) == std::vector<int>{1, 2});
	}
}
