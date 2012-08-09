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
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

const double EPS = 1E-10;
const double RADIUS = 6370.0;

inline double fit(double x)
{
    if (-EPS < x && x < EPS)
        return 0;
    return x;
}

struct P {
    P() : x(0), y(0), z(0) {}
    P(double x, double y, double z) : x(x), y(y), z(z) {}

    double theta() const {
        double sinTheta = z / sqrt(x * x + y * y + z * z);
        return asin(sinTheta);
    }

    double phi() const {
        if (x * x + y * y < EPS)
            return 0;

        return atan2(y, x);
    }

    double psi() const {
        if (y * y + z * z < EPS)
            return 0;

        return atan2(z, y);
    }

    P& rotateX(double theta) {
        double xx = x;
        double yy = y * cos(theta) - z * sin(theta);
        double zz = y * sin(theta) + z * cos(theta);
        
        x = fit(xx); y = fit(yy); z = fit(zz);
        return *this;
    }

    P& rotateY(double theta) {
        double xx = z * sin(theta) + x * cos(theta);
        double yy = y;
        double zz = z * cos(theta) - x * sin(theta);
        
        x = fit(xx); y = fit(yy); z = fit(zz);
        return *this;
    }

    P& rotateZ(double theta) {
        double xx = x * cos(theta) - y * sin(theta);
        double yy = x * sin(theta) + y * cos(theta);
        double zz = z;

        x = fit(xx); y = fit(yy) ; z = fit(zz);
        return *this;
    }

    friend ostream& operator<<(ostream& os, const P& p) {
        return (os << '(' << p.x << ',' << p.y << ',' << p.z << ')');
    }

    double x, y, z;
};

vector<P> crossCircle(const P& p, double rp, const P& q, double rq)
{
    // p を (x, 0, 0) まで移し、q を xy 平面まで移す様に回転する。
    P pp(p), qq(q);

    double phi = pp.phi();
    pp.rotateZ(-phi); qq.rotateZ(-phi);

    double theta = pp.theta();
    pp.rotateY(theta); qq.rotateY(theta);

    double psi = qq.psi();
    pp.rotateX(-psi); qq.rotateX(-psi);

    assert(pp.y < EPS);
    assert(pp.z < EPS);
    assert(qq.z < EPS);

    // 
    double x = RADIUS * cos(rp / RADIUS);

    // q 上
    double xq1 = qq.x * cos(rq / RADIUS) - qq.y * sin(rq / RADIUS);
    double yq1 = qq.x * sin(rq / RADIUS) + qq.y * cos(rq / RADIUS);

    double xq2 = qq.x * cos(-rq / RADIUS) - qq.y * sin(-rq / RADIUS);
    double yq2 = qq.x * sin(-rq / RADIUS) + qq.y * cos(-rq / RADIUS);

    if (abs(xq1 - xq2) < EPS)
        return vector<P>();

    double y = (yq2 - yq1) * (x - xq1) / (xq2 - xq1) + yq1;
    if (y < min(yq1, yq2) || max(yq1, yq2) < y)
        return vector<P>();

    if (RADIUS * RADIUS - x * x - y * y < EPS)
        return vector<P>();

    double z = sqrt(RADIUS * RADIUS - x * x - y * y);
    
    // 元に戻す
    P r1(x, y, z), r2(x, y, -z);

    r1.rotateX(psi).rotateY(-theta).rotateZ(phi);
    r2.rotateX(psi).rotateY(-theta).rotateZ(phi);

    vector<P> r;
    r.push_back(r1);
    r.push_back(r2);
    
    return r;
}

bool covers(double low, double high, const vector<pair<double, double> >& pis)
{
    for (int i = 0; i < pis.size(); ++i) {
        pair<double, double> p = pis[i];
        if (p.second < low)
            continue;
        else if (p.first - EPS <= low && p.second <= high + EPS)
            low = p.second;
        else if (low < p.first && p.second <= high)
            return false;
        else if (low < p.first && high < p.second)
            continue;
        else if (high < p.first)
            continue;
        else if (p.first - EPS <= low && high <= p.second + EPS)
            return true;
        else
            assert(false);
    }

    return high - low < EPS;
}

