#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <limits>

/*
 *		5) Catun
 *
 *			Pentru a determina cea mai apropiata fortareata de fiecare catun, aplic
 *		algoritmul Dijkstra din fiecare fortareata (pentru eficienta maxima, as putea verifica
 *		daca fortarete > catune si modific corespunzator).
 *		Toate parcurgerile folosesc acelasi vector de distante
 *		pentru a nu parcurge degeaba cand nu este cazul.
 *			Complexitatea este de O(N*M*logN) in cel mai rau caz: Fort=Cat=N/2
 */
using namespace std;
class Graph {
public:
	struct edge {
		int id,dist;

		edge(int p_id, int p_dist) : id(p_id), dist(p_dist) {}

		bool operator>(const edge& rhs) const {
			return this->dist > rhs.dist;
		}
	};
private:
	int _nodes, _edges;
	vector<vector<edge>> _adj_list;
	vector<int> _distances;
	set<int> _fortresses;
	vector<int> _leader;
	priority_queue <edge, vector<edge>, greater<>> _edge_heap;

	void dijkstra(const int start){
		int crt_node = start;
		_distances[start] = 0;
		while (true) {
			for (auto& neigh : _adj_list[crt_node])
				if (_distances[neigh.id] > _distances[crt_node] + neigh.dist) {
					if (_fortresses.find(neigh.id) == _fortresses.end())
						_leader[neigh.id] = start;
					_distances[neigh.id] = _distances[crt_node] + neigh.dist;
					_edge_heap.emplace(neigh.id, _distances[neigh.id]);
				}
			if (_edge_heap.empty())
				break;
			crt_node = _edge_heap.top().id;
			_edge_heap.pop();
		}
	}
public:
	Graph(int nodes, int edges, vector<vector<edge>> adj_list) :
		_nodes(nodes),
		_edges(edges),
		_adj_list(std::move(adj_list)) {}

	vector<int> allDistances(set<int>& fortresses) {
		_fortresses = fortresses;
		_distances = vector<int>(_nodes + 1,numeric_limits<int>::max());
		_leader = vector<int>(_nodes + 1);
		for(const auto fortress : fortresses){
			dijkstra(fortress);
		}
		return _leader;
	}
};

int main(){
	ifstream in("catun.in");
	int n, m, k;
	in >> n >> m >> k;
	set<int> fortresses;
	vector<vector<Graph::edge>> adj_list(n+1);
	for(int i=0; i<k;++i){
		int fortress;
		in >> fortress;
		fortresses.emplace(fortress);
	}
	for(int i=0; i<m;++i){
		int node1, node2, dist;
		in >> node1 >> node2 >> dist;
		adj_list[node1].emplace_back(node2, dist);
		adj_list[node2].emplace_back(node1, dist);
	}
	in.close();

	Graph g(n, m, adj_list);
	auto sol = g.allDistances(fortresses);

	ofstream out("catun.out");
	for (int i = 1; i <= n; ++i)
		out << sol[i] << " ";
	out.close();
	return 0;
}