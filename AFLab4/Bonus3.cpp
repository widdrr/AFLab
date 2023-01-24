#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

/*
 *      Bonus 3) Accounts Merge
 *
 *          Daca fiecare Mail este un Nod si avem muchii intre Mailurile care apartin aceluiasi cont,
 *      problema se reduce la a determina componentele conexe.
 *
 *      Complexitatea este O(N+M).
 *
 *
 */

class Solution {
    unordered_map<string, unordered_set<string>> _graph;
    unordered_set<string> _visited;

    void dfs(string start, vector<string>& mails) {

        if (_visited.find(start) != _visited.end())
            return;
        mails.push_back(start);
        _visited.insert(start);
        for (auto& mail : _graph[start])
            dfs(mail, mails);
    }
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {

        vector<vector<string>> merges;
        for (auto& mails : accounts) {
            for (int i = 1; i < mails.size() - 1; ++i) {

                _graph[mails[i]].insert(mails[i + 1]);
                _graph[mails[i + 1]].insert(mails[i]);
            }
        }
        for (auto& mails : accounts) {
            for (int i = 1; i < mails.size(); ++i) {
                if (_visited.find(mails[i]) != _visited.end())
                    continue;
                merges.push_back({ mails[0] });
                dfs(mails[i], merges.back());
                sort(merges.back().begin() + 1, merges.back().end());
            }
        }
        return merges;
    }
};