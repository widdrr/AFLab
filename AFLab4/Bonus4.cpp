#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>

/*
 *		Bonus 4) Flipping Matrices
 *
 *			In primul rand, daca exista o solutie atunci ea este posibila doar prin swapuri de linii sau coloane.
 *		Trebuie sa gasim o permutare de linii care sa formeze diagonala principala.
 *
 *			Consideram matricea primita drept un graph bipartit: linii si coloane, cu muchie acolo unde avem 1.
 *		Daca facem cuplajul maxim (cu flux), obtinem pentru fiecare coloana, pe ce linie se afla un 1 pe care
 *		l-am putea muta, si prin urmare intre ce linii trebuie sa facem swap.
 *
 *			Complexitatea este cea a cuplajului prin flux maxim, si anume O(N*M^2)
 *
 */


using namespace std;

class Graph {
private:
	int _nodes, _edges;
	vector<unordered_map<int, int>> _residual_graph;
	vector<int> _bfs_tree;
	vector<bool> _visited;

	bool BFS(const int start, const int dest) {

		queue<int> node_queue;
		_visited = vector<bool>(_nodes);
		_bfs_tree = vector<int>(_nodes);

		_bfs_tree[start] = -1;
		_visited[start] = true;
		node_queue.push(start);

		while (!node_queue.empty()) {
			const int node = node_queue.front();
			node_queue.pop();

			for (auto& edge : _residual_graph[node]) {

				if (!_visited[edge.first] && edge.second > 0) {
					_visited[edge.first] = true;
					_bfs_tree[edge.first] = node;
					node_queue.push(edge.first);
				}
			}
		}
		return _visited[dest];
	}
public:
	Graph(int nodes, int edges, vector<unordered_map<int, int>>& adj_list) :
		_nodes(nodes),
		_edges(edges),
		_residual_graph(move(adj_list))
	{
		for (int i = 0; i < _nodes; ++i) {
			for (const auto& edge : _residual_graph[i]) {
				if (_residual_graph[edge.first].find(i) == _residual_graph[edge.first].end())
					_residual_graph[edge.first][i] = 0;
			}
		}
	}

	int maxFlow(const int source, const int dest) {
		int max_flux = 0;
		while (BFS(source, dest)) {
			for (const auto& edge : _residual_graph[dest]) {

				int node = edge.first;
				if (_residual_graph[node][dest] == 0 || !_visited[node])
					continue;

				int flux = numeric_limits<int>::max();
				flux = min(flux, _residual_graph[node][dest]);

				while (_bfs_tree[node] != -1) {
					const int father = _bfs_tree[node];
					flux = min(flux, _residual_graph[father][node]);
					if (flux == 0)
						break;
					node = father;
				}

				if (flux == 0)
					continue;
				max_flux += flux;

				node = edge.first;
				_residual_graph[node][dest] -= flux;
				_residual_graph[dest][node] += flux;
				while (_bfs_tree[node] != -1) {
					const int father = _bfs_tree[node];
					_residual_graph[father][node] -= flux;
					_residual_graph[node][father] += flux;
					node = father;
				}
			}
		}
		return max_flux;
	}
	vector<pair<int, int>> maxMatching(const int n) {

		vector<pair<int, int>> edges;
		int value = 0;
		maxFlow(0, _nodes - 1);
		for (int i = 1; i <= n; ++i) {
			for (const auto& edge : _residual_graph[i]) {
				int j = edge.first;
				if (j != 0 && _residual_graph[i][edge.first] == 0) {
					edges.emplace_back(i, j - n);
					++value;
				}
			}

		}
		return edges;
	}
};

// int main()
// {
// 	int n;
// 	cin >> n;
//
// 	vector<unordered_map<int, int>> adj_list(2 * n + 2);
// 	int val;
// 	for (int i = 1; i <= n; ++i) {
// 		for (int j = 1; j <= n; ++j) {
// 			cin >> val;
// 			if (val == 1) {
// 				adj_list[i][j + n] = 1;
// 			}
// 		}
// 	}
//
// 	for (int i = 1; i <= n; ++i) {
// 		adj_list[0][i] = 1;
// 		adj_list[i + n][2 * n + 1] = 1;
// 	}
//
// 	Graph matching_time(2 * n + 2, 42, adj_list);
//
//
// 	auto sol = matching_time.maxMatching(n);
// 	if (sol.size() != n) {
// 		cout << -1 << "\n";
// 	}
// 	else {
// 		vector<int> perm(n + 1);
// 		vector<int> bruh_perm(n + 1);
//
// 		for (auto& edge : sol) {
// 			perm[edge.first] = edge.second;
// 		}
// 		for (int node = 1; node <= n; ++node) {
// 			if (perm[node] != node) {
// 				auto found = find(perm.begin() + 1, perm.end(), node);
// 				size_t pos = found - perm.begin();
// 				cout << "R " << node << " " << pos << "\n";
// 				swap(perm[node], perm[pos]);
// 			}
// 		}
// 	}
//
// 	return 0;
// }
