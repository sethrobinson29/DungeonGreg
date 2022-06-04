#include <iostream>
#include <fstream>
#include "characters.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void pause() {
	char c;
	cout << "\nPress any key to continue...\n";
	cin >> c;
}

/*
 * Returns true if player can leave encounter (33% chance)
 * If the player fails, the enemy is given a free swing on the player
 */
bool runAway(Player p, Enemy bad) {
	cout << "ESCAPE\n";
	if (rand() % 3 == 1) {
		cout << "Successfully ran away!\n";
		return true;
	} else {
		cout << "Enemy attacks you!\n";
		p.setHP(p.getHP() - bad.Attack(p.getGameCount()));
		cout << "Player: " << p.getHP() << endl << "Enemy: " << bad.getHP() << endl;
		return false;
	}
}


//checks if enemy is occupying space (33% chance)
bool hasEnemy() {
	if (rand() % 3 == 2) {
		return true;
	} else {
		return false;
	}
}

//returns true if enemy is dead
bool checkEnemyDead(Enemy e) {
	if (e.getHP() <= 0) {
		cout << "Enemy defeated!\n";
		//give player xp for defeated enemy and increase killCount
		return true;
	} else {
		return false;
	}
}

//returns an item for player use
Item makeItem() {
	int n = rand() % 3;
	if (n == 0) {
		return potion;
	} else if (n == 1) {
		return bomb;
	} else if (n == 2) {
		return watch;
	}
	cout << "Error\n";
	return none;
}

//makes player character
Player makePlayer() {
	string name; int hp = 50; int xp = 15; int choice; Weapon i;
	cout << "Enter player name: "; cin >> name;
	cout << "Choose weapon:\n1. Sword\n2. Axe\n3. Staff\n4. empty\n"; cin >> choice;

	switch(choice){
	case 1:
		i = sword;
		break;
	case 2:
		i = axe;
		break;
	case 3:
		i = staff;
		break;
	case 4:
		i = empty;
		break;
	}
	Player p(i, name, hp, xp);

	return p;
}

/*
 * generates enemies based on how deep into the dungeon the player has gone & how many game cycles the player has completed (starts at 1).
 * Enemy constructor: Enemy(weapon, health, level)
 */
Enemy* spawnEnemy(int depth, int gameCount) {
	Enemy *bad;
	/*
	 * if statements used to increase or decrease enemy level by manually increasing hp
	 */
	if (depth <= 2) {
		bad = new Enemy(empty, 30, 1*gameCount);
		cout << "************************\n";
		cout << "Enemy has arrived!\n\n";

	} else if (depth > 2 && depth < 5) {
		bad = new Enemy(sword, 35, 2*gameCount);
		cout << "************************\n";
		cout << "Enemy has arrived!\n\n";

	} else if (depth >= 5 && depth < 9) {
		bad = new Enemy(axe, 40, 3*gameCount);
		cout << "************************\n";
		cout << "Enemy has arrived!\n\n";
	} else if (depth == 9) {
		bad = new Enemy(staff, 45, 4*gameCount);
	} else {																									//end of the game
		bad = new Enemy(staff, 999, 666);
		for (int i = 0; i < 240; i++) {
			string str = "END";
			if (i % 10 == 0) {
				str = "\n";
			}
			cout << str;
		}
	}

	return bad;
}

