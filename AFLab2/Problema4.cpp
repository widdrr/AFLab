#include <vector>
#include <queue>
using namespace std;

/*
 *      4) Path of Maximum Probability
 *
 *          Trebuie determinat drumul care maximizeaza probabilitatea.
 *      Acest lucru se face folosind algoritmul Dijkstra , modificat pentru a maximiza
 *      produsul muchiilor. Implementarea este standard, folosind un max-heap.
 *
 *          Complexitatea este O(M*logN).
 */
class Solution {
private:
    struct edge{
        int id;
        double prob;

        edge(int p_id, double p_prob) : id(p_id), prob(p_prob) {}

        bool operator<(const edge& rhs) const{
            return this->prob < rhs.prob;
        }
    };
    vector<vector<edge>> _adj_list;
    vector<double> _path_probs;
    priority_queue <edge,vector<edge>,less<>> _edge_heap;
public:
    double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start, int end) {
        _adj_list = vector<vector<edge>>(n);
        _path_probs = vector<double>(n,0);

    	for(int i=0; i < edges.size(); ++i){
            int node1 = edges[i][0];
            int node2 = edges[i][1];
            double prob = succProb[i];

    		_adj_list[node1].emplace_back(node2,prob);
            _adj_list[node2].emplace_back(node1, prob);
        }

        int crt_node = start;
        _path_probs[start] = 1;

        while(crt_node != end){
            for (auto& neigh : _adj_list[crt_node])
                if(_path_probs[neigh.id] < _path_probs[crt_node] * neigh.prob){
                    _path_probs[neigh.id] = _path_probs[crt_node] * neigh.prob;
                    _edge_heap.emplace(neigh.id, _path_probs[neigh.id]);
				}
            if (_edge_heap.empty())
                break;
            crt_node = _edge_heap.top().id;
            _edge_heap.pop();
        }
        return _path_probs[end];
    }
};