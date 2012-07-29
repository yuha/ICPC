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

// If A wins, return true;
bool simulate(vector<long long>& as, vector<long long>& bs)
{    
    bool turnA = true;

    while (as.size() >= 1 && bs.size() >= 1) {
        // If takeover is possible, win.
        if (!as.empty() && !bs.empty() && as.back() > bs.back())
            return turnA;

        // merge if possible
        if (as.size() >= 2) {
            long long a1 = as.back(); as.pop_back();
            long long a2 = as.back(); as.pop_back();
            as.push_back(a1 + a2);
        }

        swap(as, bs);
        turnA = !turnA;
    }

    if (as.size() > 0)
        return turnA;
    else
        return !turnA;
}

string solve(const vector<long long>& origAs, const vector<long long>& origBs)
{
    // Try takeover first.
    if (!origAs.empty() && !origBs.empty() && origAs.back() > origBs.back()) {
        vector<long long> as(origAs);
        vector<long long> bs(origBs);

        // takeover
        long long b = bs.back(); bs.pop_back();

        // When A did takeover, B should do merge. If impossible B will lose.
        if (bs.size() >= 2) {
            long long b1 = bs.back(); bs.pop_back();
            long long b2 = bs.back(); bs.pop_back();
            bs.push_back(b1 + b2);

            if (simulate(as, bs))
                return "Takeover Incorporated";
        } else
            return "Takeover Incorporated";
    }

    // Takeover doesn't work, do merge.
    if (origAs.size() >= 2) {
        vector<long long> as(origAs);
        vector<long long> bs(origBs);

        long long a1 = as.back(); as.pop_back();
        long long a2 = as.back(); as.pop_back();
        as.push_back(a1 + a2);

        if (!simulate(bs, as))
            return "Takeover Incorporated";
    }

    return "Buyout Limited";
}

int main(void)
{
    int caseNo = 0;
    for (int N, M; cin >> N >> M; ) {
        vector<long long> as(N), bs(M);
        for (int i = 0; i < N; ++i)
            cin >> as[i];
        for (int j = 0; j < M; ++j)
            cin >> bs[j];

        sort(as.begin(), as.end());
        sort(bs.begin(), bs.end());

        cout << "Case " << ++caseNo << ": " << solve(as, bs) << endl;
    }
    
    return 0;
}

