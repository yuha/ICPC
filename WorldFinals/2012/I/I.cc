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
typedef complex<int> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

enum Dir {
    N, E, S, W
};

namespace std {
    bool operator<(const P& lhs, const P& rhs) {
        if (lhs.real() != rhs.real())
            return lhs.real() < rhs.real();
        return lhs.imag() < rhs.imag();
    }
}

struct Mirror {
    Mirror() {}
    Mirror(int x, int y, bool slash) : x(x), y(y), slash(slash) {}
    
    int x, y;
    bool slash; // '\' if false.
};

struct SortByRow {
    bool operator()(const Mirror& m1, const Mirror& m2) const {
        if (m1.x != m2.x)
            return m1.x < m2.x;
        return m1.y < m2.y;
    }
};

struct SortByColumn {
    bool operator()(const Mirror& m1, const Mirror& m2) const {
        if (m1.y != m2.y)
            return m1.y < m2.y;
        return m1.x < m2.x;
    }
};

struct MirrorSet {
    bool find(int x, int y, Dir d, Mirror& m) const {
        switch (d) {
        case N: {
            set<Mirror, SortByColumn>::const_iterator it = xs.lower_bound(Mirror(x, y, false));
            if (it == xs.begin())
                return false;
            --it;
            if (it->x != x)
                return false;
            m = *it;
            return true;
        }
        case E: {
            set<Mirror, SortByRow>::const_iterator it = ys.lower_bound(Mirror(x + 1, y, false));
            if (it == ys.end() || it->y != y)
                return false;
            m = *it;
            return true;
        }
        case S: {
            set<Mirror, SortByColumn>::const_iterator it = xs.lower_bound(Mirror(x, y + 1, false));
            if (it == xs.end() || it->x != x)
                return false;
            m = *it;
            return true;            
            }
        case W: {
            set<Mirror, SortByRow>::const_iterator it = ys.lower_bound(Mirror(x, y, false));
            if (it == ys.begin())
                return false;
            --it;
            if (it->y != y)
                return false;
            m = *it;
            return true;
        }
        default:
            throw "SHOULD NOT HAPPEN : MirrorSet#find";
        }
    }

    void insert(const Mirror& mirror) {
        xs.insert(mirror);
        ys.insert(mirror);
    }

    set<Mirror, SortByRow> xs;
    set<Mirror, SortByColumn> ys;
};

struct BIT {
    BIT(int n) : bit(n) {}

    int bitquery(int from, int to) { // [from, to)
        if (from > 0)
            return bitquery(0, to) - bitquery(0, from);

        int res = 0;
        for (int k = to - 1; k >= 0; k = (k & (k+1)) - 1)
            res += bit[k];
        return res;
    }
 
    void bitupdate(int pos, const int& delta) {
        for (int n = bit.size(); pos < n; pos |= pos+1)
            bit[pos] += delta;
    }
    
    vi bit;
};

Mirror trace(int x, int y, Dir d,
             const MirrorSet& ms, int maxX, int maxY,
             set<pair<P, P> >& hs, set<pair<P, P> >& vs) {
    Mirror m;
    while (true) {
        bool foundMirror = ms.find(x, y, d, m);

        if (!foundMirror) {
            switch (d) {
            case N:
                m = Mirror(x, 0, false);
                break;
            case E:
                m = Mirror(maxX + 1, y, false);
                break;
            case S:
                m = Mirror(x, maxY + 1, false);
                break;
            case W:
                m = Mirror(0, y, false);
                break;
            }
        }

        switch (d) {
        case N:
            vs.insert(make_pair(P(m.x, m.y + 1), P(x, y - 1)));
            d = m.slash ? E : W;
            break;
        case E:
            hs.insert(make_pair(P(x + 1, y), P(m.x - 1, m.y)));
            d = m.slash ? N : S;
            break;
        case S:
            vs.insert(make_pair(P(x, y + 1), P(m.x, m.y - 1)));
            d = m.slash ? W : E;
            break;
        case W:
            hs.insert(make_pair(P(m.x + 1, m.y), P(x - 1, y)));
            d = m.slash ? S : N;
            break;
        }

        x = m.x;
        y = m.y;

        if (!foundMirror)
            return m;
    }
}

