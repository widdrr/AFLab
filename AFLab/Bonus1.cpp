#include <vector>

using namespace std;


/*
 *      Bonus 1) Critical Connections
 *      Daca fiecarui nod i se atribuie un indice si se memoreaza indicele minim
 *          la care putem ajunge in subarborele sau prin parcurgerea DFS, atunci
 *          o muchie critica se gaseste intre un nod din care nu putem ajunge "mai sus"
 *          si tatal sau.
 *
 *      Complexiatea este O(n+m) cu n-noduri, m-muchii, doarece se face o singura parcurgere DFS.
 */

class Solution {
private:
    int _nodes;
    vector<vector<int>> _adj_list;
    vector<vector<int>> _critical_connect;
    vector<int> _index;
    vector<int> _lowlink;
    vector<int> _father;
    int _crt_index = 1;

    void dfs(const int start) {
        _index[start] = _crt_index++;
        _lowlink[start] = _index[start];

        for (auto node : _adj_list[start]) {
            if (!_index[node]) {
                _father[node] = start;
                dfs(node);
                _lowlink[start] = min(_lowlink[start], _lowlink[node]);

                if (_lowlink[node] == _index[node]) {
                    _critical_connect.push_back({ start,node });
                }
            }
            else if (node != _father[start])
                _lowlink[start] = min(_lowlink[start], _index[node]);
        }
    }

public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        _nodes = n;
        vector<vector<int>> adj_list(n);

        for (auto edge : connections) {
            adj_list[edge[0]].push_back(edge[1]);
            adj_list[edge[1]].push_back(edge[0]);
        }

        _adj_list = adj_list;
        _crt_index = 1;

        _index = vector<int>(n, 0);
        _lowlink = vector<int>(n, 0);
        _father = vector<int>(n, -1);

        for (int i = 0; i < n; ++i) {
            if (!_index[i]) {
                dfs(i);
            }
        }
        return _critical_connect;

    }
};