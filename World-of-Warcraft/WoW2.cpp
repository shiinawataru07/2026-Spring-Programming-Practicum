#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int RED = 0;
const int BLUE = 1;

int gametime;

string warriortype[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string weapens[3] = {"sword", "bomb", "arrow" };
int warriorstrength[5];

class Warrior {
private:
	string name;
	int camp;
	int id;
	int strength;
	virtual void born() {}
public:
	Warrior(string n, int c, int i, int s) : name(n), camp(c), id(i), strength(s) {}
	friend class dragon;
	friend class ninja;
	friend class iceman;
	friend class lion;
	friend class wolf;
};

class dragon : public Warrior {
private:
	double morale;
	string weapon;
public:
	virtual void born() {
		weapon = weapens[id % 3];
		printf("It has a %s,and it's morale is %.2lf\n", weapon.c_str(), morale);
	}
	dragon(int c, int i, int s, double m) : Warrior("dragon", c, i, s), morale(m), weapon("") {}
};

class ninja : public Warrior {
private:
	string weapon1;
	string weapon2;
public:
	virtual void born() {
		weapon1 = weapens[id % 3];
		weapon2 = weapens[(id + 1) % 3];
		printf("It has a %s and a %s\n", weapon1.c_str(), weapon2.c_str());
	}
	ninja(int c, int i, int s) : Warrior("ninja", c, i, s) {}
};

class iceman : public Warrior {
private:
	string weapon;
public:
	virtual void born() {
		weapon = weapens[id % 3];
		printf("It has a %s\n", weapon.c_str());
	}
	iceman(int c, int i, int s) : Warrior("iceman", c, i, s) {}
};

class lion : public Warrior {
private:
	int loyalty;
public:
	virtual void born() {
		printf("It's loyalty is %d\n", loyalty);
	}
	lion(int c, int i, int s, int l) : Warrior("lion", c, i, s), loyalty(l) {}
};

class wolf : public Warrior {
private:
public:
	virtual void born() {}
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
				switch(w) {
					case 0: {
						dragon d(camp, warriorid, warriorstrength[w], (double)life / warriorstrength[0]);
						d.born();
						break;
					}
					case 1: {
						ninja n(camp, warriorid, warriorstrength[w]);
						n.born();
						break;
					}
					case 2: {
						iceman i(camp, warriorid, warriorstrength[w]);
						i.born();
						break;
					}
					case 3: {
						lion l(camp, warriorid, warriorstrength[w], life);
						l.born();
						break;
					}
					case 4: {
						wolf wo(camp, warriorid, warriorstrength[w]);
						wo.born();
						break;
					}
				}
				return;
			}
		}
		//鐎癸絽鎲￠崑婊勵剾閸掑爼鈧?
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
		//鏉堟挸鍙嗗В蹇曨潚濮濓箑锛嬮惃鍕晸閸涜棄鈧?
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
