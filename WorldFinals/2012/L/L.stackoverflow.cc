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

enum Operation {
    TAKEOVER,
    MERGE,
    PASS
};

// If A always wins, return true.
bool iter(vector<long long>& as, vector<long long>& bs, Operation lastOp, bool didMerge)
{    
    // Try takeover
    if (lastOp != TAKEOVER && !as.empty() && !bs.empty() && as.back() > bs.back()) {
        if (lastOp == MERGE || didMerge)
            return true;

        long long b = bs.back(); bs.pop_back();
        bool losesAlways = iter(bs, as, TAKEOVER, didMerge);
        bs.push_back(b);

        if (!losesAlways)
            return true;
    }

    // Try merge
    if (as.size() >= 2) {
        long long a1 = as.back(); as.pop_back();
        long long a2 = as.back(); as.pop_back();
        as.push_back(a1 + a2);

        bool losesAlways = iter(bs, as, MERGE, true);
        as.pop_back();
        as.push_back(a2);
        as.push_back(a1);

        if (!losesAlways)
            return true;
    }

    // Cannot do anything.    
    if (bs.size() > 0 && as.back() < bs.back())
        return false;
    return true;
}

string solve(vector<long long>& as, vector<long long>& bs)
{
    // Try takeover first.
    if (!as.empty() && !bs.empty() && as.back() > bs.back()) {
        long long b = bs.back(); bs.pop_back();
        if (!iter(bs, as, TAKEOVER, false))
            return "Takeover Incorporated";
        bs.push_back(b);
    }

    // Then, merge.
    if (as.size() >= 2) {
        long long a1 = as.back(); as.pop_back();
        long long a2 = as.back(); as.pop_back();
        as.push_back(a1 + a2);
        if (!iter(bs, as, MERGE, true))
            return "Takeover Incorporated";

        as.pop_back();
        as.push_back(a2);
        as.push_back(a1);
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

