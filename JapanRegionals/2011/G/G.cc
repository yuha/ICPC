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

int dh[] = { -1, -1, -1,  0, 0, 0,  1, 1, 1 };
int dw[] = { -1,  0,  1, -1, 0, 1, -1, 0, 1 };

struct State {
    vi seen; // 書く数字において、見たマスの状態 左上から右下に向かって下位ビットを割り振る
    vi leftChests; // 上の状態で、あと何個財宝をおけるか

    State(int n) : seen(n), leftChests(n) { }

    int freeSpaceOf(int n) const {
        int result = 9;
        for (int i = 0; i < 9; ++i) {
            if (seen[n] & (1 << i))
                --result;
        }

        return result;
    }

    bool putChest(int w, int h, bool chest, const vector<pair<int, int> >& ds) {
        // cout << "** put: w = " << w << " h = " << h << endl;
        for (int i = 0; i < ds.size(); ++i) {
            int hh = h - ds[i].first;
            int ww = w - ds[i].second;
            if (-1 <= hh && hh <= 1 && -1 <= ww && ww <= 1) {
                int id = (hh + 1) * 3 + (ww + 1);
                // cout << "put " << i << ": ww = " << ww << " hh = " << hh << " id = " << id << " chest = " << chest << endl;
                seen[i] |= (1 << id);
                if (chest) {
                    --leftChests[i];
                    if (leftChests[i] < 0)
                        return false;
                }
            }
        }

        return true;
    }

    friend bool operator<(const State& lhs, const State& rhs) {
        if (lhs.seen != rhs.seen)
            return lhs.seen < rhs.seen;

        return lhs.leftChests < rhs.leftChests;
    }
};

int iter(int ith,
         const State& s,
         map<State, int>& cache,
         int totalChests,
         const vector<vector<char> >& m, 
         const vector<pair<int, int> >& ds)
{
    if (ith == ds.size())
        return totalChests;

    if (cache.count(s))
        return totalChests + cache[s];

    int fp = s.freeSpaceOf(ith);
    if (fp < s.leftChests[ith])
        return INT_MAX / 2;

    vector<int> chests(fp);
    for (int i = 0; i < s.leftChests[ith]; ++i)
        chests[fp - i - 1] = 1;
    // chests is already sorted.

    int minChests = INT_MAX / 2;
    do {
        State newState(s);
        int p = 0;
        bool ok = true;
        for (int i = 0; i < 9; ++i) {
            if (s.seen[ith] & (1 << i))
                continue;
                        
            if (!newState.putChest(ds[ith].second + dw[i], ds[ith].first + dh[i], chests[p++], ds)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            int x = iter(ith + 1, newState, cache, totalChests + s.leftChests[ith], m, ds);
            minChests = min(x, minChests);
        }
    } while (next_permutation(chests.begin(), chests.end()));

    cache[s] = minChests - totalChests;
    return minChests;
}

int solve(const vector<vector<char> >& m, const vector<pair<int, int> >& ds)
{
    // Create initial states.
    State s(ds.size());
    for (int i = 0; i < ds.size(); ++i) {
        s.leftChests[i] = m[ds[i].first][ds[i].second] - '0';

        for (int w = 0; w < 3; ++w) {
            for (int h = 0; h < 3; ++h) {
                int hh = ds[i].first + h - 1;
                int ww = ds[i].second + w - 1;
                int id = w + h * 3;
                if (0 <= ww && ww < m[0].size() && 0 <= hh && hh < m.size()) {
                    if (!(m[hh][ww] == '*' || ('0' <= m[hh][ww] && m[hh][ww] <= '9')))
                        s.seen[i] |= (1 << id);
                } else {
                    s.seen[i] |= (1 << id);
                }
            }
        }
    }

    map<State, int> cache; // State -> 最小の個数    
    return iter(0, s, cache, 0, m, ds);
}

int main(void)
{
    for (int H, W; cin >> H >> W, H || H; ) {
        vector<vector<char> > m(H, vector<char>(W));
        vector<pair<int, int> > digits;
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j) {
                cin >> m[i][j];
                if ('0' <= m[i][j] && m[i][j] <= '9')
                    digits.push_back(make_pair(i, j));
            }

        cout << solve(m, digits) << endl;
    }
    return 0;
}

