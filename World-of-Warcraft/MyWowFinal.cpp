#include<iostream>
#include<cstring>
#include<cstdio>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;

const int RED = 1;
const int BLUE = 2;

string warriorType[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string weaponType[3] = {"sword", "bomb", "arrow" };
int redOrder[5] = { 2, 3, 4, 1, 0 };
int blueOrder[5] = { 3, 0, 1, 2, 4 };
int warriorLife[5];
int warriorAttack[5];
int initialElements;
int cityNum;
int timeLimit;
bool gameOver = false;

class weapon {
private:
	int type;
public:
	weapon(int type) : type(type) {}
	int getType() { return type; }
    virtual void use() {}
    virtual bool broken() { return false; }
    virtual ~weapon() {}
};

class sword : public weapon {
private:
    int attackBonus;
public:
    sword(int ownerAttack) : weapon(0) {
        attackBonus = ownerAttack / 5;
    }
    void use() {
        attackBonus = attackBonus * 8 / 10;
    }
    bool broken() {
        return attackBonus <= 0;
    }
    int getAttackBonus() { return attackBonus; }
};

class bomb : public weapon {
public:
    bomb() : weapon(1) {}
};

class arrow : public weapon {
private:
    int durability;
public:
    static int arrowAttack;
    arrow() : weapon(2), durability(3) {}
    int getDurability() { return durability; }
    void use() { durability--; }
    bool broken() { return durability <= 0; }
};
int arrow::arrowAttack = 0;

class weaponFactory {
public:
	static weapon* createWeapon(int type, int ownerAttack) {
		switch(type) {
			case 0:
                if(ownerAttack < 5){
                    return nullptr;
                }else{
                    return new sword(ownerAttack);
                }
			case 1: return new bomb;
			case 2: return new arrow;
			default: return nullptr;
		}
	}
};

class warrior {
protected:
    int type;
    int camp;
    int id;
    int life;
    int attack;
    vector<weapon*> weapons;
public:
    warrior(int type, int camp, int id) : type(type), camp(camp), id(id){
        life = warriorLife[type];
        attack = warriorAttack[type];
        weapons = vector<weapon*>(3, nullptr);
    }
    bool isAlive() { return life > 0; }
    bool isDead() { return life <= 0; }
    int getType() { return type; }
    int getCamp() { return camp; }
    int getId() { return id; }
    int getLife() { return life; }
    int getAttack() { return attack; }
    int getWeaponCount() {
        int count = 0;
        for(weapon* w : weapons) {
            if(w) {
                count++;
            }
        }
        return count;
    }
    void rob(warrior* target){
        for(int i = 0; i < 3; i++) {
            if(weapons[i] == nullptr && target->hasWeapon(i)) {
                weapons[i] = target->weapons[i];
                target->weapons[i] = nullptr;
            }
        }
    }
    int increaseLife(int amount) {
        life += amount;
        return life;
    }
    int getAttackBonus() {
        int bonus = 0;
        if(weapons[0]) {
            bonus += ((sword*)weapons[0])->getAttackBonus();
        }
        return bonus;
    }
    void useWeapon(int weaponType) {
        weapons[weaponType]->use();
        if(weapons[weaponType]->broken()) {
            delete weapons[weaponType];
            weapons[weaponType] = nullptr;
        }
    }
    bool willDie(int damage) {
        return life <= damage;
    }
    void decreaseLife(int amount) {
        life -= amount;
    }
    bool hasWeapon(int weaponType) {
        return weapons[weaponType] != nullptr;
    }
    virtual void born(int hour) {
        printf("%03d:00 %s %s %d born\n", 
            hour, 
            camp == RED ? "red" : "blue", warriorType[type].c_str(), 
            id);
    }
    virtual ~warrior() {
        for(weapon* w : weapons) {
            delete w;
        }
    }
    void report(int hour) {
        printf("%03d:55 %s %s %d has ", hour, camp == RED ? "red" : "blue", warriorType[type].c_str(), id);
        int weaponCount = getWeaponCount();
        int reportedCount = 0;
        if(reportedCount == weaponCount) {
            printf("no weapon\n");
        } else {
            if(hasWeapon(2)){
                arrow* arrowptr = (arrow*)weapons[2];
                printf("arrow(%d)", arrowptr->getDurability());
                reportedCount++;
                if(reportedCount == weaponCount) {
                    printf("\n");
                }else{
                    printf(",");
                }
            }
            if(hasWeapon(1)){
                printf("bomb");
                reportedCount++;
                if(reportedCount == weaponCount) {
                    printf("\n");
                }else{
                    printf(",");
                }
            }
            if(hasWeapon(0)){
                sword* swordptr = (sword*)weapons[0];
                printf("sword(%d)", swordptr->getAttackBonus());
                reportedCount++;
                if(reportedCount == weaponCount) {
                    printf("\n");
                }else{
                    printf(",");
                }
            }
        }
    }
};

class dragon : public warrior {
private:
    double morale;
public:
    dragon(int camp, int id, double morale) : warrior(0, camp, id), morale(morale) {}
    void increaseMorale() { morale += 0.2; }
    void decreaseMorale() { morale -= 0.2; }
    double getMorale() { return morale; }
    void born(int hour) {
		warrior::born(hour);
		printf("Its morale is %.2f\n", morale);
		weapons[id % 3] = weaponFactory::createWeapon(id % 3, attack);
	}
};

class ninja : public warrior {
public:
    ninja(int camp, int id) : warrior(1, camp, id) {}
    void born(int hour) {
        warrior::born(hour);
        weapons[id % 3] = weaponFactory::createWeapon(id % 3, attack);
        weapons[(id + 1) % 3] = weaponFactory::createWeapon((id + 1) % 3, attack);
    }
};

class iceman : public warrior {
private:
    int step;
public:
    iceman(int camp, int id) : warrior(2, camp, id), step(0) {}
    void move() {
		step++;
		if(step == 2){
			step = 0;
			life -= 9;
			attack += 20;
			if(life < 1) {
				life = 1;
			}
		}
	}
    void born(int hour) {
        warrior::born(hour);
        weapons[id % 3] = weaponFactory::createWeapon(id % 3, attack);
    }
};

class lion : public warrior {
private:
    int loyalty;
public:
    static int loyaltyDecrease;
    lion(int camp, int id, int loyalty) : warrior(3, camp, id), loyalty(loyalty) {}
    int getLoyalty() { return loyalty; }
    void decreaseLoyalty() { loyalty -= loyaltyDecrease; }
    void born(int hour) {
        warrior::born(hour);
        printf("Its loyalty is %d\n", loyalty);
    }
};
int lion::loyaltyDecrease = 0;

class wolf : public warrior {
public:
    wolf(int camp, int id) : warrior(4, camp, id) {}
    void born(int hour) {
        warrior::born(hour);
    }
};

class warriorFactory {
public:
	static warrior* createWarrior(int type, int camp, int id, int headquarterElements) {
		switch(type) {
			case 0: return new dragon(camp, id, double(headquarterElements) / warriorLife[0]);
			case 1: return new ninja(camp, id);
			case 2: return new iceman(camp, id);
			case 3: return new lion(camp, id, headquarterElements);
			case 4: return new wolf(camp, id);
			default: return nullptr;
		}
	}
};

class headquarter {
private:
    int camp;
    int headquarterElements;
    int warriorCount;
    int order[5];
    int produceIndex;
    warrior* enemy;
public:
    void increaseElements(int amount) {
        headquarterElements += amount;
    }
    warrior* getEnemy() { return enemy; }
    void setEnemy(warrior* w) { enemy = w; }
    headquarter(int camp) : camp(camp), headquarterElements(initialElements), warriorCount(0), produceIndex(0), enemy(nullptr) {
        if(camp == RED) {
            memcpy(order, redOrder, sizeof(int) * 5);
        } else {
            memcpy(order, blueOrder, sizeof(int) * 5);
        }
    }
    int getHeadquarterElements() { return headquarterElements; }
    void reward(warrior* winner) {
        if(headquarterElements >= 8) {
            headquarterElements -= 8;
            winner->increaseLife(8);
        }
    }
    warrior* produce(int hour) {
        int type = order[produceIndex];
        if(headquarterElements >= warriorLife[type]) {
            headquarterElements -= warriorLife[type];
            warrior* newWarrior = warriorFactory::createWarrior(type, camp, ++warriorCount, headquarterElements);
            newWarrior->born(hour);
            produceIndex = (produceIndex + 1) % 5;
            return newWarrior;
        }
        return nullptr;
    }
};

class city {
private:
    int id;
    int flag;
    int elements;
    int lastWinner;
    warrior* redWarrior;
    warrior* blueWarrior;
public:
    city(int id) : id(id), flag(0), elements(0), lastWinner(0), redWarrior(nullptr), blueWarrior(nullptr) {}
    void setRedWarrior(warrior* w) { redWarrior = w; }
    void setBlueWarrior(warrior* w) { blueWarrior = w; }
    warrior* getRedWarrior() { return redWarrior; }
    warrior* getBlueWarrior() { return blueWarrior; }
    void raiseFlag(warrior* winner, int hour) {
        if(winner == nullptr) {
            return;
        }
        if(lastWinner == winner->getCamp() && flag != winner->getCamp()) {
            flag = winner->getCamp();
            printf("%03d:40 %s flag raised in city %d\n", hour, winner->getCamp() == RED ? "red" : "blue", id);
        }
        lastWinner = winner->getCamp();
    }
    int getElements() { return elements; }
    void setElements(int amount) { elements = amount; }
    warrior* fightAndReturnWinner(int hour){
        warrior* attacker;
        warrior* defender;
        if(flag == RED || (flag == 0 && id % 2 == 1)) {
            attacker = redWarrior;
            defender = blueWarrior;
        } else {
            attacker = blueWarrior;
            defender = redWarrior;
        }
        if(attacker->isAlive() && defender->isAlive()) {
            int attackerLionLifeRewarding = 0;
            int defenderLionLifeRewarding = 0;
            if(attacker->getType() == 3) {
                lion* lionptr = (lion*)attacker;
                attackerLionLifeRewarding = lionptr->getLife();
            }
            if(defender->getType() == 3) {
                lion* lionptr = (lion*)defender;
                defenderLionLifeRewarding = lionptr->getLife();
            }
            defender->decreaseLife(attacker->getAttack() + attacker->getAttackBonus());
            if(attacker->hasWeapon(0)) {
                attacker->useWeapon(0);
            }
            printf("%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", 
                hour,
                attacker->getCamp() == RED ? "red" : "blue", 
                warriorType[attacker->getType()].c_str(), 
                attacker->getId(),
                defender->getCamp() == RED ? "red" : "blue", 
                warriorType[defender->getType()].c_str(), 
                defender->getId(),
                id,
                attacker->getLife(),
                attacker->getAttack());
            if(defender->isDead()){
                attacker->increaseLife(defenderLionLifeRewarding);
                printf("%03d:40 %s %s %d was killed in city %d\n", 
                    hour, 
                    defender->getCamp() == RED ? "red" : "blue", 
                    warriorType[defender->getType()].c_str(), 
                    defender->getId(),
                    id);
            }else if(defender->getType() != 1) {
                attacker->decreaseLife(defender->getAttack() / 2 + defender->getAttackBonus());
                if(defender->hasWeapon(0)) {
                    defender->useWeapon(0);
                }
                printf("%03d:40 %s %s %d fought back against %s %s %d in city %d\n", 
                    hour, 
                    defender->getCamp() == RED ? "red" : "blue", 
                    warriorType[defender->getType()].c_str(), 
                    defender->getId(),
                    attacker->getCamp() == RED ? "red" : "blue", 
                    warriorType[attacker->getType()].c_str(), 
                    attacker->getId(),
                    id);
                if(attacker->isDead()) {
                    defender->increaseLife(attackerLionLifeRewarding);
                    printf("%03d:40 %s %s %d was killed in city %d\n", 
                        hour, 
                        attacker->getCamp() == RED ? "red" : "blue", 
                        warriorType[attacker->getType()].c_str(), 
                        attacker->getId(),
                        id);
                }
            }
        }
        if(attacker->isDead() && defender->isDead()) {
            delete attacker;
            delete defender;
            redWarrior = nullptr;
            blueWarrior = nullptr;
        }else if(attacker->isAlive() && defender->isAlive()) {
            lastWinner = 0;
            if(attacker->getType() == 0) {
                dragon* dragonptr = (dragon*)attacker;
                dragonptr->decreaseMorale();
                if(dragonptr->getMorale() > 0.8) {
                    printf("%03d:40 %s dragon %d yelled in city %d\n", hour, attacker->getCamp() == RED ? "red" : "blue", attacker->getId(), id);
                }
            }
            if(defender->getType() == 0) {
                dragon* dragonptr = (dragon*)defender;
                dragonptr->decreaseMorale();
            }
            if(attacker->getType() == 3) {
                lion* lionptr = (lion*)attacker;
                lionptr->decreaseLoyalty();
            }
            if(defender->getType() == 3) {
                lion* lionptr = (lion*)defender;
                lionptr->decreaseLoyalty();
            }
        }else{
            warrior* winner = attacker->isAlive() ? attacker : defender;
            warrior* loser = attacker->isAlive() ? defender : attacker;
            if(winner->getType() == 0) {
                dragon* dragonptr = (dragon*)winner;
                dragonptr->increaseMorale();
            }
            if(attacker->isAlive() && attacker->getType() == 0) {
                dragon* dragonptr = (dragon*)attacker;
                if(dragonptr->getMorale() > 0.8) {
                    printf("%03d:40 %s dragon %d yelled in city %d\n", hour, attacker->getCamp() == RED ? "red" : "blue", attacker->getId(), id);
                }
            }
            if(winner->getType() == 4){
                winner->rob(loser);
            }
            if(loser->getCamp() == RED) {
                redWarrior = nullptr;
            } else {
                blueWarrior = nullptr;
            }
            delete loser;
            return winner;
        }
        return nullptr;
    }
    bool willLose(int camp) {
        warrior* attacker;
        warrior* defender;
        bool attackerLose = false;
        bool defenderLose = false;
        bool attackerIsRed = false;
        bool attackerIsBlue = false;
        if(flag == RED || (flag == 0 && id % 2 == 1)) {
            attacker = redWarrior;
            defender = blueWarrior;
            attackerIsRed = true;
        } else {
            attacker = blueWarrior;
            defender = redWarrior;
            attackerIsBlue = true;
        }
        if(defender->willDie(attacker->getAttack() + attacker->getAttackBonus())) {
            defenderLose = true;
        }else if(defender->getType() != 1 && attacker->willDie(defender->getAttack() / 2 + defender->getAttackBonus())) {
            attackerLose = true;
        }
        if(attackerIsRed && camp == RED) {
            return attackerLose;
        } else if(attackerIsBlue && camp == BLUE) {
            return attackerLose;
        } else {
            return defenderLose;
        }
    }
    void produceElements() {
        elements += 10;
    }
    void runaway(int hour) {
		if(redWarrior && redWarrior->getType() == 3) {
			lion* lionptr = (lion*)redWarrior;
			if(lionptr->getLoyalty() <= 0) {
				printf("%03d:05 red lion %d ran away\n", hour, redWarrior->getId());
				delete redWarrior;
				redWarrior = nullptr;
			}
		}
		if(blueWarrior && blueWarrior->getType() == 3) {
			lion* lionptr = (lion*)blueWarrior;
			if(lionptr->getLoyalty() <= 0) {
				printf("%03d:05 blue lion %d ran away\n", hour, blueWarrior->getId());
				delete blueWarrior;
				blueWarrior = nullptr;
			}
		}
	}
};

int main(){
    int caseNum;
    cin >> caseNum;
    for(int caseId = 1; caseId <= caseNum; caseId++) {
        printf("Case %d:\n", caseId);
        cin >> initialElements >> cityNum >> arrow::arrowAttack >> lion::loyaltyDecrease >> timeLimit;
        for(int i = 0; i < 5; i++) {
            cin >> warriorLife[i];
        }
        for(int i = 0; i < 5; i++) {
            cin >> warriorAttack[i];
        }
        headquarter* red = new headquarter(RED);
        headquarter* blue = new headquarter(BLUE);
        vector<city*> world(cityNum + 2, nullptr);
        gameOver = false;
        for(int i = 0; i < cityNum + 2; i++) {
            world[i] = new city(i);
        }
        for(int tick = 0; tick <= timeLimit; tick++) {
            if(gameOver) {
                break;
            }
            int hour = tick / 60;
            int minute = tick % 60;
            switch(minute) {
                case 0:
                    world[0]->setRedWarrior(red->produce(hour));
                    world[cityNum + 1]->setBlueWarrior(blue->produce(hour));
                    break;
                case 5:
                    for(int i = 0; i < cityNum + 2; i++) {
                        world[i]->runaway(hour);
                    }
                    break;
                case 10: {
                    vector<warrior*> nextRed(cityNum + 2, nullptr);
					vector<warrior*> nextBlue(cityNum + 2, nullptr);
					for(int i = cityNum + 1; i > 0; i--) {
						if(world[i - 1]->getRedWarrior()) {
							nextRed[i] = world[i - 1]->getRedWarrior();
						}
					}
					for(int i = 0; i < cityNum + 1; i++) {
						if(world[i + 1]->getBlueWarrior()) {
							nextBlue[i] = world[i + 1]->getBlueWarrior();
						}
					}
                    if(nextBlue[0]) {
                        world[0]->setBlueWarrior(nextBlue[0]);
                        if(nextBlue[0]->getType() == 2) {
							iceman* icemanptr = (iceman*)nextBlue[0];
							icemanptr->move();
						}
                        printf("%03d:10 blue %s %d reached red headquarter with %d elements and force %d\n", 
							hour, 
							warriorType[nextBlue[0]->getType()].c_str(), 
							nextBlue[0]->getId(), 
							nextBlue[0]->getLife(), 
							nextBlue[0]->getAttack());
                        if(red->getEnemy() == nullptr) {
                            red->setEnemy(nextBlue[0]);
                        } else {
                            printf("%03d:10 red headquarter was taken\n", hour);
                            gameOver = true;
                        }
                    }
                    for(int i = 1; i < cityNum + 1; i++) {
                        world[i]->setRedWarrior(nextRed[i]);
                        world[i]->setBlueWarrior(nextBlue[i]);
                        if(nextRed[i]) {
                            if (nextRed[i]->getType() == 2) {
								iceman* icemanptr = (iceman*)nextRed[i];
								icemanptr->move();
							}
                            printf("%03d:10 red %s %d marched to city %d with %d elements and force %d\n",
                                hour, 
                                warriorType[nextRed[i]->getType()].c_str(), 
                                nextRed[i]->getId(), 
                                i,
                                nextRed[i]->getLife(),
                                nextRed[i]->getAttack());
                        }
                        if(nextBlue[i]) {
                            if (nextBlue[i]->getType() == 2) {
								iceman* icemanptr = (iceman*)nextBlue[i];
								icemanptr->move();
							}
                            printf("%03d:10 blue %s %d marched to city %d with %d elements and force %d\n",
                                hour, 
                                warriorType[nextBlue[i]->getType()].c_str(), 
                                nextBlue[i]->getId(), 
                                i,
                                nextBlue[i]->getLife(),
                                nextBlue[i]->getAttack());
                        }
                    }
                    if(nextRed[cityNum + 1]) {
                        world[cityNum + 1]->setRedWarrior(nextRed[cityNum + 1]);
                        if(nextRed[cityNum + 1]->getType() == 2) {
							iceman* icemanptr = (iceman*)nextRed[cityNum + 1];
							icemanptr->move();
						}
                        printf("%03d:10 red %s %d reached blue headquarter with %d elements and force %d\n", 
                            hour,
                            warriorType[nextRed[cityNum + 1]->getType()].c_str(),
                            nextRed[cityNum + 1]->getId(),
                            nextRed[cityNum + 1]->getLife(),
                            nextRed[cityNum + 1]->getAttack());
                        if(blue->getEnemy() == nullptr) {
                            blue->setEnemy(nextRed[cityNum + 1]);
                        } else {
                            printf("%03d:10 blue headquarter was taken\n", hour);
                            gameOver = true;
                        }
                    }
                    break;
                }
                case 20:
                    for(int i = 1; i <= cityNum; i++) {
                        world[i]->produceElements();
                    }
                    break;
                case 30:
                    for(int i = 1; i <= cityNum; i++) {
                        warrior* redWarrior = world[i]->getRedWarrior();
                        warrior* blueWarrior = world[i]->getBlueWarrior();
                        if(redWarrior && !blueWarrior) {
                            printf("%03d:30 red %s %d earned %d elements for his headquarter\n", 
                                hour, 
                                warriorType[redWarrior->getType()].c_str(), 
                                redWarrior->getId(),
                                world[i]->getElements());
                            red->increaseElements(world[i]->getElements());
                            world[i]->setElements(0);
                        } else if(blueWarrior && !redWarrior) {
                            printf("%03d:30 blue %s %d earned %d elements for his headquarter\n", 
                                hour, 
                                warriorType[blueWarrior->getType()].c_str(), 
                                blueWarrior->getId(),
                                world[i]->getElements());
                            blue->increaseElements(world[i]->getElements());
                            world[i]->setElements(0);
                        }
                    }
                    break;
                case 35:
                    for(int i = 1; i <= cityNum; i++) {
                        warrior* redWarrior = world[i]->getRedWarrior();
                        warrior* blueWarrior = world[i]->getBlueWarrior();
                        if(i < cityNum && redWarrior && world[i + 1]->getBlueWarrior() && redWarrior->hasWeapon(2)) {
                            redWarrior->useWeapon(2);
                            warrior* target = world[i + 1]->getBlueWarrior();
                            target->decreaseLife(arrow::arrowAttack);
                            if(target->isDead()) {
                                printf("%03d:35 red %s %d shot and killed blue %s %d\n", 
                                    hour, 
                                    warriorType[redWarrior->getType()].c_str(), 
                                    redWarrior->getId(),
                                    warriorType[target->getType()].c_str(), 
                                    target->getId());
                            } else {
                                printf("%03d:35 red %s %d shot\n", 
                                    hour, 
                                    warriorType[redWarrior->getType()].c_str(), 
                                    redWarrior->getId());
                            }
                        }
                        if(i > 1 && blueWarrior && world[i - 1]->getRedWarrior() && blueWarrior->hasWeapon(2)) {
                            blueWarrior->useWeapon(2);
                            warrior* target = world[i - 1]->getRedWarrior();
                            target->decreaseLife(arrow::arrowAttack);
                            if(target->isDead()) {
                                printf("%03d:35 blue %s %d shot and killed red %s %d\n", 
                                    hour, 
                                    warriorType[blueWarrior->getType()].c_str(), 
                                    blueWarrior->getId(),
                                    warriorType[target->getType()].c_str(), 
                                    target->getId());
                            } else {
                                printf("%03d:35 blue %s %d shot\n", 
                                    hour, 
                                    warriorType[blueWarrior->getType()].c_str(), 
                                    blueWarrior->getId());
                            }
                        }
                    }
                    break;
                case 38:{
                    for(int i = 1; i <= cityNum; i++) {
                        warrior* redWarrior = world[i]->getRedWarrior();
                        warrior* blueWarrior = world[i]->getBlueWarrior();
                        bool bombUsed = false;
                        if(redWarrior && blueWarrior) {
                            if(redWarrior->isDead() || blueWarrior->isDead()) {
                                continue;
                            }
                            if(redWarrior->hasWeapon(1) && world[i]->willLose(RED)) {
                                printf("%03d:38 red %s %d used a bomb and killed blue %s %d\n", 
                                    hour, 
                                    warriorType[redWarrior->getType()].c_str(), 
                                    redWarrior->getId(),
                                    warriorType[blueWarrior->getType()].c_str(), 
                                    blueWarrior->getId());
                                bombUsed = true;
                            }
                            if(blueWarrior->hasWeapon(1) && world[i]->willLose(BLUE)) {
                                printf("%03d:38 blue %s %d used a bomb and killed red %s %d\n", 
                                    hour, 
                                    warriorType[blueWarrior->getType()].c_str(), 
                                    blueWarrior->getId(),
                                    warriorType[redWarrior->getType()].c_str(), 
                                    redWarrior->getId());
                                bombUsed = true;
                            }
                        }
                        if(bombUsed) {
                            delete redWarrior;
                            world[i]->setRedWarrior(nullptr);
                            delete blueWarrior;
                            world[i]->setBlueWarrior(nullptr);
                        }
                    }
                    break;
                }
                case 40:{
                    vector<warrior*> winners(cityNum + 2, nullptr);
                    vector<int> redWillEarn(cityNum + 2, 0);
                    vector<int> blueWillEarn(cityNum + 2, 0);
                    for(int i = 1; i <= cityNum; i++){
                        warrior* redWarrior = world[i]->getRedWarrior();
                        warrior* blueWarrior = world[i]->getBlueWarrior();
                        if(redWarrior && blueWarrior) {
                            winners[i] = world[i]->fightAndReturnWinner(hour);
                            if(winners[i]) {
                                printf("%03d:40 %s %s %d earned %d elements for his headquarter\n", 
                                    hour, 
                                    winners[i]->getCamp() == RED ? "red" : "blue", 
                                    warriorType[winners[i]->getType()].c_str(), 
                                    winners[i]->getId(),
                                    world[i]->getElements());
                                if(winners[i]->getCamp() == RED) {
                                    redWillEarn[i] = world[i]->getElements();
                                } else {
                                    blueWillEarn[i] = world[i]->getElements();
                                }
                                world[i]->setElements(0);
                            }
                        }else{
                            if(redWarrior && redWarrior->isDead()) {
                                delete redWarrior;
                                world[i]->setRedWarrior(nullptr);
                            }
                            if(blueWarrior && blueWarrior->isDead()) {
                                delete blueWarrior;
                                world[i]->setBlueWarrior(nullptr);
                            }
                        }
                        world[i]->raiseFlag(winners[i], hour);
                    }
                    for(int i = cityNum; i >= 1; i--) {
                        if(winners[i] && winners[i]->getCamp() == RED) {
                            red->reward(winners[i]);
                        }
                    }
                    for(int i = 1; i <= cityNum; i++) {
                        if(winners[i] && winners[i]->getCamp() == BLUE) {
                            blue->reward(winners[i]);
                        }
                    }
                    for(int i = 1; i <= cityNum; i++) {
                        if(redWillEarn[i]) {
                            red->increaseElements(redWillEarn[i]);
                        }
                        if(blueWillEarn[i]) {
                            blue->increaseElements(blueWillEarn[i]);
                        }
                    }
                    break;
                }
                case 50:
                    printf("%03d:50 %d elements in red headquarter\n", hour, red->getHeadquarterElements());
                    printf("%03d:50 %d elements in blue headquarter\n", hour, blue->getHeadquarterElements());
                    break;
                case 55:
                    for(int i = 1; i <= cityNum; i++) {
                        if(world[i]->getRedWarrior()) {
                            world[i]->getRedWarrior()->report(hour);
                        }
                    }
                    if(blue->getEnemy()) {
                        blue->getEnemy()->report(hour);
                    }
                    if(red->getEnemy()) {
                        red->getEnemy()->report(hour);
                    }
                    for(int i = 1; i <= cityNum; i++) {
                        if(world[i]->getBlueWarrior()) {
                            world[i]->getBlueWarrior()->report(hour);
                        }
                    }
                    break;
            }
        }
    }
    return 0;
}