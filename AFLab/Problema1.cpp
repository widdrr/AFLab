#include <vector>
using namespace std;
/*
 *      1) Possible Bipartition
 *
 *      Problema se reduce la a determina daca graful dat poate fi bipartit sau nu.
 *      Pentru a determina acest lucru parcurgem DFS graful si incercam sa punem fiecare
 *              nod in 1 din 2 grupuri, respectand conditia ca doua noduri adiacente sa nu fie in acelasi grup.
 *      Daca in urma algoritmului rezulta doua noduri adiacente in acelasi grup, inseamna ca bipartitia este imposibila
 *      b) rezolvand a), determin deja grupurile asa ca la b) doar le returnez.
 *
 *      Complexitatea algoritmului este O(n+m), unde n-noduri, m-muchii, deoarece se
 *          efectueaza o singura parcurgere DFS.
 *
 */
class Solution {
private:
	vector<vector<int>> _dislikes;

    bool dfsPartition(const int start, vector<int>& groups) {
        for (const auto node : _dislikes[start]) {
            if (!groups[node]) {
                groups[node] = groups[start] == 1 ? 2 : 1;
                if (!dfsPartition(node, groups))
                    return false;
            }
            else if (groups[node] == groups[start]) {
                return false;
            }
        }
        return true;

    }


public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        vector<vector<int>> adj_list(n + 1);
        for (auto edge : dislikes){
            adj_list[edge[0]].push_back(edge[1]);
            adj_list[edge[1]].push_back(edge[0]);
        }

        _dislikes = adj_list;
        vector<int> groups(n + 1, 0);
        for (int i = 1; i <= n; ++i){
            if (!groups[n]) {
                groups[n] = 1;
                if (!dfsPartition(n, groups))
                    return false;
            }
        }
        return true;
    }
    vector<vector<int>> possibleBipartitionB(int n, vector<vector<int>>& dislikes)
    {
        vector<vector<int>> adj_list(n + 1);
        for (auto edge : dislikes) {
            adj_list[edge[0]].push_back(edge[1]);
            adj_list[edge[1]].push_back(edge[0]);
        }
        _dislikes = adj_list;
        vector<int> groups(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (!groups[n]) {
                groups[n] = 1;
                if (!dfsPartition(n, groups))
                    return {};
            }
        }
        vector<vector<int>> bipartition(2);
        for (int node = 1; node <= n; ++node)
            bipartition[groups[node] - 1].push_back(node);
        return bipartition;
    }
};
