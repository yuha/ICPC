#include<iostream>

using namespace std;

#define DEBUG	cout<<"DEBUG:"<<__LINE__<<endl

#define BLANK		0
#define WALL		-1
#define VISIT		-2


//#define CHANNEL		2

int map[30][30];
int answer[30][30];
long visit[30][30];
int maxlength;
int h, w;

#define LEFT	0
#define UP		1
#define RIGHT	2
#define DOWN	3


//                    L,  U,  R,  D
int ADJACENT_I[4] = { 0, -1,  0,  1};
int ADJACENT_J[4] = {-1,  0,  1,  0};

/**

  LL        |  LU        |  LR         |  LD
  ..x  .Xx  |  ..x  .2x  |             |  ..x  XXx
  .10  210  |  .10  X10  |             |  .10  X10
  ..x  .Xx  |  ..x  XXx  |             |  ..x  .2x
            |            |             |
------------+------------+-------------+--------------
  UL        |  UU        |  UR         |  UD
  ...  .XX  |  ...  .2.  |  ...  XX.   |
  .1.  21X  |  .1.  X1X  |  .1.  X12   |
  x0x  x0x  |  x0x  x0x  |  x0x  x0x   |
            |            |             |
------------+------------+-------------+--------------
  RL        |  RU        |  RR         |  RD
            |  x..  x2.  |  x..  xX.   |  x..  xXX
            |  01.  01X  |  01.  012   |  01.  01X
            |  x..  xXX  |  x..  xX.   |  x..  x2.
            |            |             |
------------+------------+-------------+--------------
  DL        |  DU        |  DR         |  DD
  x0x  x0x  |            |  x0x  x0x   |  x0x  x0x
  .1.  21X  |            |  .1.  X12   |  .1.  X1X
  ...  .XX  |            |  ...  XX.   |  ...  .2.
            |            |             |

**/

#define STP		100

int BLOCKED[4][4][4][2] = {
	{	//L
		{{  1,   0}, { -1,   0}, {STP, STP}, {STP, STP}},	//	LL
		{{  0,  -1}, {  1,  -1}, {  1,   0}, {STP, STP}},	//	LU
		{{STP, STP}, {STP, STP}, {STP, STP}, {STP, STP}},	//	LR
		{{  0,  -1}, { -1,  -1}, { -1,   0}, {STP, STP}},	//	LD
	},
	
	{	//U
		{{ -1,   0}, { -1,   1}, {  0,   1}, {STP, STP}},	//	UL
		{{  0,  -1}, {  0,   1}, {STP, STP}, {STP, STP}},	//	UU
		{{  0,  -1}, { -1,  -1}, { -1,   0}, {STP, STP}},	//	UR
		{{STP, STP}, {STP, STP}, {STP, STP}, {STP, STP}},	//	UD
	},
	
	{	//R
		{{STP, STP}, {STP, STP}, {STP, STP}, {STP, STP}},	//	RL
		{{  1,   0}, {  1,   1}, {  0,   1}, {STP, STP}},	//	RU
		{{  1,   0}, { -1,   0}, {STP, STP}, {STP, STP}},	//	RR
		{{ -1,   0}, { -1,   1}, {  0,   1}, {STP, STP}},	//	RD
	},
	
	{	//U
		{{  0,   1}, {  1,   1}, {  1,   0}, {STP, STP}},	//	DL
		{{STP, STP}, {STP, STP}, {STP, STP}, {STP, STP}},	//	DU
		{{  0,  -1}, {  1,  -1}, {  1,   0}, {STP, STP}},	//	DR
		{{  0,  -1}, {  0,   1}, {STP, STP}, {STP, STP}},	//	DD
	},
};



/**/
/*
int ADJACENT_I[4] = {1, 0, -1, 0};
int ADJACENT_J[4] = {0, 1, 0, -1};
*/
int counter;
long visitCounter;

int DI[8] = { 1,  1,  1,  0,  0, -1, -1, -1};
int DJ[8] = { 1,  0, -1,  1, -1,  1,  0, -1};








