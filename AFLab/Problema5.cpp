#include <queue>
#include <fstream>
#include <vector>

using namespace std;

/*
 *		5) Nume Necunoscut
 *
 *		Parcurgand BFS pornind dintr-un anume nod, putem determina
 *			distantele de la acel nod la toate celelalte.
 *		Pentru a fi totusi mai eficient, pot face o singura parcurgere BFS,
 *			incepand de la nodurile de control, iar conditia de vizitat se modifica in:
 *			"nevizitat, sau distanta curenta < distanta minima gasita pana acum".
 *		Aceasta conditie asigura ca nu vom parcurge inutil noduri si ca rezultatul va fi
 *			chiar distanta minima.
 *
 *		Complexitatea este O(n+m) cu n-noduri, m-muchii deoarece se efectueaza o singura
 *			parcurgere BFS, ce porneste din toate nodurile de control.
 *
 */

class Graph{
private:
	int _nodes, _edges;
	vector<vector<int>> _adj_list;
	vector<int> bfs(vector<int>& control_nodes) {
		queue<int> node_queue;
		vector<int> distances(_nodes + 1, -1);
		for (auto node : control_nodes) {
			node_queue.push(node);
			distances[node] = 0;
		}

		while(!node_queue.empty()){
			int crt_node = node_queue.front();
			for(int node: _adj_list[crt_node]){
				int crt_dist = distances[crt_node] + 1;
				if(distances[node] == -1 || distances[node] > crt_dist) {
					node_queue.push(node);
					distances[node] = distances[crt_node] + 1;
				}
			}
			node_queue.pop();
		}
		return distances;
	}


public:
	Graph(int nodes, int edges, vector<vector<int>> adj_list) :
	_nodes(nodes),
	_edges(edges),
	_adj_list(std::move(adj_list)){}

	vector<int> allDistances(vector<int>& control_nodes){
		return bfs(control_nodes);
	}
};


// int main(){
// 	int nodes, edges;
// 	ifstream in("graph.in");
//
// 	in >> nodes >> edges;
//
// 	vector<vector<int>> adj_list(nodes + 1);
// 	vector<int> control_nodes;
//
// 	for(int node=1; node<=edges; ++node){
// 		int n1, n2;
// 		in >> n1 >> n2;
// 		adj_list[n1].push_back(n2);
// 		adj_list[n2].push_back(n1);
// 	}
//
// 	int control;
// 	while (in >> control)
// 		control_nodes.push_back(control);
// 	in.close();
//
// 	Graph g(nodes, edges, adj_list);
// 	ofstream out("graph.out");
// 	auto sol = g.allDistances(control_nodes);
// 	for (int node = 1; node <= nodes; ++node)
// 		out << sol[node] << " ";
// 	out.close();
//
// }

