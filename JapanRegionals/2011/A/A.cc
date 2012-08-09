#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

int canonicalTime(int h, int m)
{
    return h * 60 + m;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        map<string, int> totalTime;

        bool existsGod = false;
        map<string, int> enteredAt;
        while (N--) {
            int M, D, h, m;
            char e, id[80];
            scanf("%d/%d %d:%d %c %s", &M, &D, &h, &m, &e, id);
            
            int time = canonicalTime(h, m);
            if (e == 'I' && id == string("000")) {
                existsGod = true;
                for (map<string, int>::iterator it = enteredAt.begin(); it != enteredAt.end(); ++it) {
                    it->second = time;
                }
            } else if (e == 'O' && id == string("000")) {
                existsGod = false;
                for (map<string, int>::iterator it = enteredAt.begin(); it != enteredAt.end(); ++it) {
                    totalTime[it->first] += time - it->second;
                    it->second = time;
                }                
            } else if (e == 'I') {
                enteredAt[id] = time;
            } else if (e == 'O') {
                if (existsGod)
                    totalTime[id] += time - enteredAt[id];
                enteredAt.erase(id);
            }
        }

        string maxId = "";
        int maxTime = 0;
        for (map<string, int>::iterator it = totalTime.begin(); it != totalTime.end(); ++it) {
            if (it->first == "000")
                continue;

            if (it->second > maxTime) {
                maxId = it->first;
                maxTime = it->second;
            }                
        }

        cout << maxTime << endl;
    }

    return 0;
}
