#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <tuple>
#include <utility>
#include <vector>
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		class iterator;
		struct value_type {
			N from;
			N to;
			E weight;
		};

		template<typename T>
		struct sorter {
			using is_transparent = void;
			//    ~~~~~~~~~~~~~^
			auto operator()(T const& lhs, T const& rhs) const -> bool {
				return *lhs < *rhs;
			}

			auto operator()(std::shared_ptr<T> const& a, std::shared_ptr<T> const& b) const -> bool {
				return *a < *b;
			}

			auto operator()(std::pair<std::shared_ptr<T>, std::shared_ptr<T>> const& lhs,
			                std::pair<std::shared_ptr<T>, std::shared_ptr<T>> const& rhs) const -> bool {
				if (*lhs.first < *rhs.first) {
					return true;
				}
				if (*lhs.first > *rhs.first) {
					return false;
				}
				return *lhs.second < *rhs.second;
			}

			auto operator()(std::shared_ptr<T> const& lhs, T const& rhs) const -> bool {
				return *lhs < rhs;
			}

			auto operator()(T const& lhs, std::shared_ptr<T> const& rhs) const -> bool {
				return lhs < *rhs;
			}
		};

		////////////////
		// Constructors
		////////////////
		graph()
		: nodes_{std::set<std::shared_ptr<N>, sorter<N>>()}
		, edges_{std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
		                  std::set<std::shared_ptr<E>, sorter<E>>,
		                  sorter<N>>()} {}

		graph(std::initializer_list<N> il)
		: nodes_{std::set<std::shared_ptr<N>, sorter<N>>()}
		, edges_{std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
		                  std::set<std::shared_ptr<E>, sorter<E>>,
		                  sorter<N>>()} {
			std::for_each(il.begin(), il.end(), [this](auto& n) { this->insert_node(n); });
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: nodes_{std::set<std::shared_ptr<N>, sorter<N>>()}
		, edges_{std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
		                  std::set<std::shared_ptr<E>, sorter<E>>,
		                  sorter<N>>()} {
			std::for_each(first, last, [this](auto& n) { this->insert_node(n); });
		}

		graph(graph&& other) noexcept {
			this->nodes_.clear();
			this->edges_.clear();
			this->nodes_.merge(other.nodes_);
			this->edges_.merge(other.edges_);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(this->nodes_, other.nodes_);
			std::swap(this->edges_, other.edges_);
			other.nodes_.clear();
			other.edges_.clear();
			return *this;
		}

		graph(graph const& other)
		: nodes_{std::set(other.nodes_)}
		, edges_{std::map(other.edges_)} {}

		auto operator=(graph const& other) -> graph& {
			if (*this == other) {
				return *this;
			}
			auto temp = other;
			this->nodes_ = std::set(temp.nodes_);
			this->edges_ = std::map(temp.edges_);
			return *this;
		}

		////////////////
		// Modifiers
		////////////////
		auto insert_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}
			this->nodes_.insert(std::make_shared<N>(value));
			return true;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}
			auto new_edge = std::make_shared<E>(weight);
			auto edge_set = this->edges_.find(std::make_pair(*nodes_.find(src), *nodes_.find(dst)));
			if (edge_set != this->edges_.end()) {
				if (is_edge(*nodes_.find(src), *nodes_.find(dst), weight)) {
					return false;
				}
			}
			else {
				this->edges_.insert({std::make_pair(*nodes_.find(src), *nodes_.find(dst)),
				                     std::set<std::shared_ptr<E>, sorter<E>>()});
				edge_set = this->edges_.find(std::make_pair(*nodes_.find(src), *nodes_.find(dst)));
			}
			edge_set->second.emplace(new_edge);
			return true;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (not is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			if (is_node(new_data)) {
				return false;
			}
			auto new_node = std::make_shared<N>(new_data);
			this->nodes_.emplace(new_node);
			auto new_edges = std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
			                          std::set<std::shared_ptr<E>, sorter<E>>,
			                          sorter<N>>();
			for (auto const& [key, val] : edges_) {
				if (*key.first == old_data && *key.second == old_data) {
					auto const nk = std::pair(new_node, new_node);
					new_edges.emplace(nk, val);
				}
				else if (*key.first == old_data) {
					auto const nk = std::pair(new_node, key.second);
					new_edges.emplace(nk, val);
				}
				else if (*key.second == old_data) {
					auto const nk = std::pair(key.first, new_node);
					new_edges.emplace(nk, val);
				}
			}
			erase_node(old_data);
			this->edges_.merge(new_edges);
			return true;
		};

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}
			if (old_data != new_data) { // no point changing graph if it's replaced by the same thing
				auto new_node = std::make_shared<N>(new_data);
				auto new_edges = std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
				                          std::set<std::shared_ptr<E>, sorter<E>>,
				                          sorter<N>>();
				for (auto const& [key, val] : edges_) {
					if (*key.first == old_data && *key.second == old_data) {
						auto const nk = std::pair(new_node, new_node);
						new_edges.emplace(nk, val);
					}
					else if (*key.first == old_data) {
						auto const nk = std::pair(new_node, key.second);
						new_edges.emplace(nk, val);
					}
					else if (*key.second == old_data) {
						auto const nk = std::pair(key.first, new_node);
						new_edges.emplace(nk, val);
					}
				}
				erase_node(old_data);
				this->edges_.merge(new_edges);
			}
		}

		auto erase_node(N const& value) -> bool {
			if (!is_node(value)) {
				return false;
			}

			for (auto it = this->edges_.begin(); it != this->edges_.end();) {
				auto key = it->first;
				if (*key.first == value || *key.second == value) {
					it = edges_.erase(it);
				}
				else {
					it++;
				}
			}
			this->nodes_.erase(std::make_shared<N>(value));
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
			auto edge_set =
			   this->edges_.find(std::pair(*this->nodes_.find(src), *this->nodes_.find(dst)));
			auto edge = edge_set->second.find(std::make_shared<E>(weight));
			if (edge != edge_set->second.end()) {
				edge_set->second.erase(edge);
				if (edge_set->second.empty()) {
					edges_.erase(std::make_pair(*this->nodes_.find(src), *this->nodes_.find(dst)));
				}
				return true;
			}

			return false;
		}

		auto erase_edge(iterator i) -> iterator {
			auto temp = i;
			temp++;
			if (temp == this->end()) {
				auto outer_iter = i.outer_;
				auto inner_iter = i.inner_;
				auto hack = edges_.erase(outer_iter, outer_iter);
				if (outer_iter->second.size() == 1) {
					hack->second.erase(inner_iter);
					edges_.erase(hack->first);
				}
				else {
					hack->second.erase(inner_iter);
				}
				return this->end();
			}
			auto outer_iter = i.outer_;
			auto inner_iter = i.inner_;
			auto hack = edges_.erase(outer_iter, outer_iter);
			if (outer_iter->second.size() == 1) {
				hack->second.erase(inner_iter);
				edges_.erase(hack->first);
			}
			else {
				hack->second.erase(inner_iter);
			}
			return temp;
		}

		auto erase_edge(iterator i, iterator s) -> iterator {
			auto temp = i;
			while (temp != s) {
				temp = erase_edge(temp);
			}
			return temp;
		}

		auto clear() noexcept -> void {
			this->nodes_.clear();
			this->edges_.clear();
		}

		////////////////
		// Accessors
		////////////////
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			return this->nodes_.find(value) != this->nodes_.end();
		}

		[[nodiscard]] auto empty() const noexcept -> bool {
			return static_cast<bool>(this->nodes_.empty());
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			return static_cast<bool>(
			   this->edges_.count(std::pair(*this->nodes_.find(src), *this->nodes_.find(dst))));
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto result = std::vector<N>();
			for (auto const& node : nodes_) {
				result.push_back(*node);
			}
			return result;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			auto result = std::vector<E>();
			auto edge_set =
			   this->edges_.find(std::pair(*this->nodes_.find(src), *this->nodes_.find(dst)));
			if (edge_set == edges_.end() || edge_set->second.empty()) {
				return result;
			}
			for (auto edge : edge_set->second) {
				result.push_back(*edge);
			}
			return result;
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			for (auto it = this->begin(); it != this->end(); it++) {
				if (((*it).from == src) && ((*it).to == dst) && ((*it).weight == weight)) {
					return it;
				}
			}
			return this->end();
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			auto result = std::vector<N>();
			for (auto const& [key, value] : edges_) {
				if (*key.first == src) {
					result.push_back(*key.second);
				}
			}

			return result;
		}

		////////////////
		// Iterator Access
		////////////////
		[[nodiscard]] auto begin() const -> iterator {
			if (edges_.empty()) {
				return iterator(edges_.end(), edges_.begin(), {});
			}
			return iterator(edges_.end(), edges_.begin(), edges_.begin()->second.begin());
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator(edges_.end(), edges_.end(), {});
		}

		////////////////
		// Comparisons
		////////////////
		// Heyo, I know this is disgusting. Yes rip style marks but I can't think of any other way :(
		// Hooray for 1 liner tho :^)
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			return this->nodes() == other.nodes()
			       && std::equal(this->edges_.begin(),
			                     this->edges_.end(),
			                     other.edges_.begin(),
			                     [](auto const& edge, auto const& edge_o) {
				                     auto [key, edges] = edge;
				                     auto [key_o, edges_o] = edge_o;
				                     if (*key.first != *key_o.first) { // src of edge isn't the same
					                     return false;
				                     }
				                     return std::equal(edges.begin(),
				                                       edges.end(),
				                                       edges_o.begin(),
				                                       [](auto const w, auto const w_o) {
					                                       return *w == *w_o;
				                                       }); // check weights are the same
			                     });
		}

		////////////////
		// Extractor
		////////////////
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			for (auto src : g.nodes()) {
				os << src << " (" << std::endl;
				for (auto dst : g.connections(src)) {
					for (auto weight : g.weights(src, dst)) {
						os << "  " << dst << " | " << weight << std::endl;
					}
				}
				os << ")" << std::endl;
			}
			return os;
		}

		////////////////
		// Iterators
		////////////////
		// Referenced from ropes tutorial example
		class iterator {
		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			// Iterator constructor
			iterator() = default;

			// Iterator source
			auto operator*() -> reference {
				return {*outer_->first.first, *outer_->first.second, **inner_};
			}

			// Iterator traversal
			auto operator++() -> iterator& {
				while (outer_ != last_) {
					if (inner_ != outer_->second.end()) {
						++inner_;
						if (inner_ != outer_->second.end()) {
							return *this;
						}
					}
					++outer_;
					if (outer_ != last_) {
						inner_ = outer_->second.begin();
						return *this;
					}
				}
				inner_ = inner_t{};
				return *this;
			}
			auto operator++(int) -> iterator {
				auto old = *this;
				++*this;
				return old;
			}
			auto operator--() -> iterator& {
				if (inner_ == inner_t{}) {
					--outer_;
					inner_ = outer_->second.end();
					--inner_;
					return *this;
				}

				if (inner_ != outer_->second.begin() && inner_ != inner_t{}) { // --end()
					--inner_;
					return *this;
				}
				--outer_;
				while (outer_->second.empty()) {
					--outer_;
				}
				inner_ = outer_->second.end();
				--inner_;

				return *this;
			}
			auto operator--(int) -> iterator {
				auto old = *this;
				--*this;
				return old;
			}

			// Iterator comparison
			auto operator==(iterator const& other) const -> bool {
				return (inner_ == other.inner_) and (outer_ == other.outer_);
			}

		private:
			using outer_t = typename std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
			                                  std::set<std::shared_ptr<E>, sorter<E>>,
			                                  sorter<N>>::const_iterator;
			using inner_t = typename std::set<std::shared_ptr<E>>::const_iterator;

			outer_t last_{};
			outer_t outer_{};
			inner_t inner_{};

			explicit iterator(outer_t last, outer_t outer, inner_t inner)
			: last_(last)
			, outer_(outer)
			, inner_(inner) {}

			friend class graph;
		};

	private:
		std::set<std::shared_ptr<N>, sorter<N>> nodes_;
		std::map<std::pair<std::shared_ptr<N>, std::shared_ptr<N>>,
		         std::set<std::shared_ptr<E>, sorter<E>>,
		         sorter<N>>
		   edges_;

		auto is_edge(std::shared_ptr<N> src, std::shared_ptr<N> dst, E weight) -> bool {
			for (auto const& it : this->edges_[std::make_pair(src, dst)]) {
				if (*it.get() == weight) {
					return true;
				}
			}
			return false;
		}
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
