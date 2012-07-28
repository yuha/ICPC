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
#include <bitset>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;


struct Bits {
    static void init() {
        for (int i = 0; i <= 75; ++i) {
            bitset<75> v;
            for (int j = 0; j < i; ++j)
                v.set(j);
            values.push_back(v);
        }
    }

    Bits() {
    }

    void set(int n, bool b) {
        v.set(n, b);
    }

    bool allTrue(int n) const {
        return v == values[n];
    }

    Bits& operator|=(const Bits& bs) {
        v |= bs.v;
        return *this;
    }

    bitset<75> v;
private:
    static vector<bitset<75> > values;
};

vector<bitset<75> > Bits::values;

bool iter(int ith, int n, int from, const Bits& vb, const vector<Bits>& vs, vi& used)
{
    if (ith == 0) {
        return vb.allTrue(n);
    }

    for (int i = from; i < n; ++i) {
        if (used[i])
            continue;
        
        used[i] = 1;
        Bits vc(vb);
        vc |= vs[i];
        if (iter(ith - 1, n, i + 1, vc, vs, used))
            return true;
        used[i] = 0;
    }

    return false;
}

vi solve(const vector<Bits>& vs, int n)
{
    int minAnswer = INT_MAX;

    vector<int> used(n);
    for (int i = 1; i <= n; ++i) {
        Bits vb;
        if (iter(i, n, 0, vb, vs, used)) {
            vi result;
            for (int j = 0; j < n; ++j) {
                if (used[j])
                    result.push_back(j + 1);
            }
            return result;
        }
    }

    throw "ASSERT_NOT_REACHED: No answer found";
}

int main(void)
{
    Bits::init();
    
    int caseNo = 0;
    for (int n; cin >> n; ) {
        vector<string> ss(n);
        for (int i = 0; i < n; ++i)
            cin >> ss[i];

        vector<Bits> vs(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
                vs[i].set(j, (i == j) ? 1 : ss[i][j] == '1');
        }

        vector<int> answer = solve(vs, n);
        cout << "Case " << ++caseNo << ": " << answer.size();
        for (int i = 0; i < answer.size(); ++i)
            cout << ' ' << answer[i];
        cout << endl;
    }
    return 0;
}

