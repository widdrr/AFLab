#include <fstream>
using namespace std;


/*
 *		2) Disjoint
 *
 *			Retinem multimile drept structuri arborescente, folosind un vector de tati.
 *		-Pentru a determina daca doua elemente sunt in aceeasi multime, se parcurg tatii pana
 *		 la radacina. Complexitatea este in jur de O(logN)?.
 *		-Pentru reuniunea a doua multimi se leaga radacina cu inaltime mai mica de cealalta, din nou complexitatea
 *		 fiind reprezentata de determinarea tatalui,deci tot O(logN)?.
 */

class DisjointSet{
private:
	const int _size;
	int* _father;
	int* _rank;

	int getRoot(int el){
		while(_father[el - 1] != el){
			el = _father[el - 1];
		}
		return el;
	}
public:
	DisjointSet(const int p_size) : _size(p_size){
		_father = new int[_size];
		_rank = new int[_size];
		for(int i = 0; i<p_size;++i){
			_father[i] = i + 1;
			_rank[i] = 1;
		}
	}
	void merge(const int x, const int y){
		const int root_y = getRoot(y);
		const int root_x = getRoot(x);
		if(_rank[root_x - 1] > _rank[root_y - 1]){
			_father[root_y - 1] = root_x;
		}
		else{
			_father[root_x - 1] = root_y;
			if(_rank[root_x - 1] == _rank[root_y - 1]){
				_rank[root_y - 1] = _rank[root_x - 1] + 1;
			}
		}
	}

	bool check(const int x, const int y){
		return getRoot(x) == getRoot(y);
	}
	~DisjointSet(){
		delete[] _father;
		delete[] _rank;
	}
};

int main(){
	ifstream in("disjoint.in");
	ofstream out("disjoint.out");
	int n, m;
	in >> n >> m;
	DisjointSet forest(n);
	for(int i=0; i<m; ++i){
		int cmd, x, y;
		in >> cmd >> x >> y;
		switch(cmd)
		{
		case 1:
			forest.merge(x, y);
			break;

		case 2:
			out << (forest.check(x, y) ? "DA\n" : "NU\n");
			break;

		default:
			break;
		}
	}
	in.close();
	out.close();
}