//actual game, returns user for ending/newgame+
Player enterDungeon(Dungeon d, Player p) {
	int killCount = 0; int roomCount = 1;
	Room *r = d.rooms;
	Graphics map(p, d, r);

	//loop that displays room
	while (roomCount <= d.getSize() ) {
		bool door = false;
		int enemyCount = r->getEnemyCount();
		p.setX(1); p.setY(0);
		cout << "||-----------------------------------------------------||\n";
		cout << "||-----------------------------------------------------||\n";
		cout << "||                        Room " << roomCount << "                       ||\n\n";
		cout << "Total enemies: " << enemyCount << " || Door X: " << r->getDoorX() << " Door Y: " << r->getDoorY() << endl;
		cout << p.getName() << " || " << p.showWeapon() << endl;
		p.showLevel(); cout << "Player X: " << p.getX() << " Player Y: " << p.getY() << endl;

		//movement loop
		while (!door) {
			map.printMap(p); cout << "Use WASD to move...\n";
			char choice, move;
			bool encounter = hasEnemy();
			bool con = true;
			cin >> move;

			//if no enemies, just move
			if (enemyCount <= 0) {encounter= false;}

			//roll for enemy
			if (encounter) {
				Enemy *badGuy = spawnEnemy(roomCount, p.getGameCount());
				if (badGuy->hasItem() == true) {
					cout << "HAS ITEM\n";
				}
				if (enemyCount <= 0) {con = false;}

				//combat loop
				while (con == true ) {
					while (badGuy->getHP() > 0 && p.getHP() > 0) {
						cout << "Player: " << p.getHP() << endl << "Enemy: " << badGuy->getHP() << endl;
						cout << "************************\n";
						cout << "A - attack | L - escape | I - use item\n\n";
						cin >> choice;
						//attack
						if (choice == 'a' || choice == 'A') {
							//change hp to hp minus damage dealt;
							int pDam = p.Attack();
							int bDam = badGuy->Attack(p.getGameCount());
							badGuy->setHP(badGuy->getHP() - pDam);
							p.setHP(p.getHP() - bDam);
							cout << "Damage dealt: " << pDam << "\nDamage taken: " << bDam << endl;
							cout << "************************\n";
							cout << "************************\n";
							//break loop if enemy dies
							if (checkEnemyDead(*badGuy)) {
								con = false;
								enemyCount--;
								p.updatePos(move);
								p.addXP(*badGuy);
								//give item to player
								if (badGuy->hasItem() == true) {
									Item temp = makeItem();
									p.setItem(temp);
									cout << p.showItem() << endl;
								}
								break;
							} else if (p.getHP() <= 0) {
								con = false;
								break;
							}
						}
						//run
						else if (choice == 'l' || choice == 'L') {
							if (runAway(p, *badGuy)) {
								con = false;
								enemyCount = 0; roomCount = 0;
								p.setHP(777);	//sets hp to run
								return p;
							} else {
								cout << "Escape failed!\n";
							}
						} else if (choice == 'i' || choice == 'I') {
							//restore 50 hp
							if (p.getItem() == potion) {
								cout << p.showItem() << " used!\n";
								p.setHP(p.getHP() + 50);
								cout << "Player: " << p.getHP() << endl;
							} else if (p.getItem() == bomb) {
								cout << p.showItem() << " used!\n";
								badGuy->setHP(badGuy->getHP() - 25);
								if (checkEnemyDead(*badGuy)) {
									con = false;
									enemyCount--;
									p.updatePos(move);
									p.addXP(*badGuy);
								}
							} else if (p.getItem() == watch) {
								cout << "**********\nFREEZE TIME\n**********\n\n";
								cout << "A - attack | L - escape\n\n";
								char x; cin >> x;
								while (x) {
									if (x == 'a' || x == 'A') {
										badGuy->setHP(badGuy->getHP() - p.Attack());
										if (checkEnemyDead(*badGuy)) {
											enemyCount--;
											p.updatePos(move);
											p.addXP(*badGuy);
											con = false;
											//give item to player
											if (badGuy->hasItem() == true) {
												Item temp = makeItem();
												p.setItem(temp);
												cout << p.showItem() << endl;
											}
										}
										break;
									} else if (x == 'l' || x == 'L') {										//player gets to leave encounter for free if time is frozen
										con = false;
										enemyCount = 0; roomCount = 0;
										p.setHP(777);	//sets hp to run
										return p;
									} else {
										cout << "Invalid choice\n"; continue;
									}
								}
							} else {
								cout << "No item in inventory\n";
							}
							p.setItem(none);

						}
						else {
							cout << "Oops wrong choice...\n";
						}
					}
				}
			}
			//no enemy, moves player
			else  {
				p.updatePos(move);
			}
			//player death
			if (p.getHP() <= 0) {
				cout << "*********\nUR DEAD xP\n*********\n";
				return p;
			}
			//gives random HP between 10-15 every two kills
			if (killCount > 1 && killCount % 2 == 0) {
				int amt = (rand() % 5 + 10);
				p.setHP(p.getHP() + amt);
				cout << "************************\n" << amt << " HP recovered!\n";
				cout << "Total HP: " << p.getHP() << endl;
			}
			//checks door each loop
			door = map.checkDoor(p);
		}
		if (door) {
			cout << "Door found!\n";
		} else if (enemyCount == 0){
			cout << "All enemies defeated!\n";
		}
		//updating variables
		roomCount++;
		if (r->getNext() != NULL) {r = r->getNext();}
		map.setRoom(r);
		pause();
	}
	return p;
}

