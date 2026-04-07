#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int RED = 0;
const int BLUE = 1;

int gametime;

string warriortype[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
int warriorstrength[5];

class Warrior {
private:
	string name;
	int camp;
	int id;
	int strength;
public:
	Warrior(string n, int c, int i, int s) : name(n), camp(c), id(i), strength(s) {}
};

class dragon : public Warrior {
private:
public:
	dragon(int c, int i, int s) : Warrior("dragon", c, i, s) {}
};

class ninja : public Warrior {
private:
public:
	ninja(int c, int i, int s) : Warrior("ninja", c, i, s) {}
};

class iceman : public Warrior {
private:
public:
	iceman(int c, int i, int s) : Warrior("iceman", c, i, s) {}
};

class lion : public Warrior {
private:
public:
	lion(int c, int i, int s) : Warrior("lion", c, i, s) {}
};

class wolf : public Warrior {
private:
public:
	wolf(int c, int i, int s) : Warrior("wolf", c, i, s) {}
};

class Headquarter {
private:
	int camp;
	int life;
	int warriorid;
	int warriornum[5];
	int warriororder[5];
	int index;
public:
	bool stop;
	Headquarter(int c, int l, int* order) : camp(c), life(l), warriorid(0), warriornum{}, index(0), stop(false) {
		memcpy(warriororder, order, sizeof(int) * 5);
	}
	void produce() {
		if(stop) {
			return;
		}
		int t = 5;
		while(t--) {
			int w = warriororder[index];
			index = (index + 1) % 5;
			if(life >= warriorstrength[w]) {
				life -= warriorstrength[w];
				warriorid++;
				printf("%03d %s %s %d born with strength %d,", gametime, camp == RED ? "red" : "blue", warriortype[w].c_str(), warriorid, warriorstrength[w]);
				warriornum[w]++;
				printf("%d %s in %s headquarter\n", warriornum[w], warriortype[w].c_str(), camp == RED ? "red" : "blue");
				return;
			}
		}
		printf("%03d %s headquarter stops making warriors\n", gametime, camp == RED ? "red" : "blue");
		stop = true;
	}
};

int main() {
	int caseNum;
	cin >> caseNum;
	int redorder[5] = { 2, 3, 4, 1, 0 };
	int blueorder[5] = { 3, 0, 1, 2, 4 };
	for (int i = 1; i <= caseNum; i++) {
		cout << "Case:" << i << endl;
		int life;
		cin >> life;
		Headquarter red(RED, life, redorder);
		Headquarter blue(BLUE, life, blueorder);
		for(int j = 0; j < 5; j++) {
			cin >> warriorstrength[j];
		}
		for(gametime = 0; ; gametime++) {
			red.produce();
			blue.produce();
			if(red.stop && blue.stop) {
				break;
			}
		}
	}
	return 0;
}
