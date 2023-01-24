#include <vector>
#include <string>

using namespace std;

/*
 *      6)
 *      a) Longest Common Subsequence
 *
 *          Lungimea celei mai mari subsecvente comune (lcs) se poate determina dinamic, obtinandu-se o matrice
 *      cu lungimile subsecventelor comune intre toate prefixele(inclusiv multimea vida) celor doua cuvinte.
 *          Matricea este populata in felul urmator:
 *          -lcs[0][x] = lcs[x][0] = 0 (toate valorile pe prima linie si coloana sunt 0)
 *          -pentru fiecare pozitie (i) din cuv1, iteram (j) prin cuv2:
 *              -daca cuv1[i] == cuv2[j], atunci lcs[i+1][j+1]= 1 + lcs[i][j] (avem o noua litera comuna)
 *              -daca cuv1[i] != cuv2[j], atunci lcs[i+1][j+1] = max(lcs[i][j+1],lcs[i+1][j])
 *               (maximul dintre lungimile pentru perechile de prefixele adiacente)
 *
 *          Complexitatea este O(L1*L2) L1,L2- lungimile cuvintelor
 *
 *      b) Shortest Common Supersequence
 *
 *          Lungimea celei mai mici supersecvente comune (scs) este inrudita cu lungimea lcs, in felul
 *      urmator: scs = l1 + l2 - lcs.
 *          Pentru a determina secventa in sine si nu doar lungimea ei, ne putem folosi de matricea lcs.
 *          Construim supersecventa (invers) in felul urmator:
 *          -parcurgem (i,j) ambele cuvinte invers(deoarece matricea lucreaza cu prefixe)
 *          -daca unul dintre cuvinte s-a terminat, evident punem doar literele celuilalt
 *          -daca cuv1[i] == cuv[j], punem litera comuna si decrementam ambii indici
 *          -daca cuv1[i] != cuv[j], determinam ce litera trebuie pusa in felul urmator:
 *              -daca lcs[i][j+1] >= lcs[i+1][j], atunci punem cuv1[i] si decrementam i
 *              -altfel, punem cuv2[j] si decrementam j.
 *              (acest lucru corespunde cu a vedea care dintre litere apare in lcs, daca niciuna nu apare
 *               atunci e irelevant pe care o punem)
 *
 *          Complexitatea este O(L1*L2) deoarece trebuie sa facem lcs mai intai.
 *
 *
 *
 */

class Solution {
private:
    vector<vector<int>> _prefixes;
    int longestCommonSubsequence(string& text1, string& text2) {
        _prefixes = vector<vector<int>>(text1.size() + 1, vector<int>(text2.size() + 1, 0));

        for (int t1_i = 0; t1_i < text1.size(); ++t1_i) {
            for (int t2_i = 0; t2_i < text2.size(); ++t2_i) {
                if (text1[t1_i] == text2[t2_i]) {
                    _prefixes[t1_i + 1][t2_i + 1] = 1 + _prefixes[t1_i][t2_i];
                }
                else {
                    _prefixes[t1_i + 1][t2_i + 1] = max(_prefixes[t1_i + 1][t2_i], _prefixes[t1_i][t2_i + 1]);
                }
            }
        }
        return _prefixes[text1.size()][text2.size()];
    }
public:
    string shortestCommonSupersequence(string str1, string str2) {
        int s1_i = str1.size() - 1;
        int s2_i = str2.size() - 1;
        string scs(str1.size() + str2.size() - longestCommonSubsequence(str1, str2), '*');
        int scs_i = scs.size() - 1;
        while (scs_i >= 0) {

            if (s1_i < 0) {
                scs[scs_i] = str2[s2_i];
                --s2_i;
            }
            else if (s2_i < 0) {
                scs[scs_i] = str1[s1_i];
                --s1_i;
            }
            else if (str1[s1_i] != str2[s2_i]) {
                if (_prefixes[s1_i][s2_i + 1] >= _prefixes[s1_i + 1][s2_i]) {
                    scs[scs_i] = str1[s1_i];
                    --s1_i;
                }
                else {
                    scs[scs_i] = str2[s2_i];
                    --s2_i;
                }
            }
            else {
                scs[scs_i] = str1[s1_i];
                --s1_i;
                --s2_i;
            }
            --scs_i;
        }
        return scs;
    }
};