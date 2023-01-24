#include <fstream>
#include <vector>
#include <limits>
#include <deque>

using namespace std;

/*
 *		Bonus 2) Padure
 *		O matrice poate fi interpretata drept un graph. Deoarece "graphul" obtinut are costuri
 *			bine stabilite (0 sau 1), este suficient sa facem o parcurgere BFS:
 *			pornind din start, calculam costul pentru a ne deplasa in oricare directie si adaugam
 *			"nodurile" in coada atunci cand costul este mai mic decat cel anterior.
 *		Optimizarea relevanta este folosirea unui deque pentru BFS. Vom introduce nodurile
 *		din fata daca deplasarea nu a costat nimic si din spate daca a costat.
 *			Acest lucru asigura ca vom vizita "nodurile" de cat mai putine ori.
 *
 *		Complexitatea este O(n*m) unde n si m sunt dimensiunile matricei, deoarece se poate
 *			parcurge intreaga matrice pana ajungem la destinatie
 *
 */

class Forest
{
private:
	int _n, _m;
	vector<vector<int>> _trees;

	bool inBounds(int y, int x) {
		return x >= 0 && y >= 0
			&& x < _m && y < _n;
	}
public:
	Forest(int n,int m, vector<vector<int>>& trees):
	_n(n),
	_m(m),
	_trees(move(trees)){}

	int getMinCost(int castle_y,int castle_x, int start_y,int start_x){
		vector<vector<int>> path_costs(_n, vector<int>(_m, numeric_limits<int>::max()));
		deque<pair<int,int>> coordinate_deque;
		vector<pair<int,int>> directions = { {-1,0},{0,1},{1,0},{0,-1} };

		coordinate_deque.push_front({start_y,start_x});
		path_costs[start_y][start_x] = 0;

		while(!coordinate_deque.empty()){
			auto crt_coords = coordinate_deque.front();
			coordinate_deque.pop_front();
			int coord_y = crt_coords.first;
			int coord_x = crt_coords.second;

			for(auto dir : directions){
				int new_y = coord_y + dir.first;
				int new_x = coord_x + dir.second;
				int crt_cost = path_costs[coord_y][coord_x];

				if(inBounds(new_y,new_x)){
					bool high_priority = true;
					if (_trees[coord_y][coord_x] != _trees[new_y][new_x]) {
						++crt_cost;
						high_priority = false;
					}

					if(crt_cost < path_costs[new_y][new_x]){
						path_costs[new_y][new_x] = crt_cost;
						if(high_priority)
							coordinate_deque.push_front({ new_y,new_x });
						else
							coordinate_deque.push_back({ new_y,new_x });
					}
				}
			}
		}
		return path_costs[castle_y][castle_x];

	}
};

// int main(){
// 	int n, m, castle_y, castle_x, start_y, start_x;
// 	vector<vector<int>> trees;
//
// 	ifstream in("padure.in");
// 	in >> n >> m >> start_y >> start_x >> castle_y >> castle_x;
// 	for(int i=0;i<n;++i){
// 		trees.emplace_back();
// 		for(int j=0; j<m;++j){
// 			int cell;
// 			in >> cell;
// 			trees[i].push_back(cell);
// 		}
// 	}
// 	in.close();
// 	Forest f(n, m, trees);
// 	ofstream out("padure.out");
// 	out << f.getMinCost(castle_y - 1, castle_x - 1, start_y - 1, start_x - 1);
//
// }