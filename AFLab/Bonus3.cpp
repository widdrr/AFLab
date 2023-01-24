#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

/*
 *
 *		Bonus 3) Graf
 *		Algoritmul se bazeaza pe urmatoarea observatie:
 *			dat fiind arborele BFS al graphului cu startul din problema, nodurile comune
 *			tuturor drumurilor minime sunt acele noduri singure pe nivel(ignorand nodurile care
 *			nu fac parte din drumuri minime)
 *		Pentru a determina nodurile: se parcurge BFS graphul si se populeaza listele de tati
 *			pentru fiecare nod, cat si distantele de la start.
 *			Apoi, se parcurge BFS subarborele descris de listele de tati, pornind de la
 *			nodul destinatie. In parcurgere, se adauga fiecare nod in lista nivelului corespunzator.
 *		La final se parcurg nivelurile si se adauga toate nodurile singure in solutie. Solutia se sorteaza
 *
 *		Complexitatea algoritmului este O(nlogn + m), deoarece implica doua parcurgeri BFS si
 *			o sortare.
 *			Daca se foloseste un algoritm de sortare bazat pe numarare, complexitatea
 *			poate ajunge la O(n+m)
 */

class Graph
{
private:
	int _nodes, _edges;
	vector<vector<int>> _adj_list;

public:
	Graph(int nodes, int edges, vector<vector<int>>& adj_list) :
		_nodes(nodes),
		_edges(edges),
		_adj_list(move(adj_list)) {}

	vector<int> commonPathNodes(int start,int end){
		vector<int> sol_nodes;
		vector<int> distances(_nodes + 1, numeric_limits<int>::max());
		vector<vector<int>> fathers(_nodes + 1);
		vector<bool> upvisited(_nodes+1,false);
		vector<vector<int>> levels(_nodes + 1);
		queue<int> nodes_q;

		distances[start] = 0;
		int crt_node = start;

		while(crt_node != end){
			int path_dist = distances[crt_node] + 1;

			for(auto node : _adj_list[crt_node]){
				if(path_dist <= distances[node]) {
					if(path_dist < distances[node])
						nodes_q.push(node);

					distances[node] = path_dist;
					fathers[node].push_back(crt_node);
					
				}
			}

			crt_node = nodes_q.front();
			nodes_q.pop();
		}

		while (!nodes_q.empty())
			nodes_q.pop();

		nodes_q.push(end);
		upvisited[end] = true;

		while(!nodes_q.empty()){
			crt_node = nodes_q.front();
			nodes_q.pop();

			const int crt_lvl = distances[crt_node];
			levels[crt_lvl].push_back(crt_node);

			for(auto node: fathers[crt_node]){
				if (!upvisited[node]) {
					upvisited[node] = true;
					nodes_q.push(node);
				}
			}
		}

		for (auto& level : levels)
			if (level.size() == 1)
				sol_nodes.push_back(level[0]);
		return sol_nodes;
	}
};

int main() {
	int nodes, edges, start, end;
	ifstream in("graf.in");

	in >> nodes >> edges >> start >> end;

	vector<vector<int>> adj_list(nodes+1);

	for (int i = 0; i < edges; ++i) {
		int node1, node2;
		in >> node1 >> node2;
		adj_list[node1].push_back(node2);
		adj_list[node2].push_back(node1);
	}
	in.close();

	Graph g(nodes,edges,adj_list);
	ofstream out("graf.out");

	auto sol = g.commonPathNodes(start, end);
	out << sol.size()<< "\n";

	sort(sol.begin(), sol.end());

	for (auto node : sol)
		out << node << " ";
	out.close();

}