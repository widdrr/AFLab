#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>

using namespace std;

/*
 *      2) Check DFS
 *
 *      Pentru a rezolva problema, incerc sa parcurg graful in ordinea stabilita de permutare.
 *      Incepand cu nodul 1, verific daca urmatorul nod din permutare este adiacent cu nodul curent:
 *      Daca da, trecem la acel nod si continuam parcurgerea.
 *      Daca nu, mai intai verificam daca nodul curent are vreun nod adiacent nevizitat:
 *          -daca are, inseamna ca parcurgerea data este imposibila, deoarece acel nod ar fi trebuit sa fie urmatorul
 *          -daca toate nodurile adiacente au fost vizitate, ne intoarcem in parcurgere la nodurile anterioare
 *      Daca parcurgerea se incheie cu success(intreg graful a fost parcurs), permutarea este buna.
 *
 *      Complexitatea este O(n+m) unde n-noduri, m-muchii, deoarece in cazul in care permutarea
 *          este buna, se efectueaza o parcurgere DFS completa. In caz contrar, algoritmul inceteaza
 *          prematur.
 *
 */
class Graph {
private:
    int _nodes, _edges;
    vector<unordered_set<int>> _adj_list;

public:
    Graph(int nodes, int edges, vector<unordered_set<int>> adj_list) :
        _nodes(nodes),
        _edges(edges),
        _adj_list(std::move(adj_list)) {}

    bool checkDFS(vector<int>& permutation) {
        stack<int> node_stack;
        vector<bool> visited(_nodes + 1, false);

    	node_stack.push(1);
        visited[1] = true;

        auto it = ++permutation.begin();
        while (!node_stack.empty())
        {
            int node = node_stack.top();
            //if only I had c++20 and could use .contains()...
            if (it != permutation.end() && _adj_list[node].find(*it) != _adj_list[node].end())
            {
                node_stack.push(*it);
                visited[*it] = true;
                ++it;
            }
            else {
                for (auto adj_node : _adj_list[node])
                    if (!visited[adj_node])
                        return false;
                node_stack.pop();
            }
        }
        return it == permutation.end();
    }

};

// int main() {
//
//     int nodes, edges;
//     vector<int> permutation;
//
//     cin >> nodes >> edges;
//
//     vector<unordered_set<int>> adj_list(nodes + 1);
//     for (int i = 0; i < nodes; ++i) {
//         int node;
//         cin >> node;
//         permutation.push_back(node);
//     }
//     for (int i = 0; i < edges; ++i) {
//         int node1, node2;
//         cin >> node1 >> node2;
//
//         adj_list[node1].insert(node2);
//         adj_list[node2].insert(node1);
//     }
//
//     Graph g(nodes, edges, adj_list);
//
//     cout << g.checkDFS(permutation);
//     return 0;
// }