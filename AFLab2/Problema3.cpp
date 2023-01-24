#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

/*
 *		3) Clustering
 *
 *			Problema de clustering pa baza unei functii de distanta date se rezuma la o
 *		modificare a algoritmului lui Kruskal:
 *			Se sorteaza muchiile obtinute pe baza functiei de distanta.
 *		Deoarece la fiecare iteratie din Kruskal, se reduce numarul de componente conexe cu 1,
 *		trebuie sa facem nrNoduri - nrGrupuriCerute iteratii. Apoi, distanta minima dintre componente
 *		este urmatoarea muchie valida gasita. Pentru Kruskal folosim un Disjoint Set.
 *			Stiind ca "graphul" este complet, complexitatea devine O(N^2logN)
 *		(ignorand complexitatea construirii grafului)
 *
 */

size_t levenshteinPrivate(string::const_iterator _s1_beg,string::const_iterator _s1_end,
					   string::const_iterator _s2_beg, string::const_iterator _s2_end)
{
	if (_s1_beg == _s1_end) return (_s2_end - _s2_beg);
	if (_s2_beg == _s2_end) return (_s1_end - _s1_beg);


	if(*_s1_beg != *_s2_beg)
	{
		return 1 + min({levenshteinPrivate(_s1_beg + 1,_s1_end,_s2_beg,_s2_end),
					    levenshteinPrivate(_s1_beg + 1, _s1_end,_s2_beg + 1, _s2_end),
						levenshteinPrivate(_s1_beg,_s1_end,_s2_beg + 1, _s2_end)});
	}

	return levenshteinPrivate(_s1_beg + 1, _s1_end, _s2_beg + 1, _s2_end);
}
int levenshtein2(const string& _s1, const string& _s2)
{
	return levenshteinPrivate(_s1.begin(), _s1.end(), _s2.begin(), _s2.end());
}
struct edge{
	int _node1, _node2, _weight;

	edge(int p_node1, int p_node2, int p_weight) :
		_node1(p_node1),
		_node2(p_node2),
		_weight(p_weight)
	{}
	bool operator>(const edge& rhs) const {
		return this->_weight > rhs._weight;
	}

};

class DisjointSet {
private:
	const int _size;
	int* _father;
	int* _rank;
	vector<bool> _visited;
	vector<int> _sets;

	int getRoot(int el) {
		while (_father[el] != el) {
			el = _father[el];
		}
		return el;
	}
	int makeSets(int node){
		if (_sets[node] != -1)
			return _sets[node];

		if(node == _father[node]){
			_sets[node] = node;
			return node;
		}

		const int set = makeSets(_father[node]);
		_sets[node] = set;
		return set;
	}
public:
	DisjointSet(const int p_size) : _size(p_size) {
		_father = new int[_size];
		_rank = new int[_size];

		for (int i = 0; i < _size; ++i) {
			_father[i] = i;
			_rank[i] = 1;
		}
	}
	void merge(const int x, const int y) {
		const int root_y = getRoot(y);
		const int root_x = getRoot(x);
		if (_rank[root_x] > _rank[root_y]) {
			_father[root_y] = root_x;
		}
		else {
			_father[root_x] = root_y;
			if (_rank[root_x] == _rank[root_y]) {
				_rank[root_y] = _rank[root_x] + 1;
			}
		}
	}

	bool check(const int x, const int y) {
		return getRoot(y) == getRoot(x);
	}
	~DisjointSet() {
		delete[] _father;
		delete[] _rank;
	}
	vector<vector<int>> getSets(){
		vector<vector<int>> set_lists(_size);
		_visited = vector<bool>(_size,false);
		_sets = vector<int>(_size, -1);
		for (int i = 0; i < _size; ++i) {
			makeSets(i);
			set_lists[_sets[i]].push_back(i);
		}
		return set_lists;
	}
};
int main(){

	vector<string> words;
	priority_queue<edge, vector<edge>, greater<>> edge_heap;

	ifstream in("cuvinte.in");
	string word;
	while(in>>word){
		words.push_back(word);
	}
	in.close();

	for (int i = 0; i < words.size() - 1; ++i) {
		for (int j = i + 1; j < words.size(); ++j) {
			edge_heap.emplace(i, j, levenshtein2(words[i], words[j]));
		}
	}

	DisjointSet components(words.size());
	int groups;
	cin >> groups;

	for(int i=0; i < words.size() - groups; ++i){
		auto next_edge = edge_heap.top();
		edge_heap.pop();
		if (!components.check(next_edge._node1, next_edge._node2)){
			components.merge(next_edge._node1, next_edge._node2);
		}
		else --i;
		
	}

	while(!edge_heap.empty() && components.check(edge_heap.top()._node1, edge_heap.top()._node2)) {
		edge_heap.pop();
	}

	int sep_degree = !edge_heap.empty() ? edge_heap.top()._weight : 0;
	auto sol = components.getSets();

	ofstream out("cuvinte.out");
	for(auto& set : sol){
		if (set.empty())
			continue;
		for(auto& index : set){
			out << words[index]<<" ";
		}
		out << "\n";
	}
	out << sep_degree;
	out.close();
	return 0;
}