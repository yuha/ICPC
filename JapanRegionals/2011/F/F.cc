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

int solve(const vector<string>& vs)
{
    const int N = vs.size();

    // 都市 i -> j への距離のマップを作っておく
    // i: AAABB  j: BBCCC であれば、距離を CCC の長さの 3 とする。
    vvi dist(N, vi(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int len = min(vs[i].length(), vs[j].length());
            for (int k = 0; k < len; ++k) {
                if (vs[i].substr(vs[i].size() - k, k) == vs[j].substr(0, k))
                    dist[i][j] = vs[j].length() - k;
            }
        }
    }

    const int NOT_VISITED = INT_MAX / 2;
    // 訪れた都市の集合 * 最後に訪れた都市 -> 最も短い長さ
    vvi dp(1 << N, vi(N, NOT_VISITED));

    // 初期状態の作成
    for (int i = 0; i < N; ++i)
        dp[1 << i][i] = vs[i].size();

    for (int k = 0; k < (1 << N); ++k) {
        for (int i = 0; i < N; ++i) {            
            if (dp[k][i] == NOT_VISITED)
                continue;

            for (int j = 0; j < N; ++j)
                dp[k | (1 << j)][j] = min(dp[k | (1 << j)][j], dp[k][i] + dist[i][j]);
        }
    }

    return *min_element(dp[(1 << N) - 1].begin(), dp[(1 << N) - 1].end());
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<string> cities(N);
        for (int i = 0; i < N; ++i)
            cin >> cities[i];

        // 完全部分文字列がないかどうかだけ見ておく
        int visited = 0;       
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == j)
                    continue;
                if (cities[i].find(cities[j]) != string::npos)
                    visited |= (1 << j);
            }
        }

        vector<string> reduced;
        for (int i = 0; i < N; ++i) {
            if (!((1 << i) & visited))
                reduced.push_back(cities[i]);
        }

        cout << solve(reduced) << endl;
    }
    
    return 0;
}

