#include <fstream>
#include <vector>
#include <queue>
#include <iomanip>
#include <limits>

using namespace std;

/*
 *		Bonus 5) Ciclu
 *			Fie o valoare x, putem verifica daca exista un ciclu cu media muchiilor <= x
 *		aplicand BellmanFord in care din fiecare muchie scadem x. Un astfel de ciclu va fi
 *		detectat drept ciclu negativ. Prin urmare trebuie sa gasim cea mai mica valoare x
 *		pentru care gasim un ciclu.
 *			Acest lucru se face eficient aplicand o cautare binara: avand limitele
 *		0 si 10^5(deoarece avem max 10^3noduri care au costuri <= 100), efectuam cautarea
 *		binara cu regula: daca gasim ciclu -> cautam in jumatatea stanga
 *						  daca nu -> cautam in jumatatea dreapta.
 *			Oprim algoritmul atunci cand diferenta dintre limite este <0.01;
 *			Complexitatea sa este O(5Log10*N*M), deci O(N*M).
 *
 */

class Graph {
public:
	struct edge {
		int to;
		float dist;
		edge(int p_to, float p_dist) : to(p_to), dist(p_dist){}
	};
private:
	int _nodes, _edges;
	vector<vector<edge>> _adj_list;
	vector<bool> _visited;
	vector<int> _times_updt;
	vector<float> _distances;
	bool detectNegativeCycle(const int src, const float value) {

		queue<int> node_queue;
		node_queue.push(src);
		_distances[src] = 0;

		while (!node_queue.empty()) {
			const int crt_node = node_queue.front();
			node_queue.pop();
			_visited[crt_node] = true;

			for (auto& edge : _adj_list[crt_node]) {
				if (_distances[edge.to] > _distances[crt_node] + edge.dist - value) {
					_distances[edge.to] = _distances[crt_node] + edge.dist - value;
					node_queue.push(edge.to);
					++_times_updt[edge.to];
					if (_times_updt[edge.to] == _nodes)
						return true;
				}
			}
		}
		return false;
	}
public:
	Graph(int nodes, int edges, vector<vector<edge>> adj_list):
		_nodes(nodes),
		_edges(edges),
		_adj_list(std::move(adj_list)) {}

	float getMinCycleAvg(){
		float u_bound = 1e5;
		float l_bound = 0;
		_distances = vector<float>(_nodes + 1, numeric_limits<float>::max());
		_times_updt = vector<int>(_nodes + 1, 0);
		_visited = vector<bool>(_nodes + 1);
		while(abs(l_bound - u_bound) >= 0.01){

			for(int i=1; i<=_nodes; ++i)
			{
				_times_updt[i] = 0;
				_distances[i] = numeric_limits<float>::max();
				_visited[i] = false;
			}

			const float crt_val = (u_bound + l_bound) / 2;
			for (int i = 1; i <= _nodes; ++i)
			{
				if (_visited[i])
					continue;
				if (detectNegativeCycle(i, crt_val)) {
					u_bound = crt_val;
					break;
				}
				else {
					bool next = true;
					for (int j = 1; j <= _nodes; ++j) {
						next &= _visited[j];
					}
					if (next) l_bound = crt_val;
				}
			}
		}
		return (u_bound+l_bound)/2;
	}
};

int main(){
	ifstream in("ciclu.in");
	int n, m;
	in >> n >> m;
	vector<vector<Graph::edge>> adj_list(n + 1);
	for(int i=0; i<m;++i)
	{
		int node1, node2;
		float dist;
		in >> node1 >> node2 >> dist;
		adj_list[node1].emplace_back(node2, dist);
	}
	in.close();

	Graph g(n, n, adj_list);
	ofstream out("ciclu.out");

	out <<fixed<<setprecision(2)<<g.getMinCycleAvg();
	out.close();
	
}