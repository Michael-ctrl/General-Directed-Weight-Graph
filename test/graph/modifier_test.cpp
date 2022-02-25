#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <istream>
#include <stdexcept>

TEST_CASE("Modifier Tests") {
	SECTION("Insert Node Tests") {
		SECTION("Insert New Nodes") { // Graph with nodes inserted should equal expected graph
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_node(5));
			CHECK(g.insert_node(4));
			auto expected_g = gdwg::graph<int, int>{1, 2, 3, 4, 5};
			CHECK(g == expected_g);
		}

		SECTION("Insert Same Nodes") { // Graph with nodes inserted should equal expected graph
			                            // despite attempted same node insertion
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_node(5));
			CHECK(g.insert_node(4));
			CHECK(!g.insert_node(5));
			CHECK(!g.insert_node(2));
			auto expected_g = gdwg::graph<int, int>{1, 2, 3, 4, 5};
			CHECK(g == expected_g);
		}
	}

	SECTION("Insert Edge Tests") {
		SECTION("Src/Dst does not exist") {
			auto g = gdwg::graph<std::string, int>{"1", "2", "3"};
			auto copy_g = g;
			CHECK_THROWS_AS(g.insert_edge("non_existent", "2", 3), std::runtime_error);
			CHECK_THROWS_AS(g.insert_edge("1", "non_existent", 2), std::runtime_error);
			CHECK_THROWS_AS(g.insert_edge("non_existent", "non_existent", 3),
			                std::runtime_error); // insert edge to non-existent source to itself
			CHECK(g == copy_g); // ensure nothing has been inserted
		}
		SECTION("Insert New Edges") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_edge(1, 2, 3));
			CHECK(g.insert_edge(1, 2, 2));
			CHECK(g.insert_edge(1, 1, 3)); // insert edge to itself

			auto g_copy = gdwg::graph<int, int>{2, 1, 3};
			CHECK(g_copy.insert_edge(1, 1, 3)); // insert edge to itself
			CHECK(g_copy.insert_edge(1, 2, 2));
			CHECK(g_copy.insert_edge(1, 2, 3));

			CHECK(g == g_copy);
		}

		SECTION("Insert Same Edges") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_edge(1, 2, 3));
			CHECK(!g.insert_edge(1, 2, 3));
			CHECK(g.insert_edge(1, 2, 2));
			CHECK(!g.insert_edge(1, 2, 2));

			auto expected_g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(expected_g.insert_edge(1, 2, 3));
			CHECK(expected_g.insert_edge(1, 2, 2));

			CHECK(g == expected_g); // ensure inserting same edge did nothing to graph g
		}
	}

	SECTION("Replace Node Tests") {
		SECTION("Old node does not exist") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			auto copy = g; // nolintline
			CHECK_THROWS_AS(g.replace_node(4, 5), std::runtime_error);
			CHECK(g == copy);
		}
		SECTION("New node already exists") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			auto copy = g; // nolintline
			CHECK(!g.replace_node(1, 3));
			CHECK(g == copy);
		}
		SECTION("Replace new node with another new node") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.replace_node(1, 4));
			auto new_g = gdwg::graph<int, int>{4, 2, 3};
			CHECK(g == new_g);
		}
		SECTION("Replace new node with another new node with existing edges") {
			auto g = gdwg::graph<int, int>{1, 2, 3};
			CHECK(g.insert_edge(1, 3, 4));
			CHECK(g.insert_edge(1, 1, 4));
			CHECK(g.replace_node(1, 4));
			auto new_g = gdwg::graph<int, int>{4, 2, 3};
			CHECK(new_g.insert_edge(4, 3, 4));
			CHECK(new_g.insert_edge(4, 4, 4));
			CHECK(g == new_g); // check graph is as expected
			CHECK(g.insert_edge(4, 2, 3)); // check you can insert edge from new node to another node
			CHECK(new_g.insert_edge(4, 2, 3));
			CHECK(g == new_g); // check graph is as expected after edge insertion
		}
	}

	SECTION("Erase Edge Tests") {
		SECTION("Erase edge(src, dst, weight)") {
			auto g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(g.insert_edge("A", "B", 3));
			CHECK(g.insert_edge("A", "A", 3));
			CHECK(g.insert_edge("C", "B", 2));
			CHECK(g.insert_edge("D", "B", 4));
			SECTION("Src/Dst are not nodes") {
				CHECK_THROWS_AS(g.erase_edge("E", "B", 3), std::runtime_error);
				CHECK_THROWS_AS(g.erase_edge("A", "E", 3), std::runtime_error);
				CHECK_THROWS_AS(g.erase_edge("E", "E", 3), std::runtime_error);
			}
			CHECK(g.erase_edge("A", "B", 3));
			CHECK(!g.erase_edge("A", "B", 3)); // remove edge after it has already been removed
			CHECK(!g.erase_edge("A", "A", 4)); // not a valid edge

			auto expected_g = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
			CHECK(expected_g.insert_edge("A", "A", 3));
			CHECK(expected_g.insert_edge("C", "B", 2));
			CHECK(expected_g.insert_edge("D", "B", 4));

			CHECK(g == expected_g);
		}
		SECTION("Erase edge(it)") {
			auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
			CHECK(g.insert_edge("a", "b", 3));
			CHECK(g.insert_edge("a", "b", 2));
			CHECK(g.insert_edge("a", "c", 1));
			auto it = g.begin();
			auto it2 = g.begin();
			it2++;
			CHECK(g.erase_edge(it) == it2);
			auto expected_g = gdwg::graph<std::string, int>{"a", "b", "c"};
			CHECK(expected_g.insert_edge("a", "b", 3));
			CHECK(expected_g.insert_edge("a", "c", 1));

			CHECK(g == expected_g);
		}
		SECTION("Erase edge(it range)") {
			auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
			CHECK(g.insert_edge("a", "b", 3));
			CHECK(g.insert_edge("a", "b", 2));
			CHECK(g.insert_edge("a", "c", 1));
			auto it = g.begin();
			auto it2 = g.begin();
			it2++;
			it2++;
			CHECK(g.erase_edge(it, it2) == it2);
			auto expected_g = gdwg::graph<std::string, int>{"a", "b", "c"};
			CHECK(expected_g.insert_edge("a", "c", 1));

			CHECK(g == expected_g);
		}
	}

	SECTION("Clear Test") {
		auto g = gdwg::graph<int, int>{1, 2, 3};
		CHECK(g.insert_edge(1, 2, 2));
		CHECK(g.insert_edge(1, 2, 3));
		CHECK(g.insert_edge(2, 1, 2));
		g.clear();
		CHECK(g.empty());
	}
}
