#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>

/*
 *		2) Cuplaj Maxim
 *
 *			Cuplajul maxim intr-un graph bipartit se poate realiza aplicand algoritmul de flux maxim
 *		pe o retea de flux bine construita: La graphul original bipartit se adauga o sursa conectata
 *		catre toate nodurile din prima partitie si o destinatie conectata catre toate din a doua. Toate muchiile
 *		au capacitate 1.
 *
 *			Dupa flux, cuplajul maxim se determina selectand din graphul rezidual toate muchiile
 *		dintre cele doua partitii care au capacitatea ramasa 0.
 *
 *			Complexitatea este dominata de algoritmul de flux utilizat, in cazul meu fiind O(N*M^2)
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
	Graph(int nodes, int edges, vector<unordered_map<int, int>> adj_list) :
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
	void maxMatching(const int set1, const int set2,ostream& out) {

		vector<pair<int,int>> edges;
		int value = 0;
		maxFlow(0, _nodes - 1);
		for (int i = 1; i <= set1; ++i) {
			for (const auto& edge : _residual_graph[i]) {
				int j = edge.first;
				if (j != 0 && _residual_graph[i][edge.first] == 0) {
					edges.emplace_back(i, j - set1);
					++value;
				}
			}

		}
		out << value << "\n";
		for (const auto& edge : edges) {
			out << edge.first << " " << edge.second << "\n";
		}
	}
};

// int main()
// {
// 	ifstream in("cuplaj.in");
//
// 	int set1, set2,edges;
// 	in >> set1 >> set2 >> edges;
//
// 	int nodes = set1 + set2 + 2;
// 	vector<unordered_map<int, int>> adj_list(nodes);
// 	for (int i = 0; i < edges; ++i) {
// 		int node1, node2;
// 		in >> node1 >> node2;
// 		adj_list[node1][set1 + node2] = 1;
// 	}
// 	for (int i = 1; i <= set1; ++i){
// 		adj_list[0][i] = 1;
// 	}
// 	for(int i = set1 + 1; i < nodes - 1; ++i){
// 		adj_list[i][nodes - 1] = 1;
// 	}
// 	in.close();
//
// 	Graph matching_time(nodes, edges, adj_list);
//
// 	
// 	ofstream out("cuplaj.out");
//
// 	matching_time.maxMatching(set1, set2, out);
//
// 	
// 	out.close();
//
// 	return 0;
// }
