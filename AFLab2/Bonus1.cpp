#include <unordered_map>
#include <vector>

using namespace std;

/*
 *      Bonus 1) Min Cost To Connect All Paths
 *          Problema reprezinta realizarea unui APM intre punctele date, muchiile
 *      avand distanta manhattan.
 *          Deoarece graphul este complet, Prim in versiunea O(N^2) optim.
 */

class Solution {
    long long manhattan(vector<int>& p1, vector<int>& p2) {
        return abs(1LL * p1[0] - p2[0]) + abs(1LL * p1[1] - p2[1]);
    }
    int getMin(unordered_map<int, long long>& p_dist) {
        long long min_dist = numeric_limits<long long>::max();
        int min_pt;
        for (auto& pt : p_dist) {
            if (pt.second < min_dist) {
                min_dist = pt.second;
                min_pt = pt.first;
            }
        }
        return min_pt;
    }
public:
    int minCostConnectPoints(vector<vector<int>>& points) {
        unordered_map<int, long long> _distances;
        vector<bool> _visited;

        for (unsigned int i = 0; i < points.size(); ++i) {
            _distances[i] = numeric_limits<long long>::max();
            _visited.emplace_back(false);
        }
        auto first = *points.begin();
        _visited[0] = true;
        for (unsigned int i = 0; i < points.size(); ++i) {
            if (!_visited[i])
                _distances[i] = min(_distances[i], manhattan(first, points[i]));
        }


        unsigned int connected = 1;
        unsigned int cost = 0;
        while (connected < points.size()) {
            auto crt = getMin(_distances);
            cost += _distances[crt];
            _distances.erase(crt);
            _visited[crt] = true;
            ++connected;
            for (unsigned int i = 0; i < points.size(); ++i) {
                if (!_visited[i])
                    _distances[i] = min(_distances[i], manhattan(points[crt], points[i]));
            }
        }
        return cost;
    }
};
