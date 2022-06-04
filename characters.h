#ifndef CHARACTERS_H_
#define CHARACTERS_H_
#include <iostream>
#include <string>
#include "dungeon.h"
using namespace std;

enum Weapon {empty, sword, axe, staff};
enum Item {none, potion, bomb, watch};

class Character {
private:
	Weapon weapon;
	int hp, level, xPos, yPos;
public:
	Weapon getWeapon() {return weapon;}
	void setWeapon(Weapon w) {weapon = w;}
	int getHP() {return hp;}
	void setHP(int x) {hp = x;}
	int getLevel() {return level;}
	void setLevel(int x) {level = x;}
	void setY(int y) {
		yPos = y;
		if (yPos > 2) {yPos = 2; cout << "Y overage\n";}
		if (yPos < 0) {yPos = 0; cout << "Y under\n";}
	}
	int getY() {return yPos;}
	void setX(int x) {
		xPos = x;
		if (xPos > 3) {xPos = 3; cout << "X overage\n";}
		if (xPos < 0) {xPos = 0; cout << "X under\n";}
	}
	int getX() {return xPos;}
	
	int Attack();
	int weaponDamage(Character*);
	void updatePos(char dir);


	Character(Weapon w = empty, int h = 0, int x = 0) {
		weapon = w; hp = h; level = x; xPos = 1; yPos = 0;
		//cout << "CHAR" << endl;
	}
};

//determines weapon damage modifier
int Character::weaponDamage(Character *c) {
	if (c->getWeapon() == empty) {
		return 0;
	} else if (c->weapon == sword) {
		return 3;
	} else if (c->weapon == axe) {
		return 4;
	} else if (c->weapon == staff) {
		return 6;
	}
}

//error checking function that derived classes must implement
int Character::Attack() {
	cout << "\n?????? ERROR: INVALID OBJECT ??????\n";
}

//changes position by 1 based on character input
void Character::updatePos(char dir) {
	if (dir == 'w' || dir == 'W') {
		this->setY(this->getY() + 1);
	} else if (dir == 'a' || dir == 'A') {
		this->setX(this->getX() - 1);
	} else if (dir == 's' || dir == 'S') {
		this->setY(this->getY() - 1);
	} else if (dir == 'd' || dir == 'D') {
		this->setX(this->getX() + 1);
	}
}


class Enemy: public Character {
private:
	bool item;		//if true, gives item after death
public:
	Enemy(Weapon w, int h, int l): Character(w, h, l) {
		//cout << "ENEMY" << endl;
		this->setHP(h); this->setLevel(1);
		item = rand() % 2;
	}
	Enemy():Character() {
		item = rand() % 2;
	}

	//checks for hit, then returns attack damage for enemy multiplied by the number of game cycles the player has played (starts at 1)
	int Attack(int ng) {
		int hit = rand() % 2;
		if (hit == 0) {
			return 0;
		}
		return (rand() % 4 + 1) * this->getLevel() * ng;
	}

	bool hasItem() {
		if (item == true) { return true; }
		else { return false; }
	}
};

class Player: public Character {
private:
	string name;
	int xp, gameCount;
	Item inventory;

public:
	Player(Weapon i = empty, string n = "no name", int h = 100, int x = 10): Character(i, h, x) {
		inventory = none;
		name = n;
		xp = 20; this->setGameCount(1);
		this->setLevel(xp / 20);
		//cout << "PLAYER" << endl;
	}
	string getName() {return name;}
	Item getItem() {return inventory;}
	void setItem(Item i) {inventory = i;}
	void setXP(int x) {xp = x;}
	int getXP() {return xp;}
	void setGameCount(int x) {gameCount = x;}
	int getGameCount() {return gameCount;}



	string showItem();
	string showWeapon();
	void showLevel() {cout << "Player level: " <<  this->getLevel() << endl;}
	void checkLevel();
	void addXP(Enemy);
	int Attack();
	double weaponMult();
};
//adds XP to player character and checks for level up
void Player::addXP(Enemy bad) {
	this->setXP(this->getXP() + (5 * bad.getLevel()));
	this->checkLevel();
}

double Player::weaponMult() {
	if (this->getWeapon() == empty) {
			return 1;
		} else if (this->getWeapon() == sword) {
			return 2;
		}else if (this->getWeapon() == axe) {
			return 3;
		} else if (this->getWeapon() == staff) {
			return 4;
		}
}

//checks for hit, then returns attack damage for player
int Player::Attack() {
	int hit = rand() % 10;
	if (hit > 8) {
		cout << "\n***** MISSED *****\n";
		return 0;
	}
	return (rand() % 4 + 3) * this->weaponMult() * this->getGameCount();
}

string Player::showWeapon() {
	if (this->getWeapon() == empty) {
		return "empty";
	} else if (this->getWeapon() == sword) {
		return "sword";
	}else if (this->getWeapon() == axe) {
		return "axe";
	} else if (this->getWeapon() == staff) {
		return "staff";
	}
}

string Player::showItem() {
	if (this->getItem() == none) {
		return "none";
	} else if (this->getItem() == potion) {
		return "potion";
	} else if (this->getItem() == bomb) {
		return "bomb";
	} else if (this->getItem() == watch) {
		return "watch";
	}
}

//increases player level only if xp is high enough
void Player::checkLevel() {
	int start = this->getLevel();
	this->setLevel(xp / 20);
	if (this->getLevel() != start) {
		cout << "Level increased from " << start << " to " << this->getLevel() << "!\n";
	}
}

#endif /* CHARACTERS_H_ */
