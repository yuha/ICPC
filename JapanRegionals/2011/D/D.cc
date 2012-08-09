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

class IntMap {
public:
    IntMap() : maxId(0) {}

    int add(const string& x) {
        if (m.count(x))
            return m[x];
        else
            m[x] = maxId++;
    }

    int find(const string& x) {
        if (m.count(x))
            return m[x];

        return -1;
    }

    int size() const {
        return maxId;
    }

private:
    int maxId;
    map<string, int> m;
    map<int, string> r;
};

typedef pair<int, int> Vertex; // 都市 * 最後に訪れた都市
typedef pair<int, int> Weight; // 全部の距離 * 最後に訪れた都市からの距離

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;

typedef vector<vector<pair<int, int> > > Graph; // 都市 -> (都市, 距離)

// for MST / Dijkstra
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

typedef map<Vertex, Weight> Potential;

int dijkstra(Graph& g, const Vertex& startV, const set<int>& gasCitiesSet, int dst, int cap) {
    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;

    Q.push(Edge(startV, Weight(0, 0)));

    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex p = edge.dest;
        Weight curr = edge.weight;

        // Found the destination.
        if (p.first == dst)
            return curr.first;

        if (pot.count(p)) { continue; } // already visited.
        pot[p] = curr;

        const int lastGasCity = p.second;
        const int lastGasDist = curr.second;
        for (int i = 0; i < g[p.first].size(); ++i) {
            const int destCity = g[p.first][i].first;
            const int edgeDist = g[p.first][i].second;

            // 既に訪れた
            if (pot.count(Vertex(destCity, lastGasCity)))
                continue;

            // 辿りつけない
            if (lastGasDist + edgeDist > cap)
                continue;

            {
                Vertex v(destCity, lastGasCity);
                Weight w(curr.first + edgeDist, lastGasDist + edgeDist);
                Q.push(Edge(v, w));
            }
            
            if (gasCitiesSet.count(destCity)) {
                Vertex v(destCity, destCity);
                Weight w(curr.first + edgeDist, 0);
                Q.push(Edge(v, w));                
            }
        }
    }

    return -1;
}

int solve(const vector<pair<pair<string, string>, int> >& roads,
          const vector<string>& gasCities,
          const string& src,
          const string& dst,
          int cap)
{
    IntMap m;
    for (int i = 0; i < roads.size(); ++i) {
        m.add(roads[i].first.first);
        m.add(roads[i].first.second);
    }

    set<int> gasCitiesSet;
    for (int i = 0; i < gasCities.size(); ++i)
        gasCitiesSet.insert(m.find(gasCities[i]));

    Graph g(m.size());
    for (int i = 0; i < roads.size(); ++i) {
        g[m.find(roads[i].first.first)].push_back(
            make_pair(m.find(roads[i].first.second), roads[i].second));
        g[m.find(roads[i].first.second)].push_back(
            make_pair(m.find(roads[i].first.first), roads[i].second));
    }

    return dijkstra(g, Vertex(m.find(src), m.find(src)), gasCitiesSet, m.find(dst), cap);
}

int main(void)
{
    for (int N, M, cap; cin >> N >> M >> cap, N || M || cap; ) {
        string src, dst; cin >> src >> dst;

        vector<pair<pair<string, string>, int> > roads;
        for (int i = 0; i < N; ++i) {
            string c1, c2; cin >> c1 >> c2;
            int d; cin >> d;

            roads.push_back(make_pair(make_pair(c1, c2), d));
        }

        vector<string> gasCities;
        for (int i = 0; i < M; ++i) {
            string s; cin >> s;
            gasCities.push_back(s);
        }

        cout << solve(roads, gasCities, src, dst, cap * 10) << endl;
    }
    return 0;
}

