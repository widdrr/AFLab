#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>

using namespace std;

/*
 *		Bonus 2) Drumuri2
 *
 *			Numarul minim de drumuri care acopera toate nodurile unui graph orientat aciclic se poate
 *		determina in materie de flux.
 *
 *			Asemanator cu determinarea cuplajului maxim, vom realiza o retea de flux in care:
 *		fiecare nod din prima partitie are un corespondent in a doua partitie. Daca un nod avea muchie
 *		cu un alt nod in graphul original, aici va avea cu corespondentul nodului in partitia 2.
 *		De asemenea fiecare corespondent din partitia 2 va avea o muchie spre nodul din partitia1.
 *		Toate muchiile au cost 1.
 *
 *			Valoarea fluxului maxim pe acesta retea determina ceva asemanator cuplajului maxim,
 *		dar in care daca avem muchia a->b, atunci muchia b->c este buna, a->d este,
 *		dar muchia e->b nu este.
 *			Numarul de drumuri minim va fi numarul original de noduri - fluxul.
 *
 *			Complexitatea este O(N*M^2) fiind dominata de algoritmul de flux.
 */

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
	int minPaths() {

		return _nodes/2 - 1 - maxFlow(0, _nodes - 1);
	}
};

int main()
{
	ifstream in("drumuri2.in");

	int nodes, edges;
	in >> nodes >> edges;

	vector<unordered_map<int, int>> adj_list(2*nodes + 2);
	for (int i = 0; i < edges; ++i) {
		int node1, node2;
		in >> node1 >> node2;
		adj_list[nodes + node1][node1] = 1;
		adj_list[node1][nodes + node2] = 1;
		adj_list[0][node1] = 1;
		adj_list[nodes + node2][2*nodes+1] = 1;
	}
	in.close();

	Graph pathing_time(2*nodes + 2, edges, adj_list);


	ofstream out("drumuri2.out");

	out<<pathing_time.minPaths();


	out.close();

	return 0;
}