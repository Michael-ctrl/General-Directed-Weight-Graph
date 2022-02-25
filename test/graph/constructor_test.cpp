#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <istream>

TEST_CASE("Constructor tests") {
	SECTION("Default Constructor Test") {
		SECTION("Ints") {
			auto g = gdwg::graph<int, int>();
			CHECK(g.empty());
			CHECK(g.nodes().empty());
			auto g2 = gdwg::graph<int, int>{3, 1, 2};
			CHECK(!g2.empty());
			CHECK(g2.nodes() == std::vector<int>{1, 2, 3});
		}

		SECTION("Strings") {
			auto g = gdwg::graph<std::string, std::string>();
			CHECK(g.empty());
			CHECK(g.nodes().empty());
			auto g2 = gdwg::graph<std::string, std::string>{"a", "c", "b"};
			CHECK(!g2.empty());
			CHECK(g2.nodes() == std::vector<std::string>{"a", "b", "c"});
		}
	}
	SECTION("Initializer List") {
		auto li = std::initializer_list<int>{2, 1, 3};
		auto li_2 = std::initializer_list<int>{4, 2, 5};

		auto g = gdwg::graph<int, int>(li);
		auto expected_g = gdwg::graph<int, int>{3, 1, 2};
		auto diff_g = gdwg::graph<int, int>(li_2);
		CHECK(g == expected_g);
		CHECK(!(g == diff_g));
	}

	SECTION("Iterator Initialisation") {
		SECTION("Basic full vector initialisation") {
			auto vec = std::vector<int>{1, 2, 3, 5, 4};
			auto g = gdwg::graph<int, int>(vec.begin(), vec.end());
			auto g_copy = gdwg::graph<int, int>{1, 2, 3, 4, 5};
			CHECK(g == g_copy);
		}
		SECTION("Partial range initialisation") {
			auto vec = std::vector<int>{1, 2, 3, 5, 4};
			auto it_1 = vec.begin();
			it_1++;
			auto it_2 = vec.begin();
			it_2++;
			it_2++;
			it_2++;
			it_2++;
			auto g = gdwg::graph<int, int>(it_1, it_2); // [2, 4)
			auto g_copy = gdwg::graph<int, int>{2, 3, 5};
			CHECK(g == g_copy);
		}
	}

	SECTION("Move Constructors") {
		SECTION("Move constructor") {
			auto g = gdwg::graph<int, int>{1, 2, 3, 4};
			CHECK(g.is_node(1));
			CHECK(g.is_node(2));
			CHECK(g.is_node(3));
			CHECK(g.is_node(4));

			auto move_g(std::move(g));
			CHECK(move_g.is_node(1));
			CHECK(move_g.is_node(2));
			CHECK(move_g.is_node(3));
			CHECK(move_g.is_node(4));
		}
		SECTION("Move assignment") {
			auto g = gdwg::graph<int, int>{1, 2, 3, 4};
			CHECK(g.is_node(1));
			CHECK(g.is_node(2));
			CHECK(g.is_node(3));
			CHECK(g.is_node(4));

			auto move_g = gdwg::graph<int, int>(std::move(g));
			CHECK(move_g.is_node(1));
			CHECK(move_g.is_node(2));
			CHECK(move_g.is_node(3));
			CHECK(move_g.is_node(4));
		}
		SECTION("Move Compare") {
			auto g = gdwg::graph<int, int>{1, 2, 3, 4};
			auto g1 = gdwg::graph<int, int>{1, 2, 3, 4};

			auto move_g1 = gdwg::graph<int, int>(std::move(g));
			auto move_g2 = std::move(g1);
			CHECK(move_g1 == move_g2);
		}
	}

	SECTION("Copy Constructors") {
		SECTION("Copy constructor") {
			auto g = gdwg::graph<int, int>{1, 2, 3, 4};
			CHECK(g.is_node(1));
			CHECK(g.is_node(2));
			CHECK(g.is_node(3));
			CHECK(g.is_node(4));

			auto copy = gdwg::graph<int, int>(g);
			CHECK(copy.is_node(1));
			CHECK(copy.is_node(2));
			CHECK(copy.is_node(3));
			CHECK(copy.is_node(4));
			CHECK(g == copy);
		}
		SECTION("Copy assignment") {
			auto g = gdwg::graph<int, int>{1, 2, 3, 4};
			CHECK(g.is_node(1));
			CHECK(g.is_node(2));
			CHECK(g.is_node(3));
			CHECK(g.is_node(4));

			auto copy = g; // NOLINTLINE
			CHECK(copy.is_node(1));
			CHECK(copy.is_node(2));
			CHECK(copy.is_node(3));
			CHECK(copy.is_node(4));

			CHECK(g == copy);
		}
		SECTION("Copy Compare") {
			auto g = gdwg::graph<int, int>{1, 2, 3, 4};
			auto copy = gdwg::graph<int, int>(g); // NOLINTLINE
			auto copy2 = g; // NOLINTLINE
			CHECK(copy == copy2);
		}
	}
}
