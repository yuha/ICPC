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

// 上から見て、上が上三角の場合
//   00 01 02 03 04 05 06 07 08 09   10 11 12 13 14 15 16 17 18 19
// 変換なし
//   01 10 06 13 04 08 17 09 12 19   20 14 11 07 18 16 15 03 05 02
// 上へ
//   13 01 12 19 10 06 17 09 18 16   07 20 05 02 14 11 15 03 04 08
// 左へ
//   06 08 01 09 04 02 10 03 13 19   14 18 15 20 12 16 17 11 05 07
// 右へ
//   10 04 17 01 05 08 15 06 12 13   11 16 20 03 18 19 14 09 07 02

int VERT[2][20], LEFT[2][20], RIGHT[2][20];

void init()
{
    int origs[20]  = { 1, 10, 6, 13, 4, 8, 17, 9, 12, 19, 20, 14, 11, 7, 18, 16, 15, 3, 5, 2 };
    int ups[20]    = { 13, 1, 12, 19, 10, 6, 17, 9, 18, 16, 7, 20, 5, 2, 14, 11, 15, 3, 4, 8 };
    int lefts[20]  = { 6, 8, 1, 9, 4, 2, 10, 3, 13, 19, 14, 18, 15, 20, 12, 16, 17, 11, 5, 7 };
    int rights[20] = { 10, 4, 17, 1, 5, 8, 15, 6, 12, 13, 11, 16, 20, 3, 18, 19, 14, 9, 7, 2 };

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (ups[j] == origs[i])
                VERT[0][j] = i;
            if (lefts[j] == origs[i])
                RIGHT[0][j] = i;
            if (rights[j] == origs[i])
                LEFT[0][j] = i;

            if (origs[j] == ups[i])
                VERT[1][j] = i;
            if (origs[j] == lefts[i])
                LEFT[1][j] = i;
            if (origs[j] == rights[i])
                RIGHT[1][j] = i;
        }
    }
}

class Robot {
public:
    Robot() : vs(20), x(0), y(0) {
        static int initials[] = {
            0, 4, 1, 5, 3, 2, 9, 6, 10, 11,
            18, 19, 17, 13, 15, 16, 14, 12, 8, 7,
        };

        for (int i = 0; i < 20; ++i)
            vs[i] = initials[i];
    }

    Robot& vert() {
        if ((x + y) % 2 == 0) {
            y += 1;
            apply(VERT[0]);
        } else {
            y -= 1;
            apply(VERT[1]);
        }

        return *this;
    }

    Robot& left() {
        if ((x + y) % 2 == 0)
            apply(LEFT[0]);
        else
            apply(LEFT[1]);

        x -= 1;
        return *this;
    }

    Robot& right() {
        if ((x + y) % 2 == 0)
            apply(RIGHT[0]);
        else
            apply(RIGHT[1]);

        x += 1;
        return *this;
    }

    int value() {
        return vs[0];
    }

    friend bool operator<(const Robot& lhs, const Robot& rhs) {
        if (lhs.x != rhs.x)
            return lhs.x < rhs.x;
        if (lhs.y != rhs.y)
            return lhs.y < rhs.y;
        return lhs.vs < rhs.vs;
    }

    void apply(int* conv) {
        vi vs2(20);
        for (int i = 0; i < 20; ++i)
            vs2[i] = vs[conv[i]];
        
        swap(vs, vs2);
    }

    vi vs;
    int x, y;
};

void createCache(int answers[201][201][20])
{
    int result = INT_MAX;
    map<Robot, int> cache;
    queue<pair<Robot, int> > rs;

    cache[Robot()] = 0;
    rs.push(make_pair(Robot(), 0));

    while (!rs.empty()) {        
        pair<Robot, int> p = rs.front(); rs.pop();

        Robot r = p.first;
        int steps = p.second;
        if (steps > 100)
            continue;

        r.vert();
        {
            if (!cache.count(r)) {
                cache[r] = steps + 1;
                if (answers[r.x+100][r.y+100][r.value()] < 0)
                    answers[r.x+100][r.y+100][r.value()] = steps + 1;
                rs.push(make_pair(r, steps + 1));
            }
        }
        r.vert();

        r.left();
        {
            if (!cache.count(r)) {
                cache[r] = steps + 1;
                if (answers[r.x+100][r.y+100][r.value()] < 0)
                    answers[r.x+100][r.y+100][r.value()] = steps + 1;
                rs.push(make_pair(r, steps + 1));
            }
        }
        r.right();

        r.right();
        {
            if (!cache.count(r)) {
                cache[r] = steps + 1;
                if (answers[r.x+100][r.y+100][r.value()] < 0)
                    answers[r.x+100][r.y+100][r.value()] = steps + 1;
                rs.push(make_pair(r, steps + 1));
            }
        }
        r.left();
    }
}

int answers[201][201][20];

int main(void) {
    init();
    memset(answers, -1, sizeof(answers));
    createCache(answers);

    for (int x, y, c; cin >> x >> y >> c, (x || y || c); ) {
        cout << answers[x+100][y+100][c] << endl;
    }
    
    return 0;
}

