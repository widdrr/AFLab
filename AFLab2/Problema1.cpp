#include <fstream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <iomanip>
#include <cmath>

/*
 *		1) Retea2
 *
 *			Se considera doua multimi: nodurile conectate la retea (powered)
 *		si cele care nu sunt (unpowered). Se calculeaza dinstanta de la fiecare
 *		nod unpowered la cel mai apropiat nod powered.
 *			La fiecare pas, se alege nodul cu distanta minima, se adauga in multimea powered
 *		si se actualizeaza distantele tuturor nodurilor unpowered. Este practic algoritmul
 *		Prim de determinare APM.
 *
 *			Fie C = centrale, B = blocuri, complexitatea algoritmului este de O(CB + B(B+C))).
 *		Worst case pare sa fie C=B=N/2, rezultand o complexitate de O(N^2)
 */

using namespace std;

struct node{
	int x,y;

	node(int p_x, int p_y) : x(p_x), y(p_y){}
	node() = default;
};

double computeDistance(node n1, node n2){
	long long n1x = n1.x;
	long long n2x = n2.x;
	long long n1y = n1.y;
	long long n2y = n2.y;
	return sqrt((n1x - n2x) * (n1x - n2x)
		+ (n1y - n2y) * (n1y - n2y));
}

pair<const int,double> getMin(unordered_map<int,double>& p_map){
	int min_index = 0;
	double min_dist = numeric_limits<double>::max();
	for(auto& val : p_map){
		if (val.second < min_dist) {
			min_index = val.first;
			min_dist = val.second;
		}
	}
	return { min_index,min_dist };
}
int main(){
	ifstream in("retea2.in");
	int n, m;
	vector<node> power;
	vector<node> not_power;
	in >> n >> m;
	for(int i=0; i<n; ++i){
		int x, y;
		in >> x >> y;
		power.emplace_back(x, y);
	}
	for(int i=0; i<m; ++i){
		int x, y;
		in >> x >> y;
		not_power.emplace_back(x, y);
	}
	unordered_map<int, double> distances;
	for (int i = 0; i < m; ++i) {
		distances[i] = numeric_limits<double>::max();
		for(auto& src: power){
			distances[i] = min(distances[i], computeDistance(not_power[i], src));
		}
	}
	double sol = 0;
	while(!distances.empty()){
		auto next_link = getMin(distances);
		sol += next_link.second;

		distances.erase(next_link.first);

		for(auto& dest : distances){
			dest.second = min(dest.second,computeDistance(not_power[dest.first], not_power[next_link.first]));
		}
	}
	
	ofstream out("retea2.out");
	out << fixed << setprecision(6) <<sol;
	out.close();
}