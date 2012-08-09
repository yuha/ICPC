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
typedef vector<string> vs;

pair<int, int> exprWithoutBase(const vs& rect, int top, int bottom, int left, int right);
pair<int, int> expr(const vs& rect, int base, int top, int bottom, int left, int right);
pair<int, int> term(const vs& rect, int base, int top, int bottom, int left, int right);
pair<int, int> factor(const vs& rect, int base, int top, int bottom, int left, int right);
pair<int, int> powexpr(const vs& rect, int base, int top, int bottom, int left, int right);
pair<int, int> primary(const vs& rect, int base, int top, int bottom, int left, int right);
pair<int, int> fraction(const vs& rect, int base, int top, int bottom, int left, int right);

int inv(int x)
{
    int v = 1;
    for (int i = 0; i < 2009; ++i) {
        v = (v * x) % 2011;
    }

    return v;
}

pair<int, int> fraction(const vs& rect, int base, int top, int bottom, int left, int right)
{
    int newRight = left;
    while (newRight < right && rect[base][newRight] == '-')
        ++newRight;
    
    pair<int, int> p1 = exprWithoutBase(rect, top, base, left, newRight);
    pair<int, int> p2 = exprWithoutBase(rect, base + 1, bottom, left, newRight);

    return make_pair((p1.first * inv(p2.first)) % 2011, newRight);
}

pair<int, int> primary(const vs& rect, int base, int top, int bottom, int left, int right)
{
    if (rect[base][left] == '(') {
        pair<int, int> p = expr(rect, base, top, bottom, left + 2, right);
        assert(rect[base][p.second + 1] == ')');
        return make_pair(p.first, p.second + 2);
    }

    assert('0' <= rect[base][left] && rect[base][left] <= '9');
    return make_pair(rect[base][left] - '0', left + 1);
}

pair<int, int> powexpr(const vs& rect, int base, int top, int bottom, int left, int right)
{
    pair<int, int> p = primary(rect, base, top, bottom, left, right);
    if (p.second < right && top <= base - 1 && '0' <= rect[base-1][p.second] && rect[base-1][p.second] <= '9') {
        int newValue = 1;
        int exp = rect[base-1][p.second] - '0';
        while (exp--)
            newValue = newValue * p.first % 2011;

        return make_pair(newValue, p.second + 1);
    }

    return p;
}

pair<int, int> factor(const vs& rect, int base, int top, int bottom, int left, int right)
{
    if (left + 2 < right && rect[base][left] == '-' && rect[base][left + 1] == '-')
        return fraction(rect, base, top, bottom, left, right);

    if (rect[base][left] == '-') {
        pair<int, int> p = factor(rect, base, top, bottom, left + 2, right);
        return make_pair((-p.first + 2011) % 2011, p.second);
    }

    return powexpr(rect, base, top, bottom, left, right);
}

pair<int, int> term(const vs& rect, int base, int top, int bottom, int left, int right)
{
    pair<int, int> p = factor(rect, base, top, bottom, left, right);
    int newValue = p.first;
    int newRight = p.second;

    while (newRight + 3 < right) {
        if (rect[base][newRight + 1] == '*') {
            pair<int, int> q = term(rect, base, top, bottom, newRight + 3, right);
            
            newValue = (newValue * q.first) % 2011;
            newRight = q.second;
        } else {
            break;
        }
    }

    return make_pair(newValue, newRight);
}

pair<int, int> expr(const vs& rect, int base, int top, int bottom, int left, int right)
{
    pair<int, int> p = term(rect, base, top, bottom, left, right);
    int newValue = p.first;
    int newRight = p.second;

    while (newRight + 3 < right) {
        if (rect[base][newRight + 1] == '+') {
            pair<int, int> q = term(rect, base, top, bottom, newRight + 3, right);

            newValue = (newValue + q.first) % 2011;
            newRight = q.second;
        } else if (rect[base][newRight + 1] == '-') {
            pair<int, int> q = term(rect, base, top, bottom, newRight + 3, right);

            newValue = (newValue - q.first + 2011) % 2011;
            newRight = q.second;
        } else {
            break;
        }        
    }

    return make_pair(newValue, newRight);
}

pair<int, int> exprWithoutBase(const vs& rect, int top, int bottom, int left, int right)
{
    for (int i = top; i < bottom; ++i) {
        if (rect[i][left] != '.')
            return expr(rect, i, top, bottom, left, right);
    }

    return exprWithoutBase(rect, top, bottom, left + 1, right);
}

int main(void)
{    
    for (int N; cin >> N, N; ) {
        vs rect(N);
        for (int i = 0; i < N; ++i)
            cin >> rect[i];

        pair<int, int> p = exprWithoutBase(rect, 0, N, 0, rect[0].size());
        cout << p.first << endl;
    }
    return 0;
}

