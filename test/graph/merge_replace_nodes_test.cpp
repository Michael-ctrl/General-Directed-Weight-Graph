
#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <istream>
#include <stdexcept>

TEST_CASE("Modifier Tests") {
	SECTION("Merge Replace Node Tests") {
		SECTION("Old/New node does not exist") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			auto copy = g; // nolintline
			CHECK_THROWS_AS(g.merge_replace_node(1, 4), std::runtime_error);
			CHECK_THROWS_AS(g.merge_replace_node(4, 1), std::runtime_error);
			CHECK(g == copy);
		}
		SECTION("Merge Replace Node") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			g.merge_replace_node(1, 3);
			auto new_g = gdwg::graph<int, int>{2, 3};
			CHECK(g == new_g);
		}

		SECTION("Merge Replace Node with edges") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_edge(1, 3, 4));
			CHECK(g.insert_edge(1, 1, 4));
			g.merge_replace_node(1, 3);
			auto new_g = gdwg::graph<int, int>{2, 3};
			CHECK(new_g.insert_edge(3, 3, 4));
			CHECK(g == new_g); // check graph is as expected
			CHECK(g.insert_edge(3, 2, 3)); // check you can insert edge from new node to another node
			CHECK(new_g.insert_edge(3, 2, 3));
			CHECK(g == new_g); // check graph is as expected after edge insertion
		}
		SECTION("Test 1 from spec") {
			auto g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(g.insert_edge("A", "B", 1));
			CHECK(g.insert_edge("A", "C", 2));
			CHECK(g.insert_edge("A", "D", 3));
			g.merge_replace_node("A", "B");

			CHECK(g.connections("B") == std::vector<std::string>{"B", "C", "D"});
		}
		SECTION("Test 2 from spec") {
			auto g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(g.insert_edge("A", "B", 1));
			CHECK(g.insert_edge("A", "C", 2));
			CHECK(g.insert_edge("A", "D", 3));
			CHECK(g.insert_edge("B", "B", 1));
			g.merge_replace_node("A", "B");

			CHECK(g.connections("B") == std::vector<std::string>{"B", "C", "D"});
		}
		SECTION("Test 3 from spec") {
			auto g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(g.insert_edge("A", "B", 1));
			CHECK(g.insert_edge("A", "C", 2));
			CHECK(g.insert_edge("A", "D", 3));
			g.merge_replace_node("A", "B");

			CHECK(g.connections("B") == std::vector<std::string>{"B", "C", "D"});
		}
		SECTION("Test 4 from spec") {
			auto g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(g.insert_edge("A", "B", 3));
			CHECK(g.insert_edge("C", "B", 2));
			CHECK(g.insert_edge("D", "B", 4));
			g.merge_replace_node("B", "A");

			CHECK(g.connections("A") == std::vector<std::string>{"A"});
			CHECK(g.connections("C") == std::vector<std::string>{"A"});
			CHECK(g.connections("D") == std::vector<std::string>{"A"});
		}
		SECTION("Test 5 from spec") {
			auto g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(g.insert_edge("A", "B", 3));
			CHECK(g.insert_edge("A", "A", 3));
			CHECK(g.insert_edge("C", "B", 2));
			CHECK(g.insert_edge("D", "B", 4));
			g.merge_replace_node("B", "A");

			CHECK(g.connections("A") == std::vector<std::string>{"A"});
			CHECK(g.connections("C") == std::vector<std::string>{"A"});
			CHECK(g.connections("D") == std::vector<std::string>{"A"});
		}
	}
}