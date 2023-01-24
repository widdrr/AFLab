#include <fstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <vector>
#include <limits>

using namespace std;

/*
 *		Bonus 6) DBZ
 *
 *			Incepem prin face un Dijkstra multisource din toti vecinii nodului de start, iar pentru fiecare nod
 *		stocam nodul de incep al drumului care se duce in el. Rezultatul este distanta minima pana la fiecare nod
 *		din toti vecinii startului, si din ce vecin se ajunge.
 *			Folosind aceasta informatie, luam toate perechile de noduri (mai putin startul) care au muchie
 *		si care au nod de incepu diferit si formam un ciclu adunand distantele si costul muchiei dintre ele.
 *			Rezultatul final este maximul acestor cicluri.
 *
 *			Complexitatea este O(MlogN + M + N).
 */

class Graph {

private:
	vector<vector<pair<int, int>>> _repr;
	int _nodes;
	vector<int> costs;
	vector<int> father;

	static constexpr int _infinity = numeric_limits<int>::max();
	static bool _pairGreater(pair<int, int> edge1, pair<int, int> edge2) {
		return edge1.second > edge2.second;
	}
	void modDijkstra(const int source) {
		priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&_pairGreater)> edge_heap(&_pairGreater);

		for(int i = 1; i < _nodes; ++i){
			costs[i] = _infinity;
			father[i] = -1;
		}
		costs[source] = 0;

		for(auto& edge : _repr[source]){
			const auto node = edge.first;
			const auto cost = edge.second;
			edge_heap.emplace(node, cost);
			costs[node] = cost;
			father[node] = node;
		}

		while (!edge_heap.empty()) {

			const auto crt_edge = edge_heap.top();
			edge_heap.pop();
			const auto crt_node = get<0>(crt_edge);
			const auto crt_cost = get<1>(crt_edge);

			if (crt_cost > costs[crt_node])
				continue;

			for (const auto& edge : _repr[crt_node]) {

				const auto node = edge.first;
				auto cost = edge.second;
				if (cost + crt_cost < costs[node]) {

					edge_heap.emplace(node, cost + crt_cost);
					costs[node] = cost + crt_cost;
					father[node] = father[crt_node];
				}
			}
		}
	}

public:
	Graph(int nodes, vector<vector<pair<int, int>>>& p_repr) :
		_repr(move(p_repr)),
		_nodes(nodes),
		costs(nodes),
		father(nodes)
	{}

	int goku(const int source) {

		int min_dist = _infinity;
		modDijkstra(source);
		for (int i = 1; i < _nodes; ++i)
		{
			if (i == source)
				continue;

			for(auto& edge : _repr[i]){
				auto node = edge.first;
				auto cost = edge.second;

				if(father[i] == father[node] || 
				  (father[i] == i && node==source))
					continue;
				int potential = costs[i] + cost + costs[node];
				
				min_dist = min(min_dist, potential);

			}
		}
		return min_dist == _infinity ? -1 : min_dist;
	}
};


//int main()
//{
//	ifstream in("dbz.in");
//	int n, m;
//	in >> n >> m;
//
//	vector<vector<pair<int, int>>> repr(n + 1);
//
//	for (int i = 0; i < m; ++i) {
//		int node1, node2, cost;
//		in >> node1 >> node2 >> cost;
//		repr[node1].emplace_back(node2, cost);
//		repr[node2].emplace_back(node1, cost);
//	}
//	in.close();
//
//	Graph son(n + 1, repr);
//
//	ofstream out("dbz.out");
//	for(int node = 1; node <=n;++node){
//		out << son.goku(node) << " ";
//	}
//	out.close();
//
//	return 0;
//}