void printMap() {
	for (int i = 1; i <= h; i++) {
		for (int j = 1; j <= w; j++) {
			switch(map[i][j]) {
			case WALL:
				cout << "#";
				break;
			case VISIT:
				cout << "C";
				break;
			case BLANK:
				cout << ".";
				break;
			default:
				cout << map[i][j];
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}



bool dfs2(int nowI, int nowJ) {
	if (nowI == h && nowJ == w) {
		return true;
	}
	if (visit[nowI][nowJ] == visitCounter) return false;
	if (map[nowI][nowJ] != BLANK) return false;
	visit[nowI][nowJ] = visitCounter;
	
	for (int a = 0; a < 4; a++) {
		int nextI = nowI + ADJACENT_I[a];
		int nextJ = nowJ + ADJACENT_J[a];
		if (dfs2(nextI, nextJ)) {
			return true;
		}
	}
	return false;
}

bool reachableToGoal(int nowI, int nowJ, int length) {
	/*
	for (int i = 0; i < h+2; i++) {
		for (int j = 0; j < w+2; j++) {
			if (map[i][j] == BLANK){
				visit[i][j] = false;
			} else if (map[i][j] <= length - 2) {
				for (int a =~0; a < 8; a++) {
					visit[i+DI[a]][j+DJ[a]] = true;
				}
			} else {
				visit[i][j] = true;
			}
		}
	}
	visit[nowI][nowJ] = false;
	*/
	bool result = dfs2(nowI, nowJ);
	visitCounter++;
	return result;
}

bool check(int nextI, int nextJ, int nextLength) {
	for (int a = 0; a< 8; a++) {
		if(map[nextI + DI[a]][nextJ + DJ[a]] < nextLength - 2) {
			return false;
		}
	}
	return true;
}

int expectedMaxLength(int length) {
	
	int result = length;
	for (int i = 1; i <= h; i++) {
		for (int j = 1; j <= w; j++) {
			if (map[i][j] != BLANK) {
				continue;
			}
			if (check(i, j, length)) {
				result++;
			}
		}
	}
	return result;
}

/*
void dfs(int nowI, int nowJ, int length) {
//	DEBUG;
//	printMap();
	counter++;
	
	if (nowI == h && nowJ == w) {
		if (maxlength < length) {
			for (int i = 0; i < h+2; i++) {
				for (int j = 0; j < w+2; j++) {
					answer[i][j] = map[i][j];
				}
			}
			maxlength = length;
		}
		return;
	}
	
	if (expectedMaxLength(length) <= maxlength) {
		return;
	}
	
	
	for (int a = 0; a < 4; a++) {
		int nextI = nowI + ADJACENT_I[a];
		int nextJ = nowJ + ADJACENT_J[a];
		if (map[nextI][nextJ] != BLANK) {
			continue;
		}
		if (!check(nextI, nextJ, length+1)) {
			continue;
		}
		if (!reachableToGoal(nextI, nextJ, length+1)) {
			continue;
		}
		map[nextI][nextJ] = length+1;
		dfs(nextI, nextJ, length+1);
		map[nextI][nextJ] = BLANK;
	}
}
*/


void dfs2(int nowI, int nowJ, int length, int rest, int previousDirection) {
	/*
	DEBUG;
	printMap();
	DEBUG;
	cout << nowI << endl;
	cout << nowJ << endl;
	cout << length << endl;
	cout << rest << endl;
	cout << previousDirection << endl;
	*/
	if (nowI == h && nowJ == w) {
		/*
		DEBUG;
		printMap();
		DEBUG;
		*/
		if (maxlength < length) {
			for (int i = 0; i < h+2; i++) {
				for (int j = 0; j < w+2; j++) {
					answer[i][j] = map[i][j];
				}
			}
			maxlength = length;
		}
		return;
	}
	
	if (length + rest <= maxlength) {
		/*
		DEBUG;
		cout << length << endl;
		cout << rest << endl;
		cout << maxlength << endl;
		*/
		return;
	}
	
	for (int nextDirection = 0; nextDirection < 4; nextDirection++) {
		int nextI = nowI + ADJACENT_I[nextDirection];
		int nextJ = nowJ + ADJACENT_J[nextDirection];
		/*
		DEBUG;
		cout << nextI << endl;
		cout << nextJ << endl;
		DEBUG;
		*/
		if (map[nextI][nextJ] != BLANK) {
//			DEBUG;
			continue;
		}
		for (int k = 0; BLOCKED[previousDirection][nextDirection][k][0] != STP; k++) {
			int blockedI = nowI + BLOCKED[previousDirection][nextDirection][k][0];
			int blockedJ = nowJ + BLOCKED[previousDirection][nextDirection][k][1];
			/*
			DEBUG;
			cout << blockedI << endl;
			cout << blockedJ << endl;
			*/
			if (map[blockedI][blockedJ] == BLANK) {
				rest--;
			}
			if (map[blockedI][blockedJ] >= 0) {
				map[blockedI][blockedJ]++;
			}
		}
		
		map[nextI][nextJ] = VISIT;
		dfs2(nextI, nextJ, length+1, rest-1, nextDirection);
		map[nextI][nextJ] = BLANK;
		
		
		for (int k = 0; BLOCKED[previousDirection][nextDirection][k][0] != STP; k++) {
			int blockedI = nowI + BLOCKED[previousDirection][nextDirection][k][0];
			int blockedJ = nowJ + BLOCKED[previousDirection][nextDirection][k][1];
			if (map[blockedI][blockedJ] > 0) {
				map[blockedI][blockedJ]--;
			}
			if (map[blockedI][blockedJ] == BLANK) {
				rest++;
			}
		}
	}
}

int countBlank() {
	int count = 0;
	for (int i = 1; i <= h; i++) {
		for (int j = 1; j <= w; j++) {
			if (map[i][j] == BLANK) {
				count++;
			}
		}
	}
	return count;
}

void startDfs(int startDirection, int nextI, int nextJ, int blockedI, int blockedJ) {
	if (map[nextI][nextJ] == WALL) {
		return;
	}
	map[nextI][nextJ] = VISIT;
	if (map[blockedI][blockedJ] == BLANK) {
		map[blockedI][blockedJ]++;
	}
	
	dfs2(nextI, nextJ, 2, countBlank(), startDirection);
	
	if (map[blockedI][blockedJ] > 0) {
		map[blockedI][blockedJ]--;
	}
	map[nextI][nextJ] = BLANK;
	
}

void solve(const string* input) {
	for (int i = 0; i < h+2; i++) {
		for (int j = 0; j < w+2; j++) {
			map[i][j] = WALL;
			visit[i][j] = -1;
		}
	}
	
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if(input[i][j] == '.') {
//				DEBUG;
				map[i+1][j+1] = BLANK;
			}
		}
	}
//	printMap();
	maxlength = 0;
	visitCounter = 0;
	
	map[1][1] = VISIT;
	startDfs(RIGHT, 1, 2, 2, 1);
	startDfs(DOWN,  2, 1, 1, 2);
	/*
	
	map[2][1] = VISIT;
	map[1][2] = 1;
	dfs2(2, 1, 2, w*h-3, RIGHT);
	
	map[1][2] = VISIT;
	map[2][1] = 1;
	dfs2(1, 2, 2, w*h-3, DOWN);
	
	
//	dfs(1, 1, 1);
	*/
	
	DEBUG;
	int step = 0;
	for (int i = 1; i <= h; i++) {
		for (int j = 1; j <= w; j++) {
			switch(answer[i][j]) {
			case WALL:
				cout << "#";
				break;
			case VISIT:
				cout << "C";
				step++;
				break;
			default:
				cout << ".";
				break;
			}
		}
		cout << endl;
	}
	cout << step << endl;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		cin >> h >> w;
		if (h == 0 && w == 0) {
			break;
		}
		
		string input[h];
		for (int i = 0; i < h; i++) {
			cin >> input[i];
		}
		cout << "Case " << casenumber << ":" << endl;
		counter = 0;
		solve(input);
	}
}
