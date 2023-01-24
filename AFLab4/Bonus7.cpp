#include <fstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <vector>

using namespace std;

/*
 *		Bonus 7) Riremito
 *
 *			Pentru fiecare nod vom stoca urmatoarele valori:
 *			- costul de a parcurge subarborele nodului si atat (eventual folosire riremito-> la radacina)
 *			- constul de a parcurge subarborele nodului si de a te intoarce in nod din subarbore
 *
 *			Parcurgem DFS arborele si populam aceste valori:
 *			- frunzele vor avea 0 la ambele
 *			- pentru fiecare copil al nodului incercam sa vizitam subarborele sau si sa ne intoarcem in
 *			  nod cu riremito sau fara
 *			- costul de parcurgere cu intoarcere este suma costurilor determinate pentru fiecare copil
 *			- costul de parcurgere fara intoarcere difera fata de cel cu intoarcere prin costul maxim
 *			  de intoarcere din orice copil
 *
 *			Complexitatea este O(N+M)
 *
 *
 */

class Graph {

private:
	vector<vector<pair<int, long long>>> _repr;
	vector<long long> sub_tree_cost;
	vector<long long> sub_tree_hearth;
	vector<bool> visited;
	int _nodes;

	void explore_dungeon(int node, long long path_distance){

		visited[node] = true;
		long long return_cost = 0;
		for(auto& edge : _repr[node]){

			const auto nbh_node = edge.first;
			const auto distance = edge.second;

			if(visited[nbh_node])
				continue;

			explore_dungeon(nbh_node,path_distance + distance);

			const long long explore_and_return = distance + sub_tree_cost[nbh_node] + distance;
			const long long explore_and_hearth = distance + sub_tree_hearth[nbh_node] + path_distance;

			if( explore_and_return >= explore_and_hearth){

				sub_tree_cost[node] += explore_and_hearth;
				return_cost = max(return_cost, path_distance);
			}
			else{
				sub_tree_cost[node] += explore_and_return;
				return_cost = max(return_cost, sub_tree_cost[nbh_node] - sub_tree_hearth[nbh_node] + distance);
			}

		}

		sub_tree_hearth[node] = sub_tree_cost[node] - return_cost;
	}


public:
	Graph(int nodes, vector<vector<pair<int, long long>>>& p_repr) :
		_repr(move(p_repr)),
		_nodes(nodes)
	{}

	long long dungeon_cost(int node)
	{
		sub_tree_cost = vector<long long>(_nodes,0);
		sub_tree_hearth = vector<long long>(_nodes, 0);
		visited = vector<bool>(_nodes, false);

		explore_dungeon(node, 0);

		return sub_tree_hearth[node];
	}
};


int main()
{
	ifstream in("riremito.in");
	int n, k;
	in >> n;

	vector<vector<pair<int, long long>>> repr(n + 1);

	for (int i = 0; i < n - 1; ++i) {
		int node1, node2;
		long long cost;
		in >> node1 >> node2 >> cost;
		repr[node1].emplace_back(node2, cost);
		repr[node2].emplace_back(node1, cost);
	}

	Graph dungeon(n + 1, repr);
	ofstream out("riremito.out");

	in >> k;
	for(int i=0; i<k; ++i)
	{
		int root;
		in >> root;
		out << dungeon.dungeon_cost(root) << "\n";
	}
	in.close();
	out.close();

	return 0;
}