int main() {
	bool exit = false;
	Player user(empty, "greg", 25, 1); 																			//default player

	while (!exit) {
		int gameCount = user.getGameCount();																	//newgame+
		int choice;
		cout << "Dungeon level: " << gameCount << endl;
		cout << "||-----------------------------------------------------||\n";
		cout << "||-----------------------------------------------------||\n";
		cout << "||-------------Dungeon Greg: Fight Enemies-------------||\n";
		cout << "||-----------------------------------------------------||\n";
		cout << "PLAYER: " << user.getName() << " LEVEL: " << user.getLevel() << endl;
		cout << user.showWeapon() << endl;
		cout << "\n1. Enter dungeon\n2. Create character\n3. Switch weapon\n4. Help\n5. Exit\n";
		cin >> choice;

		if (choice == 1) {
			srand(time(NULL));
			user.setHP(150 + 50*(gameCount-1)); user.setItem(bomb);

			Dungeon dung(rand() % 7 + 2);																		//generates dungeon and rooms
			cout << "# of Rooms: " << dung.getSize() << endl;
			user = enterDungeon(dung, user);

			if (user.getHP() <= 0) {
				cout << "YOU LOSE\n";
				exit = true;
			} else if (user.getHP() == 777) {
				cout << "YOU RAN YOU WEENIE\n\n";
			}
			else {
				cout << "CONGRATS YOU'VE WON\nPlay again? [Y/N]\n";
				char c; cin >> c;
				if (c == 'Y' || c == 'y') {
					user.setGameCount(user.getGameCount() + 1);
					gameCount = user.getGameCount();
					continue;
				} else {
					cout << "SEEYA\n";
					exit = true;
				}
			}


		} else if (choice == 2) {
			user = makePlayer();
		}
		else if (choice == 3) {
			cout << "\n||-------Select the weapon you would like to use-------||\n";
			cout << "1. Sword\n2. Axe\n3. Staff\n4. empty\n5. Nevermind\n";
			int x; cin >>x;
			if (x == 1) {
				user.setWeapon(sword);
			} else if (x == 2) {
				user.setWeapon(axe);
			} else if (x == 3) {
				user.setWeapon(staff);
			}else if (x == 4) {
				user.setWeapon(empty);
			} else {
				continue;
			}
		}
		else if (choice == 4) {
			cout << "\n\n\n\n\n";
			cout << "||-----------------------------------------------------||\n";
			cout << "||-----------------------------------------------------||\n";
			cout << "||------------------- How To Play ---------------------||\n";
			cout << "||------------------ Dungeon Greg ---------------------||\n";
			cout << "||-----------------------------------------------------||\n";
			cout << "||-----------------------------------------------------||\n";

			cout << "Greg has wandered into a dungeon of unknown size with rooms\n" <<
					"potentially full of enemies to fight. Using the keyboard, you\n" <<
					"will guide Greg through the mysterious caverns of your imagination\n\n" <<
					"Each room could hold 1-4 enemies, and there can be 2-9 rooms per dungeon;\n"
					"although the 9th room would be a real challenge if you made it.\n"
					"\nTHIS GAME IS NOT BALANCED.\n"
					"\nIt was really just a way to work on programming over Winter 2021 break,\n"
					"and I plan to come back and make some changes in the future.\n\n";

			cout << "||-----------------------------------------------------||\n";
			cout << "||--------------------- Controls ----------------------||\n";
			cout << "||-----------------------------------------------------||\n";
			cout << "Once you have pushed greg into a dungeon instance, or made your\n"
					"own character to torment,  press WASD to walk to the next block.\n"
					"If any enemy resides in that square, you will have an encounter.\n"
					"In an encounter, \'A\' will attack, \'L\' will attempt to escape\n"
					"the encounter and the dungeon- taking greg, or you, back to the\n"
					"main menu. and \'I\' will allow greg to use one of the items listed\n"
					"below:\n\n";
			cout << "||----------------------- Items -----------------------||\n";
			cout << "POTION: restores 50 hp to the player.\nBOMB: deals 25 damage instantly\n"
					"\tto enemy.\nWATCH: freezes time for one turn, allowing the player to\n"
					"\tget an extra attack, or a guaranteed escape.";
			pause();
		}
		else if (choice == 5){
			cout << "SEEYA\n";
			exit = true;
		}

	}
	return 0;
}
