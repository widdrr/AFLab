#include <algorithm>
#include <vector>

using namespace std;

/*
 *      Bonus 2) Checking Existence....
 *          Pentru a rezolva, la fiecare query, in loc sa parcurgem pentru a verifica,
 *      incercam sa vedem daca putem construi drumul cerut din muchiile < valoarea din query.
 *      Sortam muchiile si adaugam muchii pe rand folosind un DisjointSet (practic este algoritmul lui Kruskal).
 *      Odata ce am epuizat muchiile disponibile, verificam daca nodurile cerute sunt in aceeasi componenta
 *          Avantajul este ca daca sortam query-urile crescator dupa valoare, putem adauga
 *      la APM-ul deja creat si astfel evitam refacerea sa de fiecare data.
 *          Complexitatea finala este cam O(M*logN) M-muchii,N-noduri, fiind dominat de sortare.
 *      Sau poate O(Q*logN) Q-query, daca avem foarte multe query-uri pe care le verificam in aprox O(logN)
 *
 *
 */

class Solution {
    class DisjointSet {
    private:
        const int _size;
        int* _father;
        int* _rank;

        int getRoot(int el) {
            while (_father[el] != el) {
                el = _father[el];
            }
            return el;
        }
    public:
        DisjointSet(const int p_size) : _size(p_size) {
            _father = new int[_size];
            _rank = new int[_size];
            for (int i = 0; i < p_size; ++i) {
                _father[i] = i;
                _rank[i] = 1;
            }
        }
        void merge(const int x, const int y) {
            const int root_y = getRoot(y);
            const int root_x = getRoot(x);
            if (root_x == root_y) {
                return;
            }
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
            return getRoot(x) == getRoot(y);
        }
        ~DisjointSet() {
            delete[] _father;
            delete[] _rank;
        }
    };
public:
    vector<bool> distanceLimitedPathsExist(int n, vector<vector<int>>& edgeList, vector<vector<int>>& queries) {

        auto sort_cost = [](vector<int>& e1, vector<int>& e2) { return e1[2] < e2[2]; };
        for (int i = 0; i < queries.size(); ++i) {
            queries[i].push_back(i);
        }
        sort(edgeList.begin(), edgeList.end(), sort_cost);
        sort(queries.begin(), queries.end(), sort_cost);

        vector<bool> solution(queries.size());
        DisjointSet apm(n);
        auto it = edgeList.begin();
        for (auto& query : queries) {
            while (!apm.check(query[0], query[1]) && (*it)[2] < query[2]) {
                apm.merge((*it)[0], (*it)[1]);
                ++it;
            }
            solution[query[3]] = apm.check(query[0], query[1]);
        }

        return solution;
    }
};