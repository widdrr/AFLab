#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>

/*
 *		3) Harta
 *
 *			Pentru a construit un graph orientat date fiind gradele nodurilor, aplicam din nou
 *		fluxul maxim (pentru a determina cuplajul) pe o retea de flux.
 *
 *			Vom construi o retea bipartita in care fiecare partitie are numarul de noduri date.
 *		Exista muchie intre oricare doua noduri din partitii diferite (mai putin corespondentul fiecarui nod).
 *		Toate aceste muchii au capacitate 1. Adaugam o sursa si o destinatie legate de cate o partitie.
 *		Pe muchia de la sursa la un nod, capacitatea va fi gradul de iesire al nodului, iar de la nod la
 *		destinaite va fi gradul sau de intrare.
 *			Muchiile graphului reprezinta cuplajul maxim, adica muchiile care au capacitate 0 la final.
 *
 *			Complexitatea este O(N*M^2), dominata de algoritmul de flux folosit.
 *			
 */

using namespace std;

class Graph {
private:
	int _nodes;
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
	Graph(int nodes, vector<unordered_map<int, int>> adj_list) :
		_nodes(nodes),
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
	vector<pair<int, int>> getGraph(const int nodes) {

		vector<pair<int, int>> edges;

		maxFlow(0, _nodes - 1);

		for (int i = 1; i <= nodes; ++i) {
			for (const auto& edge : _residual_graph[i]) {
				int j = edge.first;
				if (j != 0 && _residual_graph[i][edge.first] == 0) {
					edges.emplace_back(i, j - nodes);
				}
			}
		}
		return edges;

	}
};

// int main()
// {
// 	ifstream in("harta.in");
//
// 	int nodes;
// 	in >> nodes;
//
// 	int internal_nodes = 2 * nodes + 2;
// 	vector<unordered_map<int, int>> adj_list(internal_nodes);
//
// 	for (int i = 1; i <= nodes; ++i) {
// 		int degree_out, degree_in;
// 		in >> degree_out >> degree_in;
//
// 		adj_list[0][i] = degree_out;
// 		adj_list[i + nodes][internal_nodes - 1] = degree_in;
// 		for(int j = nodes + 1; j < internal_nodes - 1; ++j){
// 			if (j - nodes == i)
// 				continue;
// 			adj_list[i][j] = 1;
// 		}
// 	}
// 	in.close();
//
//
// 	ofstream out("harta.out");
//
// 	Graph fromDegrees(internal_nodes, adj_list);
//
// 	auto sol = fromDegrees.getGraph(nodes);
// 	out << sol.size()<<"\n";
// 	for(const auto& edge : sol){
// 		out << edge.first << " " << edge.second << "\n";
// 	}
// 	out.close();
//
// 	return 0;
// }