#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
 *      4) Valid arrangement of pairs
 *
 *          Problema se traduce in gasirea unui lant sau ciclu eulerian pe un graph orientat.
 *      Determinam cele doua noduri cu grade diferite de intrare si iesire, deoarece ele vor fi
 *      inceputul si finalul lantului. Daca nu exista, nu conteaza de unde incepem deoarece vom avea un ciclu.
 *
 *          Algoritmul folosit este o implementare recursiva a algoritmului lui Hierholzer, practic o
 *      parcurgere DFS, dar adaugam muchiile parcurse in postordine. Acest lucru ne asigura un lant corect
 *      si daca graphul nu este eulerian, dar la final trebuie sa inversam parcurgerea.
 *
 *          Complexitatea este O(N+M)
 *
 */

using namespace std;
class Solution {
private:
    unordered_map<int, unordered_set<int>> _graph;
    vector<vector<int>> _arrangement;
    unordered_map<int, int> _degrees;
    void computePath(const int start) {

    	while (!_graph[start].empty()) {
            int node = *_graph[start].begin();
            _graph[start].erase(node);
            computePath(node);
            _arrangement.push_back({ start, node });
        }
    }
public:
    vector<vector<int>> validArrangement(vector<vector<int>>& pairs) {
        int start = pairs[0][0];
        for (const auto& pair : pairs) {
            _graph[pair[0]].insert(pair[1]);
            ++_degrees[pair[0]];
            --_degrees[pair[1]];
        }
        for (const auto& degree : _degrees) {
            if (degree.second > 0) {
                start = degree.first;
            }
        }
        computePath(start);
        reverse(_arrangement.begin(), _arrangement.end());
        return _arrangement;
    }
};
