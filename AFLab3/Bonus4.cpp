#include <fstream>
#include <unordered_set>
#include <vector>

using namespace std;

/*
 *		Bonus 4) Johnie
 *
 *			Cerinta data este descompunerea unui (multi)graph dat in numarul minim de lanturi
 *		euleriene.
 *
 *			Daca toate nodurile ar avea grad par, atunci numarul de drumuri ar fi numarul de componente
 *		conexe deoarece fiecare componenta ar fi euleriana. Ne folosim de acest lucru si introducem
 *		un nod fictiv care are muchie catre fiecare nod de grad impar.
 *
 *			Aplicam apoi algoritmul lui Hierholzer, cu modificarea ca atunci cand ajungem iar in
 *		nodul fictiv, inseamna ca am incheiat un lant si il salvam. Practic determinam
 *		un mare ciclu eulerian pe care apoi il impartim in bucatele acolo unde apare nodul fictiv.
 *
 *			Complexitatea este O(N+M)
 */

class Graph {
private:
	int _nodes, _edges;
	vector<vector<pair<int,int>>> _adj_list;
	vector<vector<int>> _paths;
	vector<bool> _deleted;

	void euclidean_path(const int start, vector<int>& path){

		while(!_adj_list[start].empty()){
			auto node = _adj_list[start].back();
			_adj_list[start].pop_back();


			if (!_deleted[node.second]) {

				_deleted[node.second] = true;
				euclidean_path(node.first, path);
			}
		}
		if(start == 0){
			if (!path.empty()) {
				_paths.push_back(path);
				path.clear();
			}
		}
		else{
			path.push_back(start);
		}
	}
public:
	Graph(int nodes, int edges, vector<vector<pair<int,int>>>& adj_list):
		_nodes(nodes),
		_edges(edges),
		_adj_list(move(adj_list)),
		_deleted(_edges)
	{}
	vector<vector<int>> getEuclideanPaths()
	{
		vector<int> path;
		euclidean_path(0, path);
		return _paths;
	}
};

int main(){

	ifstream in("johnie.in");
	int nodes, edges;
	in >> nodes >> edges;

	vector<int> degrees(nodes + 1,0);
	vector<vector<pair<int,int>>> adj_list(nodes + 1);

	for(int i = 0; i < edges; ++i){
		int node1, node2;
		in >> node1 >> node2;
		++degrees[node1];
		++degrees[node2];
		adj_list[node1].emplace_back(node2,i);
		adj_list[node2].emplace_back(node1,i);
	}
	int no_edge = edges;
	for(int node = 1; node <= nodes; ++node){

		if (degrees[node] % 2 == 1){
			adj_list[0].emplace_back(node,no_edge);
			adj_list[node].emplace_back(0,no_edge);
			++no_edge;
		}
	}
	Graph euclidean(nodes, no_edge, adj_list);

	auto sol = euclidean.getEuclideanPaths();

	ofstream out("johnie.out");
	out << sol.size();
	for(auto& path : sol){
		out << "\n" << path.size() << " ";
		for(auto& node : path){
			out << node << " ";
		}
	}
	out.close();
}
