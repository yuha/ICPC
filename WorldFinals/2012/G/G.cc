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

struct P {
    P() {}
    P(int x, int y, int z, int k) : x(x), y(y), z(z), k(k) {}

    int x, y, z, k;
};

double calcDistance(const P& lhs, const P& rhs)
{
    double x = lhs.x - rhs.x;
    double y = lhs.y - rhs.y;
    double z = lhs.z - rhs.z;
    return sqrt(x*x + y*y + z*z);
}

struct SubVertex {
    SubVertex(int id) : id(id), k(0) {}
    
    int id;
    int k;
    vi vs;
};

void gather(int ith, const vector<P>& ps, const map<int, vi>& edges, int level, vi& used, SubVertex& sv)
{
    if (used[ith])
        return;

    if (ps[ith].z > level) {
        used[ith] = true;
        return;
    }

    sv.vs.push_back(ith);
    used[ith] = true;

    if (edges.count(ith)) {
        const vi& es = edges.find(ith)->second;
        for (int i = 0; i < es.size(); ++i)
            gather(es[i], ps, edges, level, used, sv);
    }
}

typedef int Vertex;
typedef double Weight;

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

// for MST / Dijkstra
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

typedef map<Vertex, Weight> Potential;

Potential dijkstra(Graph& g, const Vertex& startV) {
    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;

    Q.push(Edge(startV, 0));

    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex p = edge.dest;
        Weight curr = edge.weight;

        if (pot.count(p)) { continue; } // already visited.
        pot[p] = curr;

        Edges& es = g[p];
        for (int i = 0; i < es.size(); ++i) {
            Edge& e = es[i];
            if (pot.count(e.dest)) { continue; } // already visited.
            Q.push(Edge(e.dest, curr + e.weight));
        }
    }

    return pot;
}


double solve(const vector<P>& ps, const map<int, vi>& edges, int level)
{
    const int N = ps.size();

    // Creates subgraph
    int id = 0;
    vi used(ps.size());
    vector<SubVertex> svs;
    for (int i = 0; i < ps.size(); ++i) {
        if (used[i])
            continue;
        if (ps[i].z > level) {
            used[i] = true;
            continue;
        }

        SubVertex sv(id);
        gather(i, ps, edges, level, used, sv);
        assert(!sv.vs.empty());

        int k = 0;
        for (int j = 0; j < sv.vs.size(); ++j)
            k += ps[sv.vs[j]].k;
        sv.k = k;

        sort(sv.vs.begin(), sv.vs.end());

        if (sv.k == 1 && !(sv.vs.front() == 0 || sv.vs.back() == N - 1))
            continue;

        svs.push_back(sv);
        ++id;
    }

    // If the first svs contains 0 and N-1, it's easy.
    if (svs[0].vs.size() >= 2 && svs[0].vs.front() == 0 && svs[0].vs.back() == N - 1)
        return 0.5 * svs[0].k;

    int startV = 0;
    int goalV;
    for (int i = 0; i < svs.size(); ++i) {
        if (svs[i].vs.back() == N - 1) {
            goalV = i;
            break;
        }
    }

    Graph g(svs.size());
    for (int i = 0; i < svs.size(); ++i) {
        for (int j = i + 1; j < svs.size(); ++j) {
            double dist = 1E+10;
            for (int ii = 0; ii < svs[i].vs.size(); ++ii) {
                if (ps[svs[i].vs[ii]].k == 0)
                    continue;
                for (int jj = 0; jj < svs[j].vs.size(); ++jj) {
                    if (ps[svs[j].vs[jj]].k == 0)
                        continue;
                    double d = calcDistance(ps[svs[i].vs[ii]], ps[svs[j].vs[jj]]);
                    if (d < dist)
                        dist = d;
                }
            }

            if (dist < 1E+10) {
                g[i].push_back(Edge(j, dist + (svs[j].k - 2) * 0.5));
                g[j].push_back(Edge(i, dist + (svs[i].k - 2) * 0.5));
            }
        }
    }

    Potential pot = dijkstra(g, startV);
    if (pot.count(goalV))
        return pot[goalV] + svs[startV].k * 0.5;
    return 1E+10;
}

int main(void)
{
    int caseNo = 0;
    for (int N, M; cin >> N >> M; ) {
        vector<P> ps(N);
        set<int> levels;
        for (int i = 0; i < N; ++i) {
            cin >> ps[i].x >> ps[i].y >> ps[i].z >> ps[i].k;
            levels.insert(ps[i].z);
        }
        
        map<int, vi> edges;
        for (int i = 0; i < M; ++i) {
            int a, b; cin >> a >> b;
            edges[a-1].push_back(b-1);
            edges[b-1].push_back(a-1);
        }

        double result = 1E+10;
        for (set<int>::iterator it = levels.begin(); it != levels.end(); ++it) {
            if (*it < ps[0].z || *it < ps[N-1].z)
                continue;
            double r = solve(ps, edges, *it);
            if (r < 0)
                continue;
            result = min(result, r);
        }

        if (result >= 1E+9)
            printf("Case %d: impossible\n", ++caseNo);
        else
            printf("Case %d: %.4f\n", ++caseNo, result);
    }
    return 0;
}

