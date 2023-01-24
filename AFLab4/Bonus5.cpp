#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

/*
 *      Bonus 5) Matching-Substrings
 *
 *          Impartim fiecare cuvant intr-un prefix(fara ultima litera) si un sufix (fara prima litera)
 *      Construim un graph orientat din sufixe si prefixe cu muchie daca fac parte din acelasi cuvant, pe muchie fiind
 *      litera care se adauga la prefix pentru a obtine sufixul.
 *          Cautand un lant eulerian in graphul rezultat, obtinem secventa cerunta. Inainte de asta, avem grija
 *      sa numaram gradele nodurilor pentru e detecta daca exista sau nu un lant si eventual sa-i determinam startul
 *
 *          Complexitatea va fi O(N+M) fiind un DFS postorder
 */

void euclideanPath(string start, unordered_map<string, vector<pair<string,char>>>& graph, vector<char>& path) {
    while (!graph[start].empty()) {
        auto node = graph[start].back().first;
        auto letter = graph[start].back().second;
        graph[start].pop_back();
        euclideanPath(node, graph, path);
        path.push_back(letter);
    }
}

// int main() {
//     int n, k;
//
//     cin >> n >> k;
//     unordered_map<string, vector<pair<string, char>>> graph;
//     unordered_map<string, int> in_deg;
//     unordered_map<string, int> out_deg;
//     for (int i = 0; i < n; ++i) {
//         string word;
//         cin >> word;
//
//         string pref = word.substr(0, k - 1);
//         string suf = word.substr(1);
//         char letter = k != 1 ? suf.back() : word[0];
//         graph[pref].emplace_back(suf, letter);
//         if (in_deg.find(pref) == in_deg.end())
//             in_deg[pref] = 0;
//         if (out_deg.find(suf) == out_deg.end())
//             out_deg[suf] = 0;
//         ++in_deg[suf];
//         ++out_deg[pref];
//     }
//     int deg_counter = 0;
//     string start = graph.begin()->first;
//     for (auto& keyval : out_deg) {
//         auto str = keyval.first;
//         auto deg = keyval.second;
//
//         if (deg != in_deg[str]) {
//             ++deg_counter;
//             if (deg_counter > 2) {
//                 cout << -1;
//                 return 0;
//             }
//             if (deg > in_deg[str])
//                 start = str;
//         }
//     }
//     vector<char> sol;
//     euclideanPath(start, graph, sol);
//     for (auto it = start.rbegin(); it != start.rend(); ++it) {
//         sol.emplace_back(*it);
//     }
//     if (sol.size() != n + k - 1){
//         cout << -1;
//         return 0;
//     }
//     reverse(sol.begin(), sol.end());
//     for (auto& letter : sol)
//         cout << letter;
// }
