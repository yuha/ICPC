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

const double INF = numeric_limits<double>::infinity();
const double EPS = 1E-9;

// 線分 (p1, p2) で p を対象に移動する
inline P mirror(const P& p, const P& p1, const P& p2)
{
    if (abs(p - p1) < EPS || abs(p - p2) < EPS)
        return p;

    double theta = arg((p2 - p1) / (p - p1));
    return p1 + (p - p1) * P(cos(2 * theta), sin(2 * theta));
}

inline double dot(const P& p, const P& q) {
    return std::real(std::conj(p) * q);
}

inline double cross(const P& p, const P& q) {
    return std::imag(std::conj(p) * q);
}

bool isCrossed(P As, P Ae, P Bs, P Be) { 
    P A = Ae - As, B = Be - Bs, S = Bs - As;
    double f = cross(A, B);
    if (abs(f) < EPS) {
        // この if の中は A と B が平行のとき。
        Bs = (Bs - As) / A;
        Be = (Be - As) / A;
        // 同一線上ではないので交わっていない。
        if (abs(imag(Bs)) > EPS) { return false; } 
        // 同一線上
        if (real(Bs) < -EPS && real(Be) < -EPS) { return false; }
        if (1 + EPS < real(Bs) && 1 + EPS < real(Be)) { return false; }
        // この場合 crossed はうまく定義されないので注意する。
        return true;
    } // parallel
    double p = cross(S, B) / f;
    double q = cross(S, A) / f;
    
    // 線分ではなく直線を扱うときは、次の if を取り除く。
    if (p < -EPS || 1 + EPS < p) { return false; } // A が線分のとき、B と交わってない
    if (q < -EPS || 1 + EPS < q) { return false; } // B が線分のとき、A と交わってない
    
    return true;
}

double pointLineDistance(P p, P p1, P p2)
{
    p = p - p1; p2 = p2 - p1;
    P pe = p2 / abs(p2);
    p /= pe; p2 /= pe;

    if (p2.real() < 0) {
        p2 = P(-p2.real(), p2.imag());
        p = P(-p.real(), p.imag());
    }

    if (p.real() < 0)
        return abs(p);
    else if (0 <= p.real() && p.real() <= p2.real())
        return abs(p.imag());
    else
        return abs(p - p2);
}

double calcDist(int start, int goal, const vector<P>& ps)
{
    assert(goal - start >= 2);

    // goal を壁を使って
    vector<P> qs;
    qs.reserve(goal - start + 1);
    for (int i = start; i <= goal; ++i)
        qs.push_back(ps[i]);

    for (int i = 1; i + 1 < qs.size(); ++i) {
        for (int k = i + 2; k < qs.size(); ++k)
            qs[k] = mirror(qs[k], qs[i], qs[i + 1]);
    }


    double r = abs(qs.back() - qs.front());
//    cout << "calcDist " << start << ' ' << goal << ':' << r << endl;
    // 線分 (ps.front(), ps.back()) と (ps[i], ps[i+1]) が全部重なっている必要がある
    for (int i = 1; i + 2 < qs.size(); ++i) {
        if (!isCrossed(qs.front(), qs.back(), qs[i], qs[i+1]))
            return INF;
    }

    return r;
}

double f(int start, int goal, const vector<P>& ps,  vector<vector<double> >& dist)
{
    if (dist[start][goal] >= 0)
        return dist[start][goal];

    if (goal - start <= 2)
        return dist[start][goal] = abs(ps[goal] - ps[start]);

    double result = calcDist(start, goal, ps);
    for (int k = start + 1; k < goal; ++k)
        result = min(result, f(start, k, ps, dist) + f(k, goal, ps, dist));
    return dist[start][goal] = result;
}

double solve(const vector<P>& pp, double px, double py)
{
    int idx = -1;
    double d = INF;
    for (int i = 0; i < pp.size(); ++i) {
        double dd = pointLineDistance(P(px, py), pp[i], pp[i+1]);
        if (dd < d) {
            idx = i;
            d = dd;
        }
    }

    double result = INF;

    for (int k = -1; k <= 1; ++k) {
        vector<P> ps;
        ps.push_back(P(px, py));
        for (int i = 0; i < pp.size(); ++i)
            ps.push_back(pp[(i + idx + k + pp.size()) % pp.size()]);
        ps.push_back(pp[(k + idx + pp.size()) % pp.size()]);
        ps.push_back(P(px, py));
        
        vector<vector<double> > dist(ps.size(), vector<double>(ps.size(), -1));
        double r = f(0, ps.size() - 1, ps, dist);

        result = min(result, r);
    }
    return result;
}

int main(void)
{
    int caseNo = 0;
    for (int N, px, py; cin >> N >> px >> py; ) {
        vector<P> ps(N);
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y;
            ps[i] = P(x, y);
        }

        double r = solve(ps, px, py);
        printf("Case %d: %.2f\n", ++caseNo, r);
    }
    
    return 0;
}

