#include<iostream>
#include<cstring>
#include<cstdio>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;

const int RED = 0;
const int BLUE = 1;

string warriortype[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string weapontype[3] = {"sword", "bomb", "arrow" };
int redorder[5] = { 2, 3, 4, 1, 0 };
int blueorder[5] = { 3, 0, 1, 2, 4 };
int warriorstrength[5];
int warriorattack[5];
int caseNum;
int life;
int citynum;

int gametick = 0;

class warrior;

class weapon {
private:
	int type;
protected:
	int durability;
public:
	weapon(int t) : type(t) {
		switch(type) {
			case 0: durability = -1; break; // -1 means infinite durability
			case 1: durability = 1; break;
			case 2: durability = 2; break;
		}
	}
	int gettype() {
		return type;
	}
	int getdurability() {
		return durability;
	}
	virtual int attack(warrior* owner) {
		return 0;
	}
};

class warrior {
private:
	int type;
	int camp;
	int id;
	int strength;
	int attack;
	vector<weapon*> weapons;
	static bool weaponcompare(weapon* a, weapon* b) {
		if(a->gettype() != b->gettype()) {
			return a->gettype() < b->gettype();
		}
		return a->getdurability() < b->getdurability();
	}
public:
	warrior(int t, int c, int i) : type(t), camp(c), id(i){
		strength = warriorstrength[type];
		attack = warriorattack[type];
	}
	void weaponSort() {
		sort(weapons.begin(), weapons.end(), weaponcompare);
	}
	void decreaseStrength(int s) {
		strength -= s;
		if(strength < 0) {
			strength = 0;
		}
	}
	int getattack() {
		return attack;
	}
	int gettype() {
		return type;
	}
	int getid() {
		return id;
	}
	int getCamp() {
		return camp;
	}
	int getStrength() {
		return strength;
	}
	int getAttack() {
		return attack;
	}
	bool isdead() {
		return strength <= 0;
	}
	bool weaponCannotUse() {
		if(weapons.empty()) {
			return true;
		}
		for(weapon* w : weapons) {
			if(w->gettype() > 0 || w->attack(this) > 0) {
				return false;
			}
		}
		return true;
	}
	void report() {
		printf("%03d:55 %s %s %d has %d sword %d bomb %d arrow and %d elements\n", 
			gametick/60, 
			camp == RED ? "red" : "blue", 
			warriortype[type].c_str(), 
			id, 
			count_if(weapons.begin(), weapons.end(), [](weapon* w){ return w->gettype() == 0; }), 
			count_if(weapons.begin(), weapons.end(), [](weapon* w){ return w->gettype() == 1; }), 
			count_if(weapons.begin(), weapons.end(), [](weapon* w){ return w->gettype() == 2; }), 
			strength);
	}
	virtual void born() {
		printf("%03d:00 %s %s %d born\n", gametick/60, camp == RED ? "red" : "blue", warriortype[type].c_str(), id);
	}
	friend class dragon;
	friend class ninja;
	friend class iceman;
	friend class lion;
	friend class wolf;
	friend class city;
	virtual ~warrior() {
		for(weapon* w : weapons) {
			delete w;
		}
	}
};

class sword : public weapon {
public:
	sword() : weapon(0) {}
	int attack(warrior* owner) {
		durability--;
		return owner->getattack() / 5;
	}
};

class bomb : public weapon {
public:
	bomb() : weapon(1) {}
	int attack(warrior* owner) {
		durability--;
		int damage = owner->getattack() * 2 / 5;
		if(owner->gettype() != 1) {
			owner->decreaseStrength(damage / 2); // bomb hurts the owner
		}
		return damage;
	}
};

class arrow : public weapon {
public:
	arrow() : weapon(2) {}
	int attack(warrior* owner) {
		durability--;
		return owner->getattack() * 3 / 10;
	}
	void setDurability(int d) {
		durability = d;
	}
};

class weaponFactory {
public:
	static weapon* createWeapon(int type) {
		switch(type) {
			case 0: return new sword;
			case 1: return new bomb;
			case 2: return new arrow;
			default: return nullptr;
		}
	}
};

class dragon : public warrior {
private:
public:
	void born() {
		warrior::born();
		weapons.push_back(weaponFactory::createWeapon(id % 3));
	}
	dragon(int c, int i) : warrior(0, c, i) {}
};

class ninja : public warrior {
private:
public:
	void born() {
		warrior::born();
		weapons.push_back(weaponFactory::createWeapon(id % 3));
		weapons.push_back(weaponFactory::createWeapon((id + 1) % 3));
	}
	ninja(int c, int i) : warrior(1, c, i) {}
};

class iceman : public warrior {
private:
public:
	void born() {
		warrior::born();
		weapons.push_back(weaponFactory::createWeapon(id % 3));
	}
	iceman(int c, int i) : warrior(2, c, i) {}
};

class lion : public warrior {
private:
	int loyalty;
public:
	static int loyaltydec;
	void decreaseLoyalty() {
		loyalty -= loyaltydec;
	}
	int getLoyalty() {
		return loyalty;
	}
	void born() {
		warrior::born();
		weapons.push_back(weaponFactory::createWeapon(id % 3));
		printf("Its loyalty is %d\n", loyalty);
	}
	lion(int c, int i, int l) : warrior(3, c, i), loyalty(l) {}
};
int lion::loyaltydec = 0;

class wolf : public warrior {
private:
public:
	void born() {
		warrior::born();
	}
	void rob(warrior* target, int cityId) {
		if(target->gettype() == 4) {
			return; // wolf won't rob another wolf
		}
		if(target->weapons.empty()) {
			return; // target has no weapon to rob
		}
		target->weaponSort();
		int robType = target->weapons[0]->gettype();
		int robNum = 0;
		for(int i = target->weapons.size() - 1; i >= 0; i--) {
			if(weapons.size() >= 10) {
				break; // wolf can carry at most 10 weapons
			}
			if(target->weapons[i]->gettype() == robType) {
				weapons.push_back(target->weapons[i]);
				target->weapons.erase(target->weapons.begin() + i);
				robNum++;
			}
		}
		if(robNum > 0) {
			string wolfCamp = getCamp() == RED ? "red" : "blue";
			printf("%03d:35 %s wolf %d took %d %s from %s %s %d in city %d\n", 
				gametick/60, 
				wolfCamp.c_str(), 
				getid(), 
				robNum, 
				weapontype[robType].c_str(), 
				target->getCamp() == RED ? "red" : "blue", 
				warriortype[target->gettype()].c_str(), 
				target->getid(), 
				cityId);
		}
		return;
	}
	wolf(int c, int i) : warrior(4, c, i) {}
};

//浼犲叆鍙傛暟锛歵ype锛堟澹被鍨嬶級锛宑amp锛堥樀钀ワ級锛宨d锛堢紪鍙凤級锛宭ife锛堝墿浣欑敓鍛藉€硷級 杩斿洖涓€涓猈arrior鎸囬拡
class warriorFactory {
public:
	static warrior* createWarrior(int type, int camp, int id, int life) {
		switch(type) {
			case 0: return new dragon(camp, id);
			case 1: return new ninja(camp, id);
			case 2: return new iceman(camp, id);
			case 3: return new lion(camp, id, life);
			case 4: return new wolf(camp, id);
			default: return nullptr;
		}
	}
};

class city {
private:
	int id;
	warrior* redWarrior;
	warrior* blueWarrior;
	static bool weaponCompareWhenRob(weapon* a, weapon* b) {
		if(a->gettype() != b->gettype()) {
			return a->gettype() < b->gettype();
		}
		return a->getdurability() > b->getdurability(); // if same type, prefer higher durability
	}
public:
	city(int i) : id(i), redWarrior(nullptr), blueWarrior(nullptr) {}
	void fight() {
		if(redWarrior && blueWarrior) {
			redWarrior->weaponSort();
			blueWarrior->weaponSort();
			int turn;
			if(id % 2 == 1) {
				turn = RED;
			} else {
				turn = BLUE;
			}
			int index[2] = {0, 0}; // Index[0] for red warrior's weapon index, Index[1] for blue warrior's weapon index
			int round = 0;
			while(true) {
				round++;
				if(round > 500) {
					break;
				}
				warrior* attacker;
				warrior* defender;
				if(turn == RED) {
					attacker = redWarrior;
					defender = blueWarrior;
				}else{
					attacker = blueWarrior;
					defender = redWarrior;
				}
				if(attacker->weaponCannotUse() && defender->weaponCannotUse()) {
					break;
				}
				if(attacker->weaponCannotUse() == false) {
					index[turn] = index[turn] % attacker->weapons.size();
					int damage = attacker->weapons[index[turn]]->attack(attacker);
					defender->decreaseStrength(damage);
					if(attacker->weapons[index[turn]]->getdurability() == 0) {
						delete attacker->weapons[index[turn]];
						attacker->weapons.erase(attacker->weapons.begin() + index[turn]);
					} else {
						index[turn] ++;
					}
					if(defender->isdead()) {
						if(attacker->isdead()) {
							printf("%03d:40 both red %s %d and blue %s %d died in city %d\n", 
								gametick/60, 
								warriortype[redWarrior->gettype()].c_str(), 
								redWarrior->getid(), 
								warriortype[blueWarrior->gettype()].c_str(), 
								blueWarrior->getid(), 
								id);
							delete redWarrior;
							delete blueWarrior;
							redWarrior = nullptr;
							blueWarrior = nullptr;
						} else {
							printf("%03d:40 %s %s %d killed %s %s %d in city %d remaining %d elements\n", 
								gametick/60, 
								attacker->getCamp() == RED ? "red" : "blue", 
								warriortype[attacker->gettype()].c_str(), 
								attacker->getid(), 
								defender->getCamp() == RED ? "red" : "blue", 
								warriortype[defender->gettype()].c_str(), 
								defender->getid(), 
								id, 
								attacker->getStrength());
							if(attacker->gettype() == 0) { // if the winner is a dragon, it will yell
								printf("%03d:40 %s dragon %d yelled in city %d\n", 
									gametick/60, 
									attacker->getCamp() == RED ? "red" : "blue", 
									attacker->getid(), 
									id);
							}
							sort(defender->weapons.begin(), defender->weapons.end(), weaponCompareWhenRob);
							for(int i = 0; i < defender->weapons.size(); i++) {
								if(attacker->weapons.size() >= 10) {
									break; // warrior can carry at most 10 weapons
								}
								weapon* newWeapon = weaponFactory::createWeapon(defender->weapons[i]->gettype());
								if(defender->weapons[i]->gettype() == 2 && defender->weapons[i]->getdurability() == 1) {
									((arrow*)newWeapon)->setDurability(1); // if the arrow can only be used once, the new arrow can also only be used once
								}
								attacker->weapons.push_back(newWeapon);
							}
							delete defender;
							if(turn == RED) {
								blueWarrior = nullptr;
							} else {
								redWarrior = nullptr;
							}
						}
						return;
					} else {
						if(attacker->isdead()) {
							printf("%03d:40 %s %s %d killed %s %s %d in city %d remaining %d elements\n", 
								gametick/60, 
								defender->getCamp() == RED ? "red" : "blue", 
								warriortype[defender->gettype()].c_str(), 
								defender->getid(), 
								attacker->getCamp() == RED ? "red" : "blue", 
								warriortype[attacker->gettype()].c_str(), 
								attacker->getid(), 
								id, 
								defender->getStrength());
							if(defender->gettype() == 0) { // if the winner is a dragon, it will yell
								printf("%03d:40 %s dragon %d yelled in city %d\n", 
									gametick/60, 
									defender->getCamp() == RED ? "red" : "blue", 
									defender->getid(), 
									id);
							}
							if(attacker->weapons.size() > 0) {
								sort(attacker->weapons.begin(), attacker->weapons.end(), weaponCompareWhenRob);
								for(int i = 0; i < attacker->weapons.size(); i++) {
									if(defender->weapons.size() >= 10) {
										break; // warrior can carry at most 10 weapons
									}
									weapon* newWeapon = weaponFactory::createWeapon(attacker->weapons[i]->gettype());
									if(attacker->weapons[i]->gettype() == 2 && attacker->weapons[i]->getdurability() == 1) {
										((arrow*)newWeapon)->setDurability(1); // if the arrow can only be used once, the new arrow can also only be used once
									}
									defender->weapons.push_back(newWeapon);
								}
							}
							delete attacker;
							if(turn == RED) {
								redWarrior = nullptr;
							} else {
								blueWarrior = nullptr;
							}
							return;
						}
					}
				}
				turn = 1 - turn;
			}
			printf("%03d:40 both red %s %d and blue %s %d were alive in city %d\n", 
				gametick/60, 
				warriortype[redWarrior->gettype()].c_str(), 
				redWarrior->getid(), 
				warriortype[blueWarrior->gettype()].c_str(), 
				blueWarrior->getid(), 
				id);
			if(redWarrior->gettype() == 0) { // if red warrior is a dragon, it will yell
				printf("%03d:40 red dragon %d yelled in city %d\n", 
					gametick/60, 
					redWarrior->getid(), 
					id);
			}
			if(blueWarrior->gettype() == 0) { // if blue warrior is a dragon, it will yell
				printf("%03d:40 blue dragon %d yelled in city %d\n", 
					gametick/60, 
					blueWarrior->getid(), 
					id);
			}
			return;
		}
	}
	void runaway() {
		if(redWarrior && redWarrior->gettype() == 3) {
			lion* lionptr = (lion*)redWarrior;
			if(lionptr->getLoyalty() <= 0) {
				printf("%03d:05 red lion %d ran away\n", gametick/60, redWarrior->getid());
				delete redWarrior;
				redWarrior = nullptr;
			}
		}
		if(blueWarrior && blueWarrior->gettype() == 3) {
			lion* lionptr = (lion*)blueWarrior;
			if(lionptr->getLoyalty() <= 0) {
				printf("%03d:05 blue lion %d ran away\n", gametick/60, blueWarrior->getid());
				delete blueWarrior;
				blueWarrior = nullptr;
			}
		}
	}
	void setRedWarrior(warrior* w) {
		redWarrior = w;
	}
	void setBlueWarrior(warrior* w) {
		blueWarrior = w;
	}
	warrior* getRedWarrior() {
		return redWarrior;
	}
	warrior* getBlueWarrior() {
		return blueWarrior;
	}
};


class Headquarter {
private:
	int camp;
	int life;
	int warriorid;
	int warriororder[5];
	int index;
	bool stop;
public:
	Headquarter(int c, int l, int* order) : camp(c), life(l), warriorid(0), index(0), stop(false) {
		memcpy(warriororder, order, sizeof(int) * 5);
	}
	void produce(city& targetCity) {
		if(stop) {
			return;
		}
		int type = warriororder[index];
		index = (index + 1) % 5;
		if(life >= warriorstrength[type]) {
			life -= warriorstrength[type];
			warriorid++;
			warrior* w = warriorFactory::createWarrior(type, camp, warriorid, life);
			if(camp == RED){
				targetCity.setRedWarrior(w);
			} else {
				targetCity.setBlueWarrior(w);
			}
			w->born();
		} else {
			stop = true;
		}
	}
	void report() {
		printf("%03d:50 %d elements in %s headquarter\n", gametick/60, life, camp == RED ? "red" : "blue");
	}
	friend class game;
};

class game {
private:
	Headquarter* red;
	Headquarter* blue;
	vector<city> cities;
	int tick;
	bool isOver;
public:
	game() : red(nullptr), blue(nullptr), tick(0), isOver(false) {}
	void start() {
		cin >> life >> citynum >> lion::loyaltydec >> tick;
		for(int i = 0; i < 5; i++) {
			cin >> warriorstrength[i];
		}
		for(int i = 0; i < 5; i++) {
			cin >> warriorattack[i];
		}
		red = new Headquarter(RED, life, redorder);
		blue = new Headquarter(BLUE, life, blueorder);
		for(int i = 0; i < citynum + 2; i++) {
			cities.push_back(city(i)); // city 0 and city citynum + 1 are the red and blue headquarters respectively
		}
	}
	void play() {
		for(gametick = 0; gametick <= tick; gametick += 5) {
			if(isOver) {
				break;
			}
			int minute = gametick % 60;
			switch(minute) {
				case 0: {
					red->produce(cities[0]);
					blue->produce(cities[citynum + 1]);
					break;
				}
				case 5: {
					// lion runs away
					for(int i = 0; i < citynum + 2; i++) {
						cities[i].runaway();
					}
					break;
				}
				case 10: {
					vector<warrior*> nextRed(citynum + 2, nullptr);
					vector<warrior*> nextBlue(citynum + 2, nullptr);
					for(int i = citynum + 1; i > 0; i--) {
						if(cities[i - 1].getRedWarrior()) {
							nextRed[i] = cities[i - 1].getRedWarrior();
							cities[i - 1].setRedWarrior(nullptr);
						}
					}
					for(int i = 0; i < citynum + 1; i++) {
						if(cities[i + 1].getBlueWarrior()) {
							nextBlue[i] = cities[i + 1].getBlueWarrior();
							cities[i + 1].setBlueWarrior(nullptr);
						}
					}
					if(nextBlue[0]) {
						cities[0].setBlueWarrior(nextBlue[0]);
						if(nextBlue[0]->gettype() == 2) { // iceman moves
							iceman* icemanptr = (iceman*)nextBlue[0];
							icemanptr->decreaseStrength(icemanptr->getStrength() / 10);
						}
						printf("%03d:10 blue %s %d reached red headquarter with %d elements and force %d\n", 
							gametick/60, 
							warriortype[nextBlue[0]->gettype()].c_str(), 
							nextBlue[0]->getid(), 
							nextBlue[0]->getStrength(), 
							nextBlue[0]->getAttack());
						printf("%03d:10 red headquarter was taken\n", gametick/60);
						isOver = true;
					}
					for(int i = 1; i < citynum + 1; i++) {
						if(nextRed[i]) {
							if (nextRed[i]->gettype() == 2) { // iceman moves
								iceman* icemanptr = (iceman*)nextRed[i];
								icemanptr->decreaseStrength(icemanptr->getStrength() / 10);
							}
							if (nextRed[i]->gettype() == 3) { // lion moves
								lion* lionptr = (lion*)nextRed[i];
								lionptr->decreaseLoyalty();
							}
							cities[i].setRedWarrior(nextRed[i]);
							printf("%03d:10 red %s %d marched to city %d ", gametick/60, warriortype[nextRed[i]->gettype()].c_str(), nextRed[i]->getid(), i);
							printf("with %d elements and force %d\n", nextRed[i]->getStrength(), nextRed[i]->getAttack());
						}
						if(nextBlue[i]) {
							if (nextBlue[i]->gettype() == 2) { // iceman moves
								iceman* icemanptr = (iceman*)nextBlue[i];
								icemanptr->decreaseStrength(icemanptr->getStrength() / 10);
							}
							if (nextBlue[i]->gettype() == 3) { // lion moves
								lion* lionptr = (lion*)nextBlue[i];
								lionptr->decreaseLoyalty();
							}
							cities[i].setBlueWarrior(nextBlue[i]);
							printf("%03d:10 blue %s %d marched to city %d ", gametick/60, warriortype[nextBlue[i]->gettype()].c_str(), nextBlue[i]->getid(), i);
							printf("with %d elements and force %d\n", nextBlue[i]->getStrength(), nextBlue[i]->getAttack());
						}
					}
					if(nextRed[citynum + 1]) {
						cities[citynum + 1].setRedWarrior(nextRed[citynum + 1]);
						if(nextRed[citynum + 1]->gettype() == 2) { // iceman moves
							iceman* icemanptr = (iceman*)nextRed[citynum + 1];
							icemanptr->decreaseStrength(icemanptr->getStrength() / 10);
						}
						printf("%03d:10 red %s %d reached blue headquarter with %d elements and force %d\n", 
							gametick/60, 
							warriortype[nextRed[citynum + 1]->gettype()].c_str(), 
							nextRed[citynum + 1]->getid(), 
							nextRed[citynum + 1]->getStrength(), 
							nextRed[citynum + 1]->getAttack());
						printf("%03d:10 blue headquarter was taken\n", gametick/60);
						isOver = true;
					}
					break;
				}
				case 35: {
					for(int i = 1; i <= citynum; i++) {
						warrior* redWarrior = cities[i].getRedWarrior();
						warrior* blueWarrior = cities[i].getBlueWarrior();
						if(redWarrior && blueWarrior) {
							if(redWarrior->gettype() == 4) {
								((wolf*)redWarrior)->rob(blueWarrior, i);
							}
							if(blueWarrior->gettype() == 4) {
								((wolf*)blueWarrior)->rob(redWarrior, i);
							}
						}
					}
					break;
				}
				case 40: {
					for (int i = 1; i <= citynum; i++) {
						cities[i].fight();
					}
					break;
				}
				case 50: {
					red->report();
					blue->report();
					break;
				}
				case 55: {
					for(int i = 1; i <= citynum; i++) {
						if(cities[i].getRedWarrior()) {
							cities[i].getRedWarrior()->report();
						}
						if(cities[i].getBlueWarrior()) {
							cities[i].getBlueWarrior()->report();
						}
					}
					break;
				}
			}
		}
	}
};

int main() {
	cin >> caseNum;
	for (int i = 1; i <= caseNum; i++) {
		printf("Case %d:\n", i);
		game newGame;
		newGame.start();
		newGame.play();
	}
	return 0;
}
