#include <vector>

using namespace std;

/*
 *      Bonus 5) Max Area of Island
 *      Matricea data poate fi tratata drept un graph, in care celulele "1" ortogonale
 *          sunt adiacente. 
 *      Astfel, problema se transforma in determinarea celei mai mari componente conexe,
 *          lucru aflat usor printr-o parcurgere DFS.
 *
 *      Complexitatea este  O(n*m) unde n si m sunt dimensiunile matricei,
 *      deoarece trebuie parcursa intreaga matrice in cautarea celulelor "1";
 */

class Solution {
private:
    int _max_island = 0;
    int _island_size = 1;
    int _m;
    int _n;
    vector<pair<int, int>> directions = { {-1,0},{0,1},{1,0},{0,-1} };

    bool inBounds(int y, int x) {
        return x >= 0 && y >= 0
            && x < _n && y < _m;
    }
    void dfs(int y, int x, vector<vector<int>>& grid) {
        grid[y][x] = ++_island_size;
        for (auto dir : directions) {
            int new_y = y + dir.first;
            int new_x = x + dir.second;

            if (inBounds(new_y, new_x)) {
                if (grid[new_y][new_x] == 1) {
                    dfs(new_y, new_x, grid);
                }
            }
        }
    }
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        _m = grid.size();
        _n = grid[0].size();

        for (int y = 0; y < _m; ++y) {
            for (int x = 0; x < _n; ++x) {
                if (grid[y][x] == 1) {
                    _island_size = 1;
                    dfs(y, x, grid);
                    _max_island = max(_max_island, _island_size - 1);
                }
            }
        }
        return _max_island;
    }
};