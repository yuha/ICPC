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

long long solve(int n, const string& s, 
                vector<long long>& dp,
                const vector<string>& firsts, const vector<string>& lasts)
{
    if (dp[n] >= 0)
        return dp[n];

    if (n == 0)
        return dp[0] = (s == "0");
    if (n == 1)
        return dp[1] = (s == "1");

    long long result = solve(n-1, s, dp, firsts, lasts) + solve(n-2, s, dp, firsts, lasts);
    if (s.size() <= 1)
        return dp[n] = result;

    string concat = lasts[n-1] + firsts[n-2];
    
    int start = (int)lasts[n-1].size() - (int)s.size() + 1;
    if (start < 0)
        start = 0;

    while ((start = concat.find(s, start)) != string::npos && start < lasts[n-1].size())
        ++result, ++start;

    return dp[n] = result;
}

void buildTable(int n, vector<string>& firsts, vector<string>& lasts)
{
    vector<string> ss(n + 1);

    ss[0] = firsts[0] = lasts[0] = "0";
    ss[1] = firsts[1] = lasts[1] = "1";

    int i;
    for (i = 2; i <= n; ++i) {
        if (ss[i-2].length() > 100000) {
            string s = ss[i-1] + ss[i-2];
            firsts[i] = s.substr(0, 100000);
            lasts[i] = s.substr(s.length() - 100000, 100000);
            ++i;
            break;
        }

        string s = ss[i-1] + ss[i-2];
        ss[i] = s; 
        firsts[i] = s.substr(0, min((int)s.length(), 100000));
        lasts[i] = s.substr(max(0, static_cast<int>(s.length() - 100000)), min((int)s.length(), 100000));
    }

    for (; i <= n; ++i) {
        firsts[i] = firsts[i-1];
        lasts[i] = lasts[i-2];
    }    
}

int main(void)
{
    vector<string> firsts(101);
    vector<string> lasts(101);

    buildTable(100, firsts, lasts);

    int caseNo = 0;
    for (int n; cin >> n; ) {
        string s; cin >> s;
        vector<long long> dp(n + 1, -1);
        cout << "Case " << ++caseNo << ": " << solve(n, s, dp, firsts, lasts) << endl;
    }
    return 0;
}

