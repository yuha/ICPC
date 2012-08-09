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
#include <sstream>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

struct LengthComparator {
    bool operator()(const string& lhs, const string& rhs) {
        if (lhs.length() != rhs.length())
            return lhs.length() > rhs.length();

        return lhs > rhs;
    }
};

void iter(int ith,
          const vector<char>& currentMapping,
          bool& answerFound,
          string& answerIfFound,
          const vector<vector<string> >& dict,
          const vector<string>& ciph,
          const string& original)
{
    if (ith == ciph.size()) {
        string answer = original;
        for (int i = 0; i < answer.length(); ++i) {
            if (answer[i] == '.' || answer[i] == ' ')
                continue;

            answer[i] = currentMapping[answer[i] - 'A'];
        }

        // Found another answer!
        if (answerFound && answer != answerIfFound)
            throw "-.";

        answerFound = true;
        answerIfFound = answer;
        return;
    }

    int len = ciph[ith].length();
    for (int i = 0; i < dict[len].size(); ++i) {
        vector<char> newMapping = currentMapping;
        
        const string& plainWord = dict[len][i];
        const string& ciphWord = ciph[ith];

        for (int j = 0; j < len; ++j) {
            if (newMapping[ciphWord[j] - 'A'] == '-')
                newMapping[ciphWord[j] - 'A'] = plainWord[j];
            else if (newMapping[ciphWord[j] - 'A'] != plainWord[j])
                goto next;

            if (newMapping[plainWord[j] - 'A'] == '-')
                newMapping[plainWord[j] - 'A'] = ciphWord[j];
            else if (newMapping[plainWord[j] - 'A'] != ciphWord[j])
                goto next;
        }

        iter(ith + 1, newMapping, answerFound, answerIfFound, dict, ciph, original);
    next:
        ;
    }
}

string solve(const vector<vector<string> >& dict,
             const vector<string>& ciph,
             const string& original)
{
    string answer;
    vector<char> mapping(26, '-');

    bool answerFound = false;
    try {
        iter(0, mapping, answerFound, answer, dict, ciph, original);
    } catch (const char* e) {
        return e;
    }

    return answer;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        string str;

        vector<vector<string> > dict(100); // length -> string
        for (int i = 0; i < N; ++i) {
            cin >> str;
            dict[str.length()].push_back(str);
        }

        getline(cin, str); // skip '\n'

        string ciph;
        getline(cin, ciph);

        istringstream ss(ciph);
        vector<string> words;
        while (ss >> str) {
            if (str[str.length() - 1] == '.')
                str = str.substr(0, str.length() - 1);

            words.push_back(str);
        }

        sort(words.begin(), words.end(), LengthComparator());

        cout << solve(dict, words, ciph) << endl;
    }
    return 0;
}

