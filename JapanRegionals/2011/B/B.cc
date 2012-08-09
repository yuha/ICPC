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

int dh[] = { -1, -1, 0, 1, 1,  1,  0, -1 };
int dw[] = {  0,  1, 1, 1, 0, -1, -1, -1 };

int main(void)
{
    for (int H, W; cin >> H >> W, H || W; ) {
        vector<string> donuts(H);
        map<string, int> counter;

        for (int i = 0; i < H; ++i)
            cin >> donuts[i];

        int N = H * W + 1;
        char buf[1024];
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W; ++w) {
                for (int d = 0; d < 8; ++d) {
                    int hh = h, ww = w;
                    for (int i = 0; i < N; ++i) {
                        buf[i] = donuts[hh][ww];
                        buf[i + 1] = '\0';
                        counter[buf] += 1;

                        hh = (hh + dh[d] + H) % H;
                        ww = (ww + dw[d] + W) % W;
                        if (hh == h && ww == w)
                            break;
                    }
                }
            }
        }

        int maxLength = 0;
        int maxCount = 0;
        string answer;

        for (map<string, int>::iterator it = counter.begin(); it != counter.end(); ++it) {
            if (it->second <= 1)
                continue;
            
            if (it->first.length() == maxLength && it->second > maxCount) {
                maxCount = it->second;
                answer = it->first;
            } else if (it->first.length() > maxLength) {
                maxLength = it->first.length();
                maxCount = it->second;
                answer = it->first;
            }
        }

        if (answer.length() >= 2)
            cout << answer << endl;
        else
            cout << 0 << endl;
    }

    
    return 0;
}

