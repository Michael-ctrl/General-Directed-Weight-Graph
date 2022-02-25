#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <istream>

TEST_CASE("Iterator tests") {
	SECTION("iterator begin test") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
		CHECK(g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "b", 1));
		CHECK(!g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "b", 3));
		CHECK(g.insert_edge("b", "c", 2));
		auto i = g.begin();
		auto i1 = g.begin();
		CHECK(i == i1);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 1);
		i++;
		i1++;
		CHECK(i == i1);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 2);
		i++;
		i1++;
		CHECK(i == i1);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 3);
		i++;
		i1++;
		CHECK(i == i1);
		CHECK((*i).from == "b");
		CHECK((*i).to == "c");
		CHECK((*i).weight == 2);
		i++;
		i1++;
		CHECK(i == i1);
		CHECK(i == g.end());
	}
	SECTION("iterator end test") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
		CHECK(g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "b", 1));
		CHECK(!g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "b", 3));
		CHECK(g.insert_edge("b", "c", 2));
		auto i = g.end();
		auto i2 = g.end();
		i--;
		i2--;
		CHECK(i == i2);
		CHECK((*i).from == "b");
		CHECK((*i).to == "c");
		CHECK((*i).weight == 2);
		i--;
		i2--;
		CHECK(i == i2);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 3);
		i--;
		i2--;
		CHECK(i == i2);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 2);
		i--;
		i2--;
		CHECK(i == i2);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 1);
		CHECK(i == g.begin());
		i++;
		i2++;
		CHECK(i == i2);
		CHECK((*i).from == "a");
		CHECK((*i).to == "b");
		CHECK((*i).weight == 2);
		auto i1 = g.begin();
		auto i3 = g.begin();
		CHECK(i1 == i3);
	}
	SECTION("Iterator remove edge") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
		CHECK(g.insert_edge("a", "b", 3));
		CHECK(g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "c", 1));
		auto it = g.begin();
		auto i2 = g.begin();
		i2++;
		CHECK(g.erase_edge(it) == i2);
	}
	SECTION("Iterator remove range edge") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
		CHECK(g.insert_edge("a", "b", 3));
		CHECK(g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "b", 4));
		CHECK(g.insert_edge("a", "c", 1));
		// [a,b] {2, 3, 4}
		auto it = g.begin();
		auto i2 = g.begin();
		i2++;
		i2++; // [a,b] = 4
		CHECK(g.erase_edge(it, i2) == i2);
	}
	SECTION("Iterator remove range edge") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
		CHECK(g.insert_edge("a", "b", 3));
		CHECK(g.insert_edge("a", "b", 2));
		CHECK(g.insert_edge("a", "b", 4));
		CHECK(g.insert_edge("a", "c", 1));
		g.clear();
		CHECK(g.empty());
	}
}
