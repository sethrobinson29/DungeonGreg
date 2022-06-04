#ifndef DUNGEON_H_
#define DUNGEON_H_
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;


//rooms to fight enemies in
class Room{
private:
	int enemyCount, doorX, doorY;
	Room *next;
public:
	Room() {												//creates room with 1-4 enemies, and an exit tile on (doorX, doorY)
		enemyCount = (rand() % 4 + 1); 
		doorX = (rand() % 4); doorY = 2;					//door could be in any x value but will always be on the top row
		next = NULL;
	}
	int getEnemyCount() {return enemyCount;}
	void setEnemyCount(int x) {enemyCount == x;}
	Room* getNext() {return next;}
	void setNext(Room *r) {next = r;}
	int getDoorX() {return doorX;}
	int getDoorY() {return doorY;}

};

//Dungeons hold a number of rooms each time you play through the game
class Dungeon {
private:
	int size; 												//number of rooms
	
public:
	Room *rooms;
	
	Dungeon(int s = 1) {
		size = s; rooms = NULL;
		for (int i = 0; i < this->getSize(); i++) {			//fills list with room objects
			this->addRoom();
		}
	}
	int getSize() {return size;}
	void addRoom();
	void displayRoom();
	void print();
	
};

//adds room entry to rooms list
void Dungeon::addRoom() {
	if (rooms == NULL) {
		rooms = new Room();
	} else {
		Room *ptr = rooms;
		while (ptr->getNext() != NULL) {
			ptr = ptr->getNext();
		}
		Room *temp = new Room();
		ptr->setNext(temp);

	}
}

//prints enemy count for all rooms
void Dungeon::print() {
	Room *cur = rooms;
	while (cur != NULL) {
		cout << cur->getEnemyCount() << endl;
		cur = cur->getNext();
	}
}


#endif /* DUNGEON_H_ */
