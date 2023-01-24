#include <fstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <vector>

using namespace std;

/*
 *		Bonus 1) Transport2
 *
 *		Modificam algoritmul lui Dijkstra in urmatorul fel:
 *			-stocam pentru fiecare nod masa maxima care poate ajunge in el
 *			-folosim un max heap si alegem mereu muchia cu masa maxima
 *			-consideram costul unei muchii nu prin suma ci prin minim
 *			 (daca pana in nodul x and ajuns cu masa 7 si din x -> y e masa 4, in y vom ajunge cu masa 4)
 *
 *		Complexitatea este O(M*logN).
 *
 */

class Graph {

private:
	vector<vector<pair<int, int>>> _repr;
	int _nodes;
	static constexpr int _infinity = numeric_limits<int>::max();
	static bool _pairGreater(pair<int, int> edge1, pair<int, int> edge2) {
		return edge1.second < edge2.second;
	}

public:
	Graph(int nodes, vector<vector<pair<int, int>>>& p_repr) :
		_repr(move(p_repr)),
		_nodes(nodes)
	{}

	int modDijkstra(const int source, const int dest) {
		priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&_pairGreater)> edge_heap(&_pairGreater);
		vector<int> costs(_nodes);

		for (int node = 1; node < _nodes; ++node) {
			costs[node] = 0;
		}

		costs[source] = _infinity;
		edge_heap.emplace(source, _infinity);

		while (!edge_heap.empty()) {

			const auto crt_edge = edge_heap.top();
			edge_heap.pop();
			const auto crt_node = crt_edge.first;
			const auto crt_cost = crt_edge.second;

			if (crt_node == dest)
				return costs[dest];

			if (crt_cost != costs[crt_node])
				continue;

			for (const auto& edge : _repr[crt_node]) {

				const auto node = edge.first;
				auto cost = edge.second;

				cost = min(crt_cost, cost);

				if (cost > costs[node]) {

					edge_heap.emplace(node, cost);
					costs[node] = cost;
				}
			}
		}
		return -1;
	}
};


// int main()
// {
// 	ifstream in("transport2.in");
// 	int n, m;
// 	in >> n >> m;
//
// 	vector<vector<pair<int, int>>> repr(n + 1);
//
// 	for (int i = 0; i < m; ++i) {
// 		int node1, node2, cost;
// 		in >> node1 >> node2 >> cost;
// 		repr[node1].emplace_back(node2, cost);
// 		repr[node2].emplace_back(node1, cost);
// 	}
// 	in.close();
//
// 	Graph test(n + 1, repr);
//
// 	ofstream out("transport2.out");
// 	out << test.modDijkstra(1, n);
// 	out.close();
//
// 	return 0;
// }