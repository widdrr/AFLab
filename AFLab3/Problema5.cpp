#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

/*
 *      5) Shortest Path Visiting All Nodes
 *
 *          Problema poate fi vazuta ca un superset a gasirii unui lant/ciclu hamiltonian, si anume:
 *      Daca gasim lantul cerut de problema si graphul este hamiltonian, atunci lantul gasit va fi hamiltonian
 *      din conditia de minimalitate.
 *
 *          Problema lantului hamiltonian fiind NP, singura solutie este un algoritm brute-force de tipul
 *      "incercam toate posibilitatile din ce in ce mai mari pana o gasim pe prima buna". Pentru a eficientiza
 *      acest brute-force, putem memora pentru fiecare nod, in ce stari am ajuns in acesta, unde starea
 *      este reprezentata de ce noduri am vizitat.
 *
 *          Asadar vom parcurge BFS graphul, dar nu vom merge intr-un nod cu starea curenta daca am mai fost
 *      in el cu aceeasi stare (ne-am fi invartit in cerc fara sa avansam). In momentul in care starea curenta
 *      este complica, oprim algoritmul deoarece am gasit lantul.
 *
 *          Complexitatea este un exponentiala, si anume O(2^N*N)
 *
 */

class Solution {
private:
    vector<vector<int>> _graph;
    int _nodes;
    unordered_map<int, unordered_set<vector<bool>>> _states;
    bool solution(int size, vector<bool> state) {

        if (size < _nodes - 1)
            return false;

        for (int node = 0; node < _nodes; ++node) {
            if (state[node] == false)
                return false;
        }
        return true;
    }
    int brute_force() {
        queue<tuple<int, int, vector<bool>>> perm_q;
        for (int node = 0; node < _nodes; ++node) {
            vector<bool> visited(_nodes);
            visited[node] = true;
            perm_q.emplace(node, 0, visited);
            _states[node].insert(visited);
        }
        while (!perm_q.empty()) {
            auto crt_perm = perm_q.front();
            auto node = get<0>(crt_perm);
            auto size = get<1>(crt_perm);
            auto state = get<2>(crt_perm);

            perm_q.pop();
            if (solution(size, state)) {
                return size;
            }
            for (auto nd : _graph[node]) {
                vector<bool> new_state = state;
                new_state[nd] = true;

                if (_states[nd].find(new_state) == _states[nd].end()) {
                    _states[nd].insert(new_state);
                    perm_q.emplace(nd, size + 1, new_state);
                }
            }
        }
        return -1;
    }

public:
    int shortestPathLength(vector<vector<int>>& graph) {
        _graph = move(graph);
        _nodes = _graph.size();
        return brute_force();

    }
};