long long countCrosses(const set<pair<P, P> >& hs, const set<pair<P, P> >& vs, int maxX, int& resultX, int& resultY)
{
    // Y でソート。int は priority
    typedef multiset<pair<vi, pair<P, P> > > S;
    S s;
    for (set<pair<P, P> >::const_iterator it = vs.begin(); it != vs.end(); ++it) {
        if (it->second.imag() < it->first.imag())
            continue;
        vi v1(3); v1[0] = it->first.imag(); v1[1] = 0; v1[2] = it->first.real();
        s.insert(make_pair(v1, *it));

        vi v2(3); v2[0] = it->second.imag(); v2[1] = 2; v2[2] = it->second.real();
        s.insert(make_pair(v2, *it));
    }
    for (set<pair<P, P> >::const_iterator it = hs.begin(); it != hs.end(); ++it) {        
        if (it->second.real() < it->first.real())
            continue;
        vi v(3); v[0] = it->first.imag(); v[1] = 1; v[2] = it->first.real();
        s.insert(make_pair(v, *it));
    }

    long long resultSize = 0;
    BIT t(maxX + 2);
    for (S::iterator it = s.begin(); it != s.end(); ++it) {
        if (it->first[1] == 0) // 上端点？
            t.bitupdate(it->second.first.real(), 1);
        else if (it->first[1] == 2) // 下端点？
            t.bitupdate(it->second.first.real(), -1);
        else {
            int s = t.bitquery(it->second.first.real(), it->second.second.real() + 1);
            resultSize += s;
            if (s > 0 && resultX == INT_MAX) {
                for (int x = it->second.first.real(); x <= it->second.second.real(); ++x) {
                    if (t.bitquery(x, x + 1)) {
                        resultX = x;
                        resultY = it->second.first.imag();
                        break;
                    }
                }
            }
        }
    }

    return resultSize;
}

long long solve(const MirrorSet& ms, int& resultX, int& resultY, int maxX, int maxY)
{
    set<pair<P, P> > hs1, vs1, hs2, vs2;

    Mirror m = trace(0, 1, E, ms, maxX, maxY, hs1, vs1);
    if (m.x == maxX + 1 && m.y == maxY)
        return -1;
    
    trace(maxX + 1, maxY, W, ms, maxX, maxY, hs2, vs2);


    long long resultSize = 0;
    int resultX1 = INT_MAX, resultX2 = INT_MAX, resultY1 = INT_MAX, resultY2 = INT_MAX;
    resultSize += countCrosses(hs1, vs2, maxX, resultX1, resultY1);
    resultSize += countCrosses(hs2, vs1, maxX, resultX2, resultY2);

    if (make_pair(resultY1, resultX1) < make_pair(resultY2, resultX2)) {
        resultX = resultX1;
        resultY = resultY1;
    } else {
        resultX = resultX2;
        resultY = resultY2;
    }

    return resultSize;
}

int main(void)
{
    int caseNo = 0;
    for (int R, C, M, N; cin >> R >> C >> M >> N;) {
        MirrorSet ms;
        for (int i = 0; i < M; ++i) {
            int r, c; cin >> r >> c;
            ms.insert(Mirror(c, r, true));
        }
        for (int i = 0; i < N; ++i) {
            int r, c; cin >> r >> c;
            ms.insert(Mirror(c, r, false));
        }

        cout << "Case " << ++caseNo << ": ";

        int resultX, resultY;
        long long resultSize = solve(ms, resultX, resultY, C, R);
        if (resultSize < 0)
            cout << 0 << endl;
        else if (resultSize == 0)
            cout << "impossible" << endl;
        else
            cout << resultSize << ' ' << resultY << ' ' << resultX << endl;
    }
    
    return 0;
}