double calcDistance(const P& p, const P& q, const vector<P>& ps, double R)
{
    // p を (RADIUS, 0, 0) へ、q を (x, y, 0) へもっていくように動かす。
    P pp(p), qq(q);

    double phi = pp.phi();
    pp.rotateZ(-phi); qq.rotateZ(-phi);

    double theta = pp.theta();
    pp.rotateY(theta); qq.rotateY(theta);

    double psi = qq.psi();
    pp.rotateX(-psi); qq.rotateX(-psi);

    assert(pp.y < EPS);
    assert(pp.z < EPS);
    assert(qq.z < EPS);

    vector<P> ts(ps);
    for (int i = 0; i < ts.size(); ++i)
        ts[i].rotateZ(-phi).rotateY(theta).rotateX(-psi);

    // p->q の円弧をカバーできているかどうかをチェックする。
    // (0, 0, RADIUS)
    vector<pair<double, double> > pis;
    for (int i = 0; i < ts.size(); ++i) {
        vector<P> rs = crossCircle(P(0, 0, RADIUS), M_PI * RADIUS / 2, ts[i], R);
        if (rs.size() == 2) {
            double theta0 = atan2(rs[0].y, rs[0].x);
            double theta1 = atan2(rs[1].y, rs[1].x);
            if (theta1 < theta0)
                swap(theta0, theta1);
            if (theta1 - theta0 < M_PI)
                pis.push_back(make_pair(theta0, theta1));
            else {
                pis.push_back(make_pair(theta1, M_PI));
                pis.push_back(make_pair(-M_PI, theta0));
            }
        }
    }

    // カバーできていれば、円周を返す
    sort(pis.begin(), pis.end());
    double qtheta = atan2(qq.y, qq.x);
    if (covers(min(qtheta, 0.0), max(qtheta, 0.0), pis))
        return RADIUS * abs(qtheta);
    else
        return 1E+10;
}

void buildNetwork(const vector<P>& ps, double R, vector<vector<double> >& ds)
{
    vector<P> cs(ps);

    for (int i = 0; i < ps.size(); ++i) {
        for (int j = i + 1; j < ps.size(); ++j) {
            vector<P> s = crossCircle(ps[i], R, ps[j], R);
            for (int k = 0; k < s.size(); ++k)
                cs.push_back(s[k]);
        }
    }

    // 通行可能かどうか判定しながら、点 A -> B の距離を求める。
    vector<vector<double> > dist(cs.size(), vector<double>(cs.size(), 1E+10));
    for (int i = 0; i < cs.size(); ++i) {
        dist[i][i] = 0;
        for (int j = i + 1; j < cs.size(); ++j) {
            double d = calcDistance(cs[i], cs[j], ps, R);
            dist[i][j] = d;
            dist[j][i] = d;
        }
    }

    // これを Floyd にかけると、実際の飛行経路に沿った道のりになる。
    for (int k = 0; k < cs.size(); ++k) {
        for (int i = 0; i < cs.size(); ++i) {
            for (int j = 0; j < cs.size(); ++j) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    for (int i = 0; i < ps.size(); ++i)
        for (int j = 0; j < ps.size(); ++j)
            ds[i][j] = dist[i][j];
}

double solve(vector<vector<double> > ds, int s, int t, int c)
{
    for (int i = 0; i < ds.size(); ++i) {
        for (int j = 0; j < ds.size(); ++j) {
            if (ds[i][j] > c)
                ds[i][j] = 1E+10;
        }
    }

    // めんどくさいので Floyd で。本当は dijkstra の方が速いがすでに Floyd 使いまくってる
    // からまあ大丈夫かな、と。
    for (int k = 0; k < ds.size(); ++k) {
        for (int i = 0; i < ds.size(); ++i) {
            for (int j = 0; j < ds.size(); ++j) {
                ds[i][j] = min(ds[i][j], ds[i][k] + ds[k][j]);
            }
        }
    }

    if (ds[s][t] >= 1E+10)
        return -1;
    else
        return ds[s][t];
}

int main(void)
{
    int caseNo = 0;
    for (int N, R; cin >> N >> R; ) {
        vector<P> ps(N);
        for (int i = 0; i < N; ++i) {
            double phi, theta;
            cin >> phi >> theta;
            theta = theta * M_PI / 180;
            phi = phi * M_PI / 180;
            ps[i].x = RADIUS * cos(theta) * cos(phi);
            ps[i].y = RADIUS * cos(theta) * sin(phi);
            ps[i].z = RADIUS * sin(theta);
        }

        vector<vector<double> > ds(N, vector<double>(N));
        buildNetwork(ps, R, ds);

        cout << "Case " << ++caseNo << ":" << endl;

        int Q; cin >> Q;
        for (int i = 0; i < Q; ++i) {
            int s, t, c; cin >> s >> t >> c;
            double result = solve(ds, s-1, t-1, c);
            if (result >= 0)
                printf("%.3f\n", result);
            else
                cout << "impossible" << endl;
        }
    }
    return 0;
}

