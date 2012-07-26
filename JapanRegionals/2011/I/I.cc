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

const double EPS = 1E-10;

struct Circle {
    Circle() {}
    Circle(const P& p, double r) : p(p), r(r) { }
    Circle(double x, double y, double r) : p(x, y), r(r) { }

    P p;
    double r;
};

struct ExtendedCirclePoint {
    ExtendedCirclePoint(const P& c, const P& p, int n) : c(c), p(p), n(n) {}

    P c; //center
    P p;
    int n;
};

struct comparator {
    comparator(const P& center) : center(center) {}
    bool operator()(const ExtendedCirclePoint& lhs, const ExtendedCirclePoint& rhs) {
        return arg(lhs.p - center) < arg(rhs.p - center);
    }

    P center;
};

bool crosses(const Circle& c1, const Circle& c2) {
    if (abs(c1.p - c2.p) >= c1.r + c2.r + EPS)
        return false;

    return abs(c1.p - c2.p) + EPS > max(c1.r, c2.r) - min(c1.r, c2.r);
}

pair<P, P> crossedAt(const Circle& c1, const Circle& c2) {
    double d = abs(c1.p - c2.p);
    double rc = (d * d + c1.r * c1.r - c2.r * c2.r) / (2 * d);
    double rs = sqrt(c1.r * c1.r - rc * rc);
    P diff = (c2.p - c1.p) / d;
    return make_pair(c1.p + diff * P(rc, rs), c1.p + diff * P(rc, -rs));
}

bool containsAll(const Circle& c, const vector<Circle>& cs)
{
    for (int i = 0; i < cs.size(); ++i) {
        if (c.r + EPS < abs(cs[i].p - c.p) + cs[i].r)
            return false;
    }

    return true;
}

double solve(const vector<Circle>& cs, int r)
{
    const int n = cs.size();

    vector<ExtendedCirclePoint> ecs;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            double ri = r - cs[i].r, rj = r - cs[j].r;

            Circle ci(cs[i].p, ri), cj(cs[j].p, rj);

            if (!crosses(ci, cj))
                continue;

            pair<P, P> ps = crossedAt(ci, cj);
            if (containsAll(Circle(ps.first, r), cs)) {
                P pi = (ps.first - cs[i].p); pi *= r / abs(pi); pi += ps.first;
                P pj = (ps.first - cs[j].p); pj *= r / abs(pj); pj += ps.first;
                ecs.push_back(ExtendedCirclePoint(ps.first, pi, i));
                ecs.push_back(ExtendedCirclePoint(ps.first, pj, j));
            }

            if (containsAll(Circle(ps.second, r), cs)) {
                P pi = (ps.second - cs[i].p); pi *= r / abs(pi); pi += ps.second;
                P pj = (ps.second - cs[j].p); pj *= r / abs(pj); pj += ps.second;
                ecs.push_back(ExtendedCirclePoint(ps.second, pi, i));
                ecs.push_back(ExtendedCirclePoint(ps.second, pj, j));
            }
        }
    }

    if (ecs.empty()) {
        for (int i = 0; i < n; ++i) {
            if (containsAll(cs[i], cs))
                return 2 * M_PI * (2 * r - cs[i].r);
        }

        return 0.0;
    }

    double result = 0;
    sort(ecs.begin(), ecs.end(), comparator(ecs[0].c));

    for (int i = 0; i < ecs.size(); ++i) {
        int j = (i + 1) % ecs.size();

        if (ecs[i].n != ecs[j].n) {
            result += r * abs(arg((ecs[i].p - ecs[i].c) / (ecs[j].p - ecs[j].c)));
        } else {
            double theta = abs(arg((ecs[i].p - cs[ecs[i].n].p) / (ecs[j].p - cs[ecs[i].n].p)));
            result += abs(r + r - cs[ecs[i].n].r) * theta;
        }
    }
    
    return result;
}

int main(void)
{
    for (int n, r; cin >> n >> r, (n || r); ) {
        vector<Circle> cs(n);
        for (int i = 0; i < n; ++i) {
            int x, y, c;
            cin >> x >> y >> c;
            cs[i] = Circle(x, y, c);
        }

        double result = solve(cs, r);
        printf("%.10f\n", result);
    }
    
    return 0;
}

