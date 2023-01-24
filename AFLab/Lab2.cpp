#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
private:
    int _nodes, _edges;
    vector<vector<int>> _adj_list;
    vector<int> _in_deg;

public:
	Graph(int nodes, int edges, vector<vector<int>> adj_list,vector<int> in_deg) :
        _nodes(nodes),
        _edges(edges),
        _adj_list(std::move(adj_list)),
		_in_deg(std::move(in_deg)) {}
    vector<int> findOrderV1() {
        vector<int> order;
		vector<bool> visited(_nodes +1, false);
        queue<int> nodes;
        for (int node = 1; node <= _nodes; ++node)
            if (_in_deg[node] == 0) {
                nodes.push(node);
                visited[node] = true;
            }
        while (!nodes.empty()) {
            const int crt_course = nodes.front();
            for (auto course : _adj_list[crt_course]) {
                if (!visited[course]) {
                    --_in_deg[course];
                    if (_in_deg[course] == 0) {
                        visited[course] = true;
                        nodes.push(course);
                    }
                }
            }
            order.push_back(nodes.front());
            nodes.pop();
        }
        if (order.size() == _nodes)
            return order;
        else return {};
    }
};
// int main(){
//     ifstream in("sortaret.in");
//     int nodes, edges;
//     in >> nodes >> edges;
//     vector<vector<int>> adj_list(nodes+1);
//     vector<int> in_deg(nodes+1,0);
//     for(int i=0; i<edges;++i){
//         int n1, n2;
//         in >> n1 >> n2;
//         adj_list[n1].push_back(n2);
//         ++in_deg[n2];
//     }
//     in.close();
//     ofstream out("sortaret.out");
//     Graph g(nodes, edges, adj_list, in_deg);
//     auto sol = g.findOrderV1();
//     for (auto node : sol)
//         out << node << " ";
//     out.close();
//
// }