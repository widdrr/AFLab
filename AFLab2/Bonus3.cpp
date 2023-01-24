#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 *		Bonus 3) Online
 *			Incepem prin a crea APM-ul folosind Kruskal si punem muchiile folosite intr-o lista
 *		separata. Apoi, pentru fiecare muchie noua propusa, construim un APM din
 *		muchiile din cel initial si muchia noua, si calculam noua valoare, actualizand
 *		lista de muchii de folosit la iteratia viitoare.
 *			Primul APM necesita O(M*logN), M-muchii, N-noduri, dar urmatoarele sunt surprinzator
 *		de rapide in O(M'*N*logN) M'=muchii noi, deci complexitate finala O(M*logN).
 */

class DisjointSet {
private:
	const int _size;
	int* _father;
	int* _rank;

	int getRoot(int el) {
		while (_father[el - 1] != el) {
			el = _father[el - 1];
		}
		return el;
	}
public:
	DisjointSet(const int p_size) : _size(p_size) {
		_father = new int[_size];
		_rank = new int[_size];
		for (int i = 0; i < p_size; ++i) {
			_father[i] = i + 1;
			_rank[i] = 1;
		}
	}
	void merge(const int x, const int y) {
		const int root_y = getRoot(y);
		const int root_x = getRoot(x);
		if (_rank[root_x - 1] > _rank[root_y - 1]) {
			_father[root_y - 1] = root_x;
		}
		else {
			_father[root_x - 1] = root_y;
			if (_rank[root_x - 1] == _rank[root_y - 1]) {
				_rank[root_y - 1] = _rank[root_x - 1] + 1;
			}
		}
	}

	bool check(const int x, const int y) {
		return getRoot(x) == getRoot(y);
	}
	~DisjointSet() {
		delete[] _father;
		delete[] _rank;
	}
};

struct edge{
	int node1, node2, cost;

	edge(int p_node1, int p_node2, int p_cost) :
		node1(p_node1),
		node2(p_node2),
		cost(p_cost){}

	bool operator<(const edge& rhs) const{
		return this->cost < rhs.cost;
	}
};

int main(){
	ifstream in("online.in");
	ofstream out("online.out");
	int n, m;
	in >> n >> m;
	vector<edge> edges;
	vector<edge> apm_edges;

	for(int i=0; i<m;++i){
		int node1, node2, cost;
		in >> node1>>node2>>cost;
		edges.emplace_back(node1, node2, cost);
	}
	sort(edges.begin(), edges.end());

	DisjointSet apm(n);
	int total_cost = 0;
	auto it = edges.begin();

	while(apm_edges.size() < n-1){
		auto edge = *it;
		if(!apm.check(edge.node1,edge.node2)){
			apm.merge(edge.node1, edge.node2);
			apm_edges.push_back(edge);
			total_cost += edge.cost;
		}
		++it;
	}
	out << total_cost<<"\n";

	int k;
	in >> k;

	for(int i=0; i<k; ++i){
		int node1, node2, cost;
		in >> node1 >> node2 >> cost;
		edge new_edge(node1, node2, cost);

		DisjointSet new_apm(n);
		vector<edge> new_edges;
		it = apm_edges.begin();
		int new_cost = 0;
		bool taken = false;

		while(new_edges.size() < n-1){
			auto edge = *it;
			if (!taken && edge.cost > new_edge.cost) {
				edge = new_edge;
				taken = true;
			}
			else ++it;
			if (!new_apm.check(edge.node1, edge.node2)) {
				new_apm.merge(edge.node1, edge.node2);
				new_edges.push_back(edge);
				new_cost += edge.cost;
			}
		}
		apm_edges = new_edges;
		out << new_cost << "\n";
	}
	in.close();
	out.close();
}