#include <vector>
#include <queue>
using namespace std;
/*
 *      3) Course Schedule
 *      Pe baza datelor de intrare se construieste un graf orientat
 *      In acest graf, gradul interior al unui nod determina numarul de alte cursuri de care depinde.
 *      Algoritmul este urmatorul:
 *          -toate nodurile de grad 0 se pun in coada
 *          -se ia capul cozii si pentru toate nodurile adiacente cu el(care depind de el), se scade gradul
 *          -orice nod care ajung le grad 0 este introdus in coada
 *          -se scoate nodul din coada si se introduce in vectorul de ordine, care reprezinta solutia
 *          -daca la final, vectorul de ordine contine toate nodurile, atunci este o solutie.
 *      b) Dat fiind ca la a) rezolvarea mea nu gaseste automat ciclurile, b) necesita alta solutie:
 *          -parcurg dfs graphul intr-o ordine oarecare
 *          -daca in orice moment ajungem iar intr-un nod vizitat in "parcurgerea" curenta
 *          (se face diferenta intre nodurile deja vizitate anterior in alta "parcurgere" si cele vizitate in cea curenta)
 *          ,atunci am descoperit un ciclu, pe care ar trebui sa-l gasim in stiva(sau recursie)
 *
 *      Ambele subpuncte au complexitate O(n+m) cu n-noduri, m-muchii, deoarece b) este o parcurgere dfs
 *          ,iar a) parcurge toate nodurile, iar pentru fiecare nod, scade gradul tuturor vecinilor.
 */
class Solution {
private:
    int _nodes;
    vector<vector<int>> _adj_list;
    vector<bool> _taken;
    vector<int> _order;

    bool dfs(const int start, vector<bool>& visited) {
        for (const auto node : _adj_list[start]) {
            if (!_taken[node]) {
                if (visited[node]) {
                    _order.push_back(node);
                    _order.push_back(start);
                    return false;
                }
                visited[node] = true;
                if (!dfs(node, visited)) {
                    _order.push_back(start);
                    return false;
                }
            }
        }
        _taken[start] = true;
        return true;
    }
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj_list(numCourses);
        std::vector<int> in_deg(numCourses);
        for (auto edge : prerequisites) {
            adj_list[edge[1]].push_back(edge[0]);
            ++in_deg[edge[0]];
        }
        _nodes = numCourses;
        _adj_list = adj_list;
        _taken = vector<bool>(numCourses, false);
        queue<int> nodes;
        for (int node = 0; node < numCourses; ++node)
            if (in_deg[node] == 0) {
                nodes.push(node);
                _taken[node] = true;
            }
        while (!nodes.empty()) {
            int crt_course = nodes.front();
            for (auto course : _adj_list[crt_course]) {
                if (!_taken[course]) {
                    --in_deg[course];
                    if (in_deg[course] == 0) {
                        _taken[course] = true;
                        nodes.push(course);
                    }
                }
            }
            _order.push_back(nodes.front());
            nodes.pop();
        }
        if (_order.size() == numCourses)
            return _order;
        else return {};
    }
    vector<int> findOrderB(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> adj_list(numCourses);
        for (auto edge : prerequisites) {
            adj_list[edge[0]].push_back(edge[1]);
        }
        _taken = vector<bool>(numCourses, false);
        _nodes = numCourses;
        _adj_list = adj_list;

        for (int node = 0; node < numCourses; ++node) {
            if (!_taken[node]) {
                vector<bool> visited(numCourses, false);
            	visited[node]=true;
                if (!dfs(node, visited)) {
                    reverse(_order.begin(),_order.end());
                    return _order;
                }
            }
        }
        return _order;
    }
};

