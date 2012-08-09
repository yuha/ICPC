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

int f(int s, int last, int n, int g, vvi& dist, const vvi& d)
{
    if (dist[s][last] < INT_MAX / 2)
        return dist[s][last];

    if (s == 0)
        return d[g][last];

    int result = INT_MAX;
    for (int i = 1; i < n - 1; ++i) {
        if (!(s & (1 << i)))
            continue;

        int s2 = s & ~(1 << i);
        int r = f(s2, i, n, g, dist, d) + d[i][last];
        if (r < result)
            result = r;
    }

    return dist[s][last] = result;
}

int solve(const vvi& d, int n)
{
    int h = n - 2;
    if (h == 1) {
        return d[0][1] + d[1][2] + d[2][1] + d[1][0];
    }

    vvi fromS(1 << n, vi(n, INT_MAX / 2));
    vvi fromG(1 << n, vi(n, INT_MAX / 2));

    vi uses(n);
    for (int i = 0; i < h/2; ++i)
        uses[i] = 1;
    sort(uses.begin(), uses.end());

    int result = INT_MAX;
    do {
        if (uses[0] || uses[n-1])
            continue;
        
        int s = 0, t = 0;
        for (int i = 1; i < n - 1; ++i) {
            s |= uses[i] << i;
            t |= (1-uses[i]) << i;
        }

        for (int i = 1; i < n - 1; ++i) {
            if (!uses[i])
                continue;
            
            for (int j = 1; j < n - 1; ++j) {
                if (!uses[j])
                    continue;
                
                int dist = 
                    f(s & ~(1 << i), i, n, 0, fromS, d) +
                    f(t & ~(1 << i), i, n, n-1, fromG, d) +
                    f(s & ~(1 << j), j, n, n-1, fromG, d) +
                    f(t & ~(1 << j), j, n, 0, fromS, d);
                
                result = min(dist, result);
            }
        }
    } while(next_permutation(uses.begin(), uses.end()));

    return result;
}

int main(void)
{
    int caseNo = 0;
    for (int n, m; cin >> n >> m; ) {
        vvi d(n, vi(n, INT_MAX / 2));

        for (int i = 0; i < n; ++i)
            d[i][i] = 0;

        for (int i = 0; i < m; ++i) {
            int s, t, u; cin >> s >> t >> u;
            d[s][t] = u;
            d[t][s] = u;
        }

        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        
        cout << "Case " << ++caseNo << ": " << solve(d, n) << endl;
    }
    int n;
    return 0;
}

