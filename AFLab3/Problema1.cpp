#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>

/*
 *		1) Maxflow
 *
 *			a) Pentru a determina fluxul maxim aplic algoritmul Edmonds-Karp,
 *		versiunea algoritmului Ford-Fulkerson care foloseste BFS pentru a determina drumuri de crestere.
 *
 *			   Complexitatea algoritmului este O(N*M^2), unde N-noduri, M-muchii, dar in practica se comporta
 *		mult mai bine. De asemenea, o optimizare adusa este ca la fiecare pas se considera intreg arborele BFS
 *		construit si se incearca ameliorarea fiecarui drum gasit, ceea ce reduce numarul mediu de BFS-uri.
 *
 *			b) Odata aplicat algoritmul de flux maxim, taietura maxima se poate determina din graphul rezidual:
 *		in taietura minima vom pune toate muchiile de la noduri accesibile din sursa la noduri inaccesibile.
 *		Parcurgem graphul din sursa si apoi iteram prin toate nodurile vizitate.
 *
 *			   Complexitatea este O(N+M)
 *
 */

using namespace std;

class Graph {
private:
	int _nodes, _edges;
	vector<unordered_map<int, int>> _residual_graph;
	vector<int> _bfs_tree;
	vector<bool> _visited;
	vector<unordered_map<int, int>> _adj_list;

	bool BFS(const int start, const int dest) {

		queue<int> node_queue;
		_visited = vector<bool>(_nodes + 1);
		_bfs_tree = vector<int>(_nodes + 1);

		_bfs_tree[start] = 0;
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
		_residual_graph(adj_list),
		_adj_list(std::move(adj_list))
	{
		for (int i = 1; i <= _nodes; ++i) {
			for (const auto& edge : _adj_list[i]) {
				if(_residual_graph[edge.first].find(i) == _residual_graph[edge.first].end())
					_residual_graph[edge.first][i] = 0;
			}
		}
	}

	int maxFlow(const int source,const int dest){
		int max_flux = 0;
		while(BFS(source,dest)){
			for (const auto& edge : _residual_graph[dest]) {

				int node = edge.first;
				if (edge.second == _adj_list[node][dest] || !_visited[node])
					continue;

				int flux = numeric_limits<int>::max();
				flux = min(flux, _residual_graph[node][dest]);

				while (_bfs_tree[node] != 0) {
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
				while (_bfs_tree[node] != 0) {
					const int father = _bfs_tree[node];
					_residual_graph[father][node] -= flux;
					_residual_graph[node][father] += flux;
					node = father;
				}
			}
		}
		return max_flux;
	}
	pair<vector<pair<int,int>>,int> minCut(const int source){
		queue<int> node_queue;
		vector<pair<int, int>> edges;
		int min_cut = 0;
		_visited = vector<bool>(_nodes + 1);
		
		node_queue.push(source);
		_visited[source] = true;

		while(!node_queue.empty()){
			const int crt_node = node_queue.front();
			node_queue.pop();

			for(const auto& edge : _residual_graph[crt_node]){
				int node = edge.first;
				if (!_visited[node] && _residual_graph[crt_node][node] > 0) {
					_visited[node] = true;
					node_queue.push(node);
				}
			}
		}

		for (int crt_node = 1; crt_node <= _nodes; ++crt_node) {
			if (_visited[crt_node]) {
				for (const auto& edge : _adj_list[crt_node]) {
					int node = edge.first;
					if (!_visited[node]) {
						edges.emplace_back(node, edge.first);
						min_cut += edge.second;
					}
				}
			}
		}
		return make_pair(edges, min_cut);

	}
};

// int main()
// {
// 	ifstream in("maxflow.in");
//
// 	int nodes, edges;
// 	in >> nodes >> edges;
//
// 	vector<unordered_map<int, int>> adj_list(nodes + 1);
// 	for(int i=0; i<edges; ++i){
// 		int node1, node2, capacity;
// 		in >> node1 >> node2 >> capacity;
// 		adj_list[node1][node2] = capacity;
// 	}
// 	in.close();
//
// 	Graph flux_time(nodes, edges, adj_list);
//
// 	ofstream out("maxflow.out");
// 	out << flux_time.maxFlow(1,nodes)<<'\n';
// 	//out << flux_time.minCut(1).second << '\n';
// 	out.close();
// }
