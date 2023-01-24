#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;
/*
 *      6) Easygraph
 *
 *          Problema se rezuma la determinarea unui lant maxim intr-un DAG (directed acyclic graph).
 *      Acest lucru se realizeaza printr-o sortare topologica, urmata de parcurgerea in ordinea
 *      rezultata utilizarea unui vector de distante actualizat dinamic.
 *
 *          Complexitatea este O(M+N) fiind dominata de sortarea topologica.
 */

class Graph {
private:
    int _nodes, _edges;
    vector<long long> _values;
    vector<vector<int>> _adj_list;
    vector<int> _degrees;

    vector<int> topologicalSort() {
        auto degree = _degrees;
        queue<int> nodes;
        vector<int> order;
        vector<bool> visited(_nodes + 1);
        for (int node = 1; node <= _nodes; ++node)
            if (degree[node] == 0) {
                nodes.push(node);
            }
        while (!nodes.empty()) {
            const int crt_node = nodes.front();
            for (auto course : _adj_list[crt_node]) {
                if (!visited[course]) {
                    --degree[course];
                    if (degree[course] == 0) {
                        visited[course] = true;
                        nodes.push(course);
                    }
                }
            }
            order.push_back(nodes.front());
            nodes.pop();
        }
        return order;
    }

public:
    Graph(int nodes, int edges, vector<long long> values, vector<vector<int>> adj_list, vector<int> p_degrees) :
        _nodes(nodes),
        _edges(edges),
        _values(move(values)),
        _adj_list(move(adj_list)),
        _degrees(move(p_degrees)) {}

    long long maxPath() {
        auto order = topologicalSort();
        vector<long long> distances(_nodes + 1, numeric_limits<long long>::min());
        for (const auto node : order) {
            distances[node] = max(distances[node], _values[node]);
            for (const auto neigh : _adj_list[node]) {
                distances[neigh] = max(distances[neigh], distances[node] + _values[neigh]);
            }
        }
        return *max_element(distances.begin() + 1, distances.end());
    }
};

int main() {
    ifstream in("easygraph.in");
    ofstream out("easygraph.out");
    int t;
    in >> t;
    for (int i = 0; i < t; ++i) {
        int n, m;
        in >> n >> m;
        vector<vector<int>> adj_list(n + 1);
        vector<long long> values({ 42 });
        vector<int> degrees(n + 1);
        for (int j = 0; j < n; ++j) {
            long long val;
            in >> val;
            values.emplace_back(val);
        }
        for (int j = 0; j < m; ++j) {
            int node1, node2;
            in >> node1 >> node2;
            ++degrees[node2];
            adj_list[node1].push_back(node2);
        }
        Graph grf(n, m, values, adj_list, degrees);
        out << grf.maxPath() << "\n";
    }
    in.close();
    out.close();
    return 0;
}