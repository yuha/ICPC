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

const double EPS = 1E-6;

double f(const vector<double>& coef, double xlow, double x)
{
    double result = 0;
    for (int i = 0; i < coef.size(); ++i)
        result += coef[i] / (i + 1) * (pow(x, i + 1) - pow(xlow, i + 1));

    return result;
}

double binary(const vector<double>& coef, double xlow, double xhigh, double target)
{
    double left = xlow, right = xhigh;
    while (right - left > EPS) {
        double mid = left + (right - left) / 2;
        double vol = f(coef, xlow, mid);
        if (target < vol)
            right = mid;
        else
            left = mid;
    }

    return left;
}

vector<double> integralCoef(const vector<double>& coef)
{
    vector<double> result(coef.size() * 2);
    for (int i = 0; i < coef.size(); ++i) {
        for (int j = 0; j < coef.size(); ++j)
            result[i + j] += coef[i] * coef[j];
    }

    for (int i = 0; i < result.size(); ++i)
        result[i] *= M_PI;

    return result;
}

int main(void)
{
    int caseNo = 0;
    for (int n; cin >> n; ) {
        vector<double> coef(n + 1);
        for (int i = 0; i <= n; ++i)
            cin >> coef[i];
        double xlow, xhigh, inc; cin >> xlow >> xhigh >> inc;

        coef = integralCoef(coef);

        double full = f(coef, xlow, xhigh);
        printf("Case %d: %.2f\n", ++caseNo, full);
        if (full < inc)
            printf("insufficient volume\n");
        else {
            for (int i = 1; i <= 8; ++i) {
                if (i * inc > full)
                    break;
                if (i != 1)
                    putchar(' ');
                printf("%.2f", binary(coef, xlow, xhigh, i * inc) - xlow);
            }
            printf("\n");
        }
    }
    
    
    return 0;
}

