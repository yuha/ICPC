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

struct Vertex {
    int a;
    int b;
    unsigned long long cs;

    Vertex() : a(-1), b(-1) {}
    Vertex(int a, int b, unsigned long long cs) : a(a), b(b), cs(cs) {}

    friend bool operator<(const Vertex& lhs, const Vertex& rhs) {
        if (lhs.a != rhs.a)
            return lhs.a < rhs.a;
        if (lhs.b != rhs.b)
            return lhs.b < rhs.b;
        return lhs.cs < rhs.cs;
    }

    friend bool operator>(const Vertex& lhs, const Vertex& rhs) {
        if (lhs.a != rhs.a)
            return lhs.a > rhs.a;
        if (lhs.b != rhs.b)
            return lhs.b > rhs.b;
        return lhs.cs > rhs.cs;
    }
};

typedef int Weight;

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;
typedef map<Vertex, Edges> Graph;

// for MST / Dijkstra
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

typedef map<pair<int, int>, Weight> Potential;
// typedef map<Vertex, Weight> Potential;

int dijkstra(const Vertex& startV, int goal, 
             const vi& altitudes, const vi& fees,
             const vector<vector<pair<int, int> > >& edges, 
             const vector<vector<pair<int, int> > >& redges) {
    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;

    Q.push(Edge(startV, 0));

    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex p = edge.dest;
        Weight curr = edge.weight;

        if (p.a == goal && p.b == goal)
            return curr;

        if (pot.count(make_pair(p.a, p.b))) { continue; }
        pot[make_pair(p.a, p.b)] = curr;

        int maxAltitude = max(altitudes[p.a], altitudes[p.b]);

        // (a, b) -> (a', b)
        for (int i = 0; i < edges[p.a].size(); ++i) {            
            int destA = edges[p.a][i].first;
            int costA = edges[p.a][i].second;

            if (altitudes[destA] < maxAltitude)
                continue;

            if (altitudes[destA] == maxAltitude) {
                Vertex v(destA, p.b, p.cs | (1ULL << destA));
                if (pot.count(make_pair(destA, p.b)))
                    continue;

                int c = (p.cs & (1ULL << destA)) ? 0 : fees[destA];
                Q.push(Edge(v, curr + costA + c));
            } else {
                Vertex v(destA, p.b, 1ULL << destA);
                if (pot.count(make_pair(destA, p.b)))
                    continue;

                Q.push(Edge(v, curr + costA + fees[destA]));
            }
        }

        // (a, b) -> (a, b')
        for (int i = 0; i < redges[p.b].size(); ++i) {            
            int destB = redges[p.b][i].first;
            int costB = redges[p.b][i].second;

            if (altitudes[destB] < maxAltitude)
                continue;

            if (altitudes[destB] == maxAltitude) {
                Vertex v(p.a, destB, p.cs | (1ULL << destB));
                if (pot.count(make_pair(p.a, destB)))
                    continue;

                int c = (p.cs & (1ULL << destB)) ? 0 : fees[destB];
                Q.push(Edge(v, curr + costB + c));
            } else {
                Vertex v(p.a, destB, 1ULL << destB);
                if (pot.count(make_pair(p.a, destB)))
                    continue;

                Q.push(Edge(v, curr + costB + fees[destB]));
            }
        }
    }

    return -1;
}

int solve(int N, const vi& altitudes, const vi& fees,
          const vector<vector<pair<int, int> > >& edges,
          const vector<vector<pair<int, int> > >& redges)
{
    Vertex v(0, 0, 0);

    return dijkstra(v, N-1, altitudes, fees, edges, redges);
}

int main(void)
{
    for (int N, M; cin >> N >> M, (N || M); ) {
        vi altitudes(N), fees(N);
        for (int i = 1; i < N - 1; ++i)
            cin >> fees[i] >> altitudes[i];
        altitudes[N-1] = 1000;

        vector<vector<pair<int, int> > >  edges(N);
        vector<vector<pair<int, int> > > redges(N);
        for (int i = 0; i < M; ++i) {
            int a, b, c; cin >> a >> b >> c;
            edges[a-1].push_back(make_pair(b-1, c));
            redges[b-1].push_back(make_pair(a-1, c));
        }

        cout << solve(N, altitudes, fees, edges, redges) << endl;
    }
    return 0;
}

