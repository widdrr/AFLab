#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>

/*
 *		Bonus 3) No Prime Sum
 *
 *			Problema se poate rezolva determinand acoperirea minima de noduri intr-un graph bipartit.
 *		Fie graphul bipartit in care o partitie are numerele pare iar cealalta pe cele impare.
 *		Avem muchii intre numerele a caror suma da un numar prim. Dorim sa eliminam numarul
 *		minim de noduri din acest graph astfel incat sa dispara toate muchiile.
 *
 *			Pentru a realiza acest lucru, aflam cuplajul maxim, iar pentru a decide care dintre
 *		cele doua noduri ale unei muchii din cuplaj trebuie eliminata, parcurgem DFS din toate
 *		nodurile care nu fac parte din cuplaj din prima partitie. Nodurile ce trebuie eliminate
 *		sunt nodurile nevizitate din prima partitie si cele vizitate din a doua partitie.
 *
 *			Complexitatea este O(N*M^2),  pentru cuplaj unde M e greu de determinat si
 *		O(N^2) pentru a crea graphul.
 *
 */

using namespace std;

class Graph {
private:
	int _nodes;
	unordered_map<int, unordered_map<int, int>> _residual_graph;
	unordered_map<int, int> _bfs_tree;
	unordered_set<int> _visited;

	bool BFS(const int start, const int dest) {

		queue<int> node_queue;
		_visited.clear();
		_bfs_tree.clear();

		_bfs_tree[start] = -1;
		_visited.insert(start);
		node_queue.push(start);

		while (!node_queue.empty()) {
			const int node = node_queue.front();
			node_queue.pop();

			for (auto& edge : _residual_graph[node]) {

				if (_visited.find(edge.first) == _visited.end() && edge.second > 0) {
					_visited.insert(edge.first);
					_bfs_tree[edge.first] = node;
					node_queue.push(edge.first);
				}
			}
		}
		return _visited.find(dest) != _visited.end();
	}
	void DFS(const int start) {
		_visited.insert(start);

		for (auto& edge : _residual_graph[start]) {
			if (edge.first != 0 && _visited.find(edge.first) == _visited.end() && edge.second == 1) {
				DFS(edge.first);
			}
		}
	}
public:
	Graph(int nodes, unordered_map<int, unordered_map<int, int>>& adj_list) :
		_nodes(nodes),
		_residual_graph(move(adj_list))
	{
	}

	int maxFlow(const int source, const int dest) {
		int max_flux = 0;
		while (BFS(source, dest)) {
			for (const auto& edge : _residual_graph[dest]) {

				int node = edge.first;
				if (_residual_graph[node][dest] == 0 || _visited.find(node) == _visited.end())
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
	vector<int> maxCovering() {

		unordered_set<int> matched;
		vector<int> numbers;
		maxFlow(0, -2);
		for (const auto& even : _residual_graph[0]) {
			int i = even.first;
			for (const auto& odd : _residual_graph[i]) {
				int j = odd.first;
				if (j != 0 && _residual_graph[i][j] == 0) {
					matched.insert(i);
					matched.insert(j);
				}
			}

		}
		_visited.clear();
		for (auto& edge : _residual_graph[0]) {
			int node = edge.first;
			if (matched.find(node) == matched.end() && _visited.find(node) == _visited.end()) {
				DFS(node);
			}
		}
		for (auto& bruh : _residual_graph) {
			auto node = bruh.first;
			if (node == 0 || node == -2)
				continue;
			if (node % 2 == 0 && _visited.find(node) == _visited.end())
				numbers.push_back(node);
			if (node % 2 == 1 && _visited.find(node) != _visited.end())
				numbers.push_back(node);
		}
		return numbers;
	}
};

bool isPrime(int num) {
	for (int i = 2; i * i <= num; ++i) {
		if (num % i == 0)
			return false;
	}
	return true;
}

// int main()
// {
// 	int nodes;
// 	cin >> nodes;
// 	vector<int> evens;
// 	vector<int> odds;
// 	for (int i = 0; i < nodes; ++i) {
// 		int num;
// 		cin >> num;
// 		if (num % 2 == 0)
// 			evens.push_back(num);
// 		else
// 			odds.push_back(num);
// 	}
//
// 	unordered_map<int, unordered_map<int, int>> adj_list(nodes + 2);
// 	for (auto& even : evens) {
// 		for (auto& odd : odds) {
// 			if (isPrime(even + odd)) {
// 				adj_list[0][even] = 1;
// 				adj_list[even][0] = 0;
// 				adj_list[even][odd] = 1;
// 				adj_list[odd][even] = 0;
// 				adj_list[odd][-2] = 1;
// 				adj_list[-2][odd] = 0;
// 			}
// 		}
// 	}
// 	Graph matching_time(nodes, adj_list);
// 	auto sol = matching_time.maxCovering();
// 	cout << sol.size() << "\n";
// 	for (auto& num : sol) {
// 		cout << num << " ";
// 	}
// 	return 0;
// }