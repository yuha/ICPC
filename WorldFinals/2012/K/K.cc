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

int solve(const vvi& vss, int maxD)
{
    // 直径 d の皿で、一番上に id m のものがある場合の最小の split 数を dp[d][m] とする。
    // ここで、d を考えている場合、d を超えているものは何も考えない。
    vvi dp(maxD, vi(vss.size(), -1));
    
    for (int i = 0; i < vss.size(); ++i) {
        if (vss[i][0] == 0)
            dp[0][i] = 0;
    }

    //
    for (int d = 1; d < maxD; ++d) {
        vi pos(vss.size());
        for (int i = 0; i < vss.size(); ++i) {
            vi::const_iterator it = lower_bound(vss[i].begin(), vss[i].end(), d);
            if (it != vss[i].end() && *it == d)
                pos[i] = it - vss[i].begin();
            else
                pos[i] = -1;
        }

        // この d の上には id j の plate があり、一番下が i になるようにする。
        for (int i = 0; i < vss.size(); ++i) {
            if (pos[i] == -1) {
                dp[d][i] = -1;
                continue;
            }

            int result = INT_MAX;
            for (int j = 0; j < vss.size(); ++j) {
                if (dp[d-1][j] < 0)
                    continue;

                int numCut = 0;
                if (i == j) {
                    for (int k = 0; k < pos.size(); ++k) {
                        if (pos[k] > 0)
                            ++numCut;
                    }

                    if (numCut == 1 && pos[i] >= 0)
                        numCut = 0;
                } else {
                    for (int k = 0; k < pos.size(); ++k) {
                        if (pos[k] > 0 && k != j)
                            ++numCut;
                    }
                }
                
                if (dp[d-1][j] + numCut < result)
                    result = dp[d-1][j] + numCut;                
            }

            dp[d][i] = result;
        }
    }

    int result = INT_MAX;
    for (int i = 0; i < vss.size(); ++i) {
        if (dp[maxD-1][i] < 0)
            continue;
        if (dp[maxD-1][i] < result)
            result = dp[maxD-1][i];
    }

    return vss.size() + result * 2 - 1;
}

int main(void)
{
    int caseNo = 0;
    for (int n; cin >> n; ) {
        set<int> ds;
        vvi vss(n);
        for (int i = 0; i < n; ++i) {
            int h; cin >> h;
            vi vs(h);
            for (int j = 0; j < h; ++j) {
                cin >> vs[j];
                ds.insert(vs[j]);
            }

            vss[i] = vs;
        }

        // 直径を１違いになるように付け替える。
        map<int, int> ms;
        int maxId = 0;
        for (set<int>::iterator it = ds.begin(); it != ds.end(); ++it)
            ms[*it] = maxId++;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < vss[i].size(); ++j) {
                vss[i][j] = ms[vss[i][j]];
            }
        }

        cout << "Case " << ++caseNo << ": " << solve(vss, maxId) << endl;
    }
    
    return 0;
}

