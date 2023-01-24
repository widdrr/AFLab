#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>


/*
 *		Bonus 1) Flux Maxim de Cost Minim
 *
 *			Problema este aproape la fel cu determinarea fluxului maxim, cu exceptia ca vom dori
 *		sa alegem drumurile de crestere in ordinea crescatoare a costurilor. Costurile pot fi si negative.
 *
 *			Algoritmul Bellman-Ford ar fi prea lent, dar algoritmul Dijkstra nu ar functiona in mod normal
 *		pe costuri negative. Pentru asta, vom aplica o transformare asupra costurilor muchiilor astfel
 *		incat toate sa fie pozitive, dar una care sa nu schimbe ordinea in care acestea ar fi alese.
 *		Aceasta transformare va fi: new_cost = dist[src] + old_cost - dist[dest],
 *		unde dist este distanta reala fata de sursa retelei de flux.
 *
 *			Distanta reala se poate calcula aplicand la inceput Bellman-Ford, dar deoarece dupa fiecare
 *		ameliorare de drum exista sansa ca un nod sa devina inaccesibil, si prin urmare sa se modifice
 *		distantele reale, vom calcula mereu la fiecare iteratie de dijkstra noul set de distante
 *		reale ce va fi folosit la urmatoarea iteratie.
 *
 *			Complexitatea este O(N*M^2*logN), cu N-noduri, M-muchii
 *
 */
using namespace std;

class Graph {
private:
	int _nodes, _edges;
	vector<unordered_map<int, pair<int, int>>> _residual_graph;
	vector<int> _path;
	vector<int> _costs;
	vector<int> _new_costs;
	vector<int> _real_costs;
	vector<bool> _in_queue;

	bool dijkstra(const int source, const int dest) {

		_costs = vector<int>(_nodes + 1, numeric_limits<int>::max());
		_path = vector<int>(_nodes + 1, 0);
		_new_costs = vector<int>(_nodes + 1, numeric_limits<int>::max());

		//first element is distance, second is node
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> edge_heap;

		_costs[source] = 0;
		_new_costs[source] = 0;

		edge_heap.emplace(0, source);

		while (!edge_heap.empty()) {
			auto crt_node = edge_heap.top().second;
			auto cost = edge_heap.top().first;

			edge_heap.pop();

			//if it's an outdated entry, skip it
			if(cost != _costs[crt_node])
				continue;

			for (auto& edge : _residual_graph[crt_node]) {

				const auto capacity = edge.second.second;
				if (capacity > 0) {
					auto node = edge.first;
					auto modified_dist = edge.second.first + _real_costs[crt_node] - _real_costs[node];

					if (_costs[node] > _costs[crt_node] + modified_dist) {
						_path[node] = crt_node;
						_costs[node] = _costs[crt_node] + modified_dist;
						_new_costs[node] = _new_costs[crt_node] + edge.second.first;
						edge_heap.emplace(_costs[node], node);
					}
				}
			}
		}

		for(int node = 1; node <= _nodes; ++node){
			_real_costs[node] = _new_costs[node];
		}
		return _costs[dest] != numeric_limits<int>::max();
	}

	void compute_real_costs(const int source) {

		_real_costs = vector<int>(_nodes + 1, numeric_limits<int>::max());
		_in_queue = vector<bool>(_nodes + 1);
		queue<int> node_queue;

		node_queue.push(source);
		_real_costs[source] = 0;
		_in_queue[source] = true;

		while (!node_queue.empty()) {
			auto crt_node = node_queue.front();
			node_queue.pop();
			_in_queue[crt_node] = false;

			for (auto& edge : _residual_graph[crt_node]) {
				auto node = edge.first;
				auto cost = edge.second.first;

				if (_real_costs[node] > _real_costs[crt_node] + cost) {
					_real_costs[node] = _real_costs[crt_node] + cost;

					if (!_in_queue[node]) {
						node_queue.push(node);
						_in_queue[node] = true;
					}
				}
			}
		}

	}

public:
	Graph(int nodes, int edges, vector<unordered_map<int, pair<int, int>>>& adj_list, int source) :
		_nodes(nodes),
		_edges(edges),
		_residual_graph(move(adj_list))
	{
		compute_real_costs(source);
		for (int i = 1; i <= _nodes; ++i) {
			for (const auto& edge : _residual_graph[i]) {

				if (_residual_graph[edge.first].find(i) == _residual_graph[edge.first].end()) {
					//returning edge's cost should be -original edge, to cancel out and prevent negative cycles
					_residual_graph[edge.first][i].first = -_residual_graph[i][edge.first].first;
					_residual_graph[edge.first][i].second = 0;
				}
			}
		}
	}

	int minCostOfMaxFlow(const int source, const int dest) {
		int min_cost = 0;
		while (dijkstra(source, dest)) {

			int flux = numeric_limits<int>::max();
			int node = dest;

			while (_path[node] != 0) {
				const int father = _path[node];
				flux = min(flux, _residual_graph[father][node].second);
				node = father;
			}

			node = dest;
			while (_path[node] != 0) {
				const int father = _path[node];
				min_cost += flux * _residual_graph[father][node].first;

				_residual_graph[father][node].second -= flux;
				_residual_graph[node][father].second += flux;
				node = father;
			}
		}
		return min_cost;
	}
};

// int main()
// {
// 	ifstream in("fmcm.in");
//
// 	int nodes, edges, source, dest;;
// 	in >> nodes >> edges >> source >> dest;
//
// 	vector<unordered_map<int, pair<int,int>>> adj_list(nodes + 1);
// 	for(int i=0; i<edges; ++i){
// 		int node1, node2, capacity, cost;
// 		in >> node1 >> node2 >> capacity >> cost;
// 		adj_list[node1][node2] = make_pair(cost,capacity);
// 	}
// 	in.close();
//
// 	Graph flux_time(nodes, edges, adj_list,source);
//
// 	ofstream out("fmcm.out");
// 	out << flux_time.minCostOfMaxFlow(source, dest);
// 	out.close();
// }