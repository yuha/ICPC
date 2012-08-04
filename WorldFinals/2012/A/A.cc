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

typedef pair<int, int> pi;
typedef pair<pi, pi> Line;
typedef map<double, set<pi> > Candidates;

const double EPS = 1E-9;

struct Asteroid {

    friend Asteroid operator-(const Asteroid& lhs, const Asteroid& rhs) {
        Asteroid a;
        for (int i = 0; i < 3; ++i) {
            a.p[i] = lhs.p[i] - rhs.p[i];
            a.v[i] = lhs.v[i] - rhs.v[i];
        }
        return a;
    }

    double distance(double t) {
        double dd = 0;
        for (int i = 0; i < 3; ++i)
            dd += (p[i] + v[i] * t) * (p[i] + v[i] * t);

        return sqrt(dd);
    }

    double p[3];
    double v[3];
};

inline double dot(double* p, double* q)
{
    return p[0] * q[0] + p[1] * q[1] + p[2] * q[2];
}

void enumerateCandidates(const vector<Asteroid>& va, Candidates& ts)
{
    const int N = va.size();

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            Asteroid aa = va[i] - va[j];
            for (int s = 0; s < N; ++s) {
                for (int t = s + 1; t < N; ++t) {
                    if (make_pair(i, j) >= make_pair(s, t))
                        continue;

                    Asteroid ab = va[s] - va[t];
                    double a = dot(aa.v, aa.v) - dot(ab.v, ab.v);
                    double b = 2 * (dot(aa.v, aa.p) - dot(ab.v, ab.p));
                    double c = dot(aa.p, aa.p) - dot(ab.p, ab.p);

                    // a x^2 + b x + c = 0
                    pair<pair<int, int>, pair<int, int> > pa(make_pair(i, j), make_pair(s, t));

                    double d = b * b - 4 * a * c;
                    if (-EPS < a && a < EPS) {
                        if (b < -EPS || EPS < b) {
                            double tt = -c / b;
                            if (tt > 0) {
                                double tte = tt + 0.5 * 1E-6;
                                if (tte * b + c > 0)
                                    ts[tt].insert(make_pair(i, j));
                                else
                                    ts[tt].insert(make_pair(s, t));
                            }
                        }
                    } else if (-EPS < d && d < EPS) {                   
                        double tt = -b / (2 * a);
                        if (tt > 0) {
                            double tte = tt + 0.5 * 1E-6;
                            if (a * tte * tte + tte * b + c > 0)
                                ts[tt].insert(make_pair(i, j));
                            else
                                ts[tt].insert(make_pair(s, t));
                        }
                    } else if (d >= EPS) {
                        double tt = (-b + sqrt(d)) / (2 * a);
                        if (tt > 0) {
                            double tte = tt + 0.5 * 1E-6;
                            if (a * tte * tte + tte * b + c > 0)
                                ts[tt].insert(make_pair(i, j));
                            else
                                ts[tt].insert(make_pair(s, t));
                        }
                        tt = (-b - sqrt(d)) / (2 * a);
                        if (tt > 0) {
                            double tte = tt + 0.5 * 1E-6;
                            if (a * tte * tte + tte * b + c > 0)
                                ts[tt].insert(make_pair(i, j));
                            else
                                ts[tt].insert(make_pair(s, t));
                        }
                    }
                }
            }
        }
    }
}

typedef int Vertex;
typedef double Weight;

struct SEdge { // Edge with Source
    Vertex src;
    Vertex dest;
    Weight weight;
    SEdge() {}
    SEdge(const Vertex& s, const Vertex& d, Weight w) : src(s), dest(d), weight(w) {}
};

typedef vector<SEdge> SEdges;
typedef vector<SEdges> SGraph;

bool operator>(const SEdge& lhs, const SEdge& rhs) {
  if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
  if (lhs.dest != rhs.dest) { return lhs.dest > rhs.dest; }
  return lhs.src > rhs.src;
}

typedef map<Vertex, Weight> Potential;

Weight prim(const vector<Asteroid>& va, set<pair<int, int> >& mst, double time)
{
    priority_queue<SEdge, SEdges, greater<SEdge> > Q;
    set<Vertex> visited;
    Weight mstLength = 0;

    Q.push(SEdge(0, 0, 0)); // MST: need another ctor

    while (!Q.empty() && mst.size() != va.size() - 1) {
        SEdge top = Q.top();
        Q.pop();
        Vertex newV = top.dest;
        if (visited.count(newV)) { continue; }

        mstLength += top.weight;
        visited.insert(newV);
        if (top.src != top.dest)
            mst.insert(make_pair(min(top.src, top.dest), max(top.src, top.dest)));

        for (int i = 0; i < va.size(); ++i) {
            if (newV == i)
                continue;

            SEdge e(newV, i, (va[i]-va[newV]).distance(time));
            if (!visited.count(e.dest))
                Q.push(e);
        }
    }

    return mstLength;
}

int solve(const vector<Asteroid>& va)
{
    Candidates cand;
    enumerateCandidates(va, cand);

    set<pi> lastMst;
    prim(va, lastMst, 0);

    int count = 1;
    int countPrim = 1;
    for (Candidates::iterator it = cand.begin(); it != cand.end(); ++it) {
        bool found = false;
        for (set<pi>::iterator that = it->second.begin(); that != it->second.end(); ++that) {
            if (lastMst.count(*that)) {
                found = true;
                break;
            }
        }

        if (found) {
            set<pair<int, int> > mst;
            prim(va, mst, it->first + 0.5 * 10E-6);
            ++countPrim;
            if (lastMst != mst) {
                ++count;
                swap(mst, lastMst);
            }
        }
    }

    // cout << countPrim << endl;
    return count;
}

int main(void)
{
    int caseNo = 0;
    for (int N; cin >> N; ) {
        vector<Asteroid> va(N);
        for (int i = 0; i < N; ++i) {
            cin >> va[i].p[0] >> va[i].p[1] >> va[i].p[2]
                >> va[i].v[0] >> va[i].v[1] >> va[i].v[2];
        }

        cout << "Case " << ++caseNo << ": " << solve(va) << endl;
    }
    
    return 0;
}

