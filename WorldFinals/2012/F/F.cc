#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <complex>
#include <cmath>
#include <cassert>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

struct Ring {
    Ring() : exists(false) {}

    bool exists;
    vector<int> rings;
    vector<char> keyA;
    vector<char> keyB;
};

pair<int, int> countRingOperationIter(int ith, vector<bool>& visited, bool& meaningful,
                                      const vector<Ring>& rs, const vi& group)
{
    visited[ith] = true;

    int cnt1 = 0, cnt2 = 0;
    for (int i = 0; i < rs[ith].rings.size(); ++i) {
        int k = rs[ith].rings[i];
        if (visited[k])
            continue;
        
        pair<int, int> p = countRingOperationIter(k, visited, meaningful, rs, group);
        cnt1 += min(p.first, p.second + 1);
        cnt2 += min(p.first + 1, p.second);
    }

    if (group[ith] == 1) {
        cnt2 = 10000;
        meaningful = true;
    }
    if (group[ith] == 2) {
        cnt1 = 10000;
        meaningful = true;        
    }

    return make_pair(cnt1, cnt2);
}

int countRingOperation(const vector<Ring>& rs, const vector<int>& group)
{
    vector<bool> visited(rs.size(), false);

    int cnt = 0;
    for (int i = 0; i < rs.size(); ++i) {
        if (rs[i].exists && !visited[i]) {
            bool meaningful = false;
            pair<int, int> p = countRingOperationIter(i, visited, meaningful, rs, group);
            if (meaningful)
                cnt += 1 + 2 * min(p.first, p.second);
        }
    }

    return cnt - 2;
}

void iter(const vector<Ring>& rs, int ith, int keyCost, 
          int ringA, int ringB,
          vector<int>& group,
          pair<int, int>& currentResult)
{
    if (ith == rs.size()) {
        if (keyCost > currentResult.first)
            return;

        int ringCost = countRingOperation(rs, group);
        if (make_pair(keyCost, ringCost) < currentResult)
            currentResult = make_pair(keyCost, ringCost);
        return;
    }

    if (keyCost > currentResult.first)
        return;

    bool tryA, tryB;
    if (ith == ringA) {
        group[ith] = 1;
        iter(rs, ith + 1, keyCost + rs[ith].keyB.size() * 2, ringA, ringB, group, currentResult);
    } else if (ith == ringB) {
        group[ith] = 2;
        iter(rs, ith + 1, keyCost + rs[ith].keyA.size() * 2, ringA, ringB, group, currentResult);
    } else if (rs[ith].keyA.size() < rs[ith].keyB.size()) {
        group[ith] = 2;
        iter(rs, ith + 1, keyCost + rs[ith].keyA.size() * 2, ringA, ringB, group, currentResult);
    } else if (rs[ith].keyB.size() < rs[ith].keyA.size()) {
        group[ith] = 1;
        iter(rs, ith + 1, keyCost + rs[ith].keyB.size() * 2, ringA, ringB, group, currentResult);
    } else if (rs[ith].keyA.empty() && rs[ith].keyB.empty()) {
        group[ith] = 0;
        iter(rs, ith + 1, keyCost + rs[ith].keyB.size() * 2, ringA, ringB, group, currentResult);
    } else {
        group[ith] = 1;
        iter(rs, ith + 1, keyCost + rs[ith].keyB.size() * 2, ringA, ringB, group, currentResult);
        group[ith] = 2;
        iter(rs, ith + 1, keyCost + rs[ith].keyA.size() * 2, ringA, ringB, group, currentResult);
    }
}

void solve(const vector<Ring>& rs)
{
    int ringCount = 0;
    for (int i = 0; i < rs.size(); ++i) {
        if (rs[i].exists)
            ringCount += 1;
    }

    // リングが１つしかなかった
    if (ringCount == 1) {
        for (int i = 0; i < rs.size(); ++i) {
            if (rs[i].exists) {
                if (!rs[i].keyA.empty() && !rs[i].keyB.empty())
                    cout << " impossible" << endl;
                else
                    cout << " 0 0" << endl;
                return;
            }
        }        
    }

    // key が一方の側しかなかった
    {
        int keyA = 0, keyB = 0;
        for (int i = 0; i < rs.size(); ++i) {
            keyA += rs[i].keyA.size();
            keyB += rs[i].keyB.size();
        }
        if (keyA == 0 && keyB == 0) {
            cout << " 0 0" << endl;
            return;
        }
        if (keyA == 0 || keyB == 0) {
            // key をくっつければ良い。
            vector<int> group(26);
            for (int i = 0; i < 26; ++i) {
                if (!rs[i].exists)
                    continue;
                if (!rs[i].keyA.empty() || !rs[i].keyB.empty())
                    group[i] = 1;
            }
            cout << " 0 " << countRingOperation(rs, group) + 1 << endl;
            return;
        }
    }

    // ring を２つ選んで、A 向け、B 向けとする。
    pair<int, int> result = make_pair(INT_MAX, INT_MAX);
    for (int a = 0; a < rs.size(); ++a) {
        if (!rs[a].exists)
            continue;
        for (int b = 0; b < rs.size(); ++b) {
            if (!rs[b].exists)
                continue;
            if (a == b)
                continue;

            vector<int> group(26);
            iter(rs, 0, 0, a, b, group, result);
        }
    }

    cout << ' ' << result.first << ' ' << result.second << endl;
}

int main(void)
{
    int caseNo = 0;
    vector<Ring> rings(26);
    
    for (string s; cin >> s; ) {
        if (s == "0") {
            for (int i = 0; i < rings.size(); ++i)
                sort(rings[i].rings.begin(), rings[i].rings.end());

            cout << "Case " << ++caseNo << ":";
            solve(rings);

            rings.clear();
            rings.resize(26);
            continue;
        }

        if (s[0] < 'a' || 'z' < s[0])
            swap(s[0], s[1]);

        rings[s[0] - 'a'].exists = true;
        if ('a' <= s[1] && s[1] <= 'z') {
            rings[s[0] - 'a'].rings.push_back(s[1] - 'a');
            rings[s[1] - 'a'].rings.push_back(s[0] - 'a');
            rings[s[1] - 'a'].exists = true;
        } else if ('A' <= s[1] && s[1] <= 'M')
            rings[s[0] - 'a'].keyA.push_back(s[1]);
        else
            rings[s[0] - 'a'].keyB.push_back(s[1]);
    }
    
    return 0;
}

