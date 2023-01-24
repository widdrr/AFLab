#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 *		Bonus 4) APM2
 *			Incepem prin a construi APM-ul folosind Kruskal, si salvam muchiile utilizate.
 *		Apoi, pentru fiecare muchie noua propusa, incercam sa facem un nou APM:
 *		-includem muchia ceruta
 *		-continuam sa incercam a adaugam muchiile si sa reunim componentele
 *		-cand gasim o muchie(si singura de altfel) intre doua noduri ce face parte din
 *		aceeasi componenta, costul muchiei necesare va fi costul muchiei gasite - 1.
 *			Complexitate: APM initial O(M*logN), APM urmatoare O(M'*N*logN), deci per total
 *		O(M*logN), cu M-muchii, N-noduri, M'-muchii noi propuse.
 *
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

struct edge {
	int node1, node2, cost;

	edge(int p_node1, int p_node2, int p_cost) :
		node1(p_node1),
		node2(p_node2),
		cost(p_cost) {}

	bool operator<(const edge& rhs) const {
		return this->cost < rhs.cost;
	}
};

int main() {
	ifstream in("apm2.in");
	ofstream out("apm2.out");
	int n, m, q;
	in >> n >> m >> q;
	vector<edge> edges;
	vector<edge> apm_edges;

	for (int i = 0; i < m; ++i) {
		int node1, node2, cost;
		in >> node1 >> node2 >> cost;
		edges.emplace_back(node1, node2, cost);
	}
	sort(edges.begin(), edges.end());

	DisjointSet apm(n);
	auto it = edges.begin();

	while (apm_edges.size() < n - 1) {
		auto edge = *it;
		if (!apm.check(edge.node1, edge.node2)) {
			apm.merge(edge.node1, edge.node2);
			apm_edges.push_back(edge);
		}
		++it;
	}

	for (int i = 0; i < q; ++i) {
		int node1, node2;
		in >> node1 >> node2;

		DisjointSet new_apm(n);
		it = apm_edges.begin();
		int edge_cost = 0;

		new_apm.merge(node1, node2);

		for(auto& edge : apm_edges ) {
			if (new_apm.check(edge.node1, edge.node2)) {
				edge_cost = edge.cost - 1;
			}
			else{
				new_apm.merge(edge.node1, edge.node2);
			}
		}
		out << edge_cost << "\n";
	}
	in.close();
	out.close();
}