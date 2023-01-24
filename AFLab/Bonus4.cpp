#include <fstream>
#include <vector>

using namespace std;

class Graph {
private:
    int _nodes, _edges;
    vector<vector<int>> _adj_list;
    vector<int> father;
    bool dfs(const int start, vector<bool>& visited){
        visited[start] = true;
        int crossroad_factor=0;
        for (auto node : _adj_list[start]) {
            if (_adj_list[node].size() >= 2) {
                ++crossroad_factor;
            }
            if (crossroad_factor >= 3){
                return false;
			}
        	if (!visited[node]) {
                father[node] = start;
                if (!dfs(node, visited))
                    return false;
            }
            else if(node != father[start])
                return false;
        }
        return true;
    }

public:
    Graph(int nodes, int edges, vector<vector<int>> adj_list) :
        _nodes(nodes),
        _edges(edges),
        _adj_list(std::move(adj_list)) {}

    bool findLesbulan(){
        vector<bool> visited(_nodes + 1, false);
        father = vector<int>(_nodes + 1, 0);
        for(int node = 1; node<=_nodes;++node){
	        if(!visited[node]){
                if (!dfs(node, visited))
                    return false;
	        }
        }
        return true;
    }
};

// int main(){
//     ifstream in("lesbulan.in");
//     ofstream out("lesbulan.out");
//     int bunker_count;
//     in >> bunker_count;
//     for(;bunker_count;--bunker_count)
//     {
//         int nodes, edges;
//         in >> nodes>>edges;
//         vector<vector<int>> adj_list(nodes+1);
//         for (int j = 0; j < edges;++j){
//             int node1, node2;
//             in >> node1 >> node2;
//             adj_list[node1].push_back(node2);
//             adj_list[node2].push_back(node1);
//         }
//         Graph bunker(nodes, edges, adj_list);
//         out << bunker.findLesbulan() << "\n";
//     }
//     in.close();
//     out.close();
// }