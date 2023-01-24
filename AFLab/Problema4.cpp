#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

/*
 *      4) Componente Tare Conexe
 *      Solultia este implementarea proprie a Algoritmului lui Tarjan
 *          (as fi vrut eu sa ma fi gandit singur la asta..)
 *      Algoritmul foloseste o structura de date auxiliara(am folosit o stiva) pentru a memora
 *          nodurile asa cum sunt gasite in parcurgerea DFS. Astfel in orice moment dat stim
 *          ca putem ajunge dintr-un anumit nod la toate nodurile care apar dupa el in stiva.
 *          Pentru a determina conectivitatea "in celalalt sens", se folosesti o valoare "lowlink"
 *          care determina "primul" nod in care putem ajunge din nodul curent
 *          (nodurile sunt indexate cand se intalnesc). Ea se modifica in timpul parcurgerii
 *          facandu-se minimul intre lowlinkurile nodurile adiacente*(1). Cand dam de un nod
 *          care are lowlink = index (cu alte cuvinte, nu putem ajunge "mai sus" din el), am
 *          gasit o componenta formata din el si toate nodurile care urmeaza pe stiva.
 *      (1) Original in paperul sau, Tarjan modifica diferit lowlink intr-un anumit caz.
 *          Este pur si simplu pentru corectitudinea matematica, in practica implementarea de mai
 *          jos functioneaza la fel de corect(deoarece se initializeaza lowlink cu index)
 *
 *      Algoritmul lui Tarjan are complexitate O(n+m) cu n-noduri, m-muchii, deoarece
 *      efectueaza o singura parcurgere DFS.
 *
 */

class Graph {
private:
    int _nodes, _edges;
    vector<vector<int>> _adj_list;
    /*here for convenience*/
    int _crt_index = 1;
    stack<int> _comp_stack;
    vector<vector<int>> _components;
    vector<bool> _onStack;
    vector<int> _index;
    vector<int> _lowlink;

    void dfs(const int start){
        _index[start] = _crt_index++;
        _lowlink[start] = _index[start];
        _comp_stack.push(start);
        _onStack[start] = true;
        for (const auto node : _adj_list[start]) {
            if (!_index[node]) {
                dfs(node);
                _lowlink[start] = min(_lowlink[start], _lowlink[node]);
            }
            if(_onStack[node]){
                _lowlink[start] = min(_lowlink[start], _lowlink[node]);
            }
        }
        if(_lowlink[start]==_index[start]){
            vector<int> component;
            while (_comp_stack.top() != start){
                const int node = _comp_stack.top();
                _comp_stack.pop();
                _onStack[node] = false;
            	component.push_back(node);

            }
            component.push_back(start);
            _onStack[start] = false;
            _comp_stack.pop();
            _components.push_back(move(component));
        }
    }

public:
    Graph(int nodes, int edges, vector<vector<int>> adj_list) :
        _nodes(nodes),
        _edges(edges),
        _adj_list(std::move(adj_list)) {}

    vector<vector<int>> stronglyConnected(){
        _crt_index = 1;
        _onStack = vector<bool>(_nodes+1,false);
        _index = vector<int>(_nodes + 1, 0);
        _lowlink = vector<int>(_nodes + 1, 0);

        for(int node = 1;node<=_nodes;++node){
	        if(!_index[node]){
                dfs(node);
	        }
        }
        return _components;
    }
};

// int main(){
//     ifstream in("ctc.in");
//     int nodes, edges;
//     in >> nodes >> edges;
//     vector<vector<int>> adj_list(nodes+1);
//     for(int i=0; i<edges; ++i)
//     {
//         int node1, node2;
//         in >> node1 >> node2;
//         adj_list[node1].push_back(node2);
//     }
//     in.close();
//     ofstream out("ctc.out");
//     Graph g(nodes, edges, adj_list);
//     auto stronglyConnected = g.stronglyConnected();
//     out << stronglyConnected.size()<<"\n";
//     for(auto comp : stronglyConnected){
// 	    for(auto node : comp){
//             out << node << " ";
// 	    }
//         out << "\n";
//     }
//
// }