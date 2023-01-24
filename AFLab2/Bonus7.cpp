#include <vector>
#include <queue>

using namespace std;

/*
 *      Bonus 7) ... K stops
 *          Parcurg graphul conform Dijkstra, dar populez o matrice cu valori
 *      care au semnificatia _distances[i][node] = distanta celui mai mic drum
 *      de la sursa la nod, in cel mult i stopuri.
 *          De-a lungul parcurgerii modific dinamic valorile iar la final,
 *      raspunsul _distances[k][destinatie].
 *          Complexitatea nu este ideala, fiind O(MNLogN) worst case, M-muchii, N-noduri.
 *
 */

class Solution {
private:
    struct edge {
        int id;
        int dist;
        int stops;

        edge(int p_id, int p_dist, int p_stops) : id(p_id), dist(p_dist), stops(p_stops) {}

        bool operator>(const edge& rhs) const {
            return this->dist > rhs.dist;
        }
    };
    vector<vector<edge>> _adj_list;
    vector<vector<int>> _distances;
    priority_queue <edge, vector<edge>, greater<>> _edge_heap;
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        _adj_list = vector<vector<edge>>(n);
        _distances = vector<vector<int>>(k + 1, vector<int>(n, numeric_limits<int>::max()));


        for (auto flight : flights) {
            _adj_list[flight[0]].emplace_back(flight[1], flight[2], 42);
        }

        int crt_node = src;
        for (auto& dist : _distances) {
            dist[crt_node] = 0;
        }
        for (auto& node : _adj_list[crt_node]) {
            for (int i = 0; i <= k; ++i) {
                bool flag = false;
                if (_distances[i][node.id] > _distances[0][crt_node] + node.dist) {
                    _distances[i][node.id] = _distances[0][crt_node] + node.dist;
                    if (!flag) {
                        _edge_heap.emplace(node.id, _distances[0][node.id], 0);
                        flag = true;
                    }
                }
            }
        }
        while (crt_node != dst && !_edge_heap.empty()) {
            crt_node = _edge_heap.top().id;
            int stops = _edge_heap.top().stops;
            _edge_heap.pop();

            for (auto& node : _adj_list[crt_node]) {
                for (int i = stops + 1; i <= k; ++i) {
                    bool flag = false;
                    if (_distances[i][node.id] > _distances[stops][crt_node] + node.dist) {
                        _distances[i][node.id] = _distances[stops][crt_node] + node.dist;
                        if (!flag) {
                            _edge_heap.emplace(node.id, _distances[stops + 1][node.id], stops + 1);
                            flag = true;
                        }
                    }
                }
            }
        }

        int min_dist = numeric_limits<int>::max();
        for (int i = 0; i <= k; ++i) {
            min_dist = min(min_dist, _distances[i][dst]);
        }

        return min_dist == numeric_limits<int>::max() ? -1 : min_dist;
    }
};