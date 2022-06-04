#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <string>
#include "characters.h"
#include "dungeon.h"
using namespace std;

/*
 * Class for generating maps that will be displayed in the console
 * 	for Dungeon Greg
 */

class Graphics {
private:
	Player p;
	Dungeon d;
	Room *r;

	//these lines form the top and bottom row of the room
	string line = "* * * * * * * * *\n*   *   *   *   *\n*   *   *   *   *\n*   *   *   *   *\n* * * * * * * * *\n";
	string lineX0 = "* * * * * * * * *\n*   *   *   *   *\n* X *   *   *   *\n*   *   *   *   *\n* * * * * * * * *\n";
	string lineX1 = "* * * * * * * * *\n*   *   *   *   *\n*   * X *   *   *\n*   *   *   *   *\n* * * * * * * * *\n";
	string lineX2 = "* * * * * * * * *\n*   *   *   *   *\n*   *   * X *   *\n*   *   *   *   *\n* * * * * * * * *\n";
	string lineX3 = "* * * * * * * * *\n*   *   *   *   *\n*   *   *   * X *\n*   *   *   *   *\n* * * * * * * * *\n";

	//these lines form the middle row of the room
	string middleLine = "*   *   *   *   *\n*   *   *   *   *\n*   *   *   *   *\n";
	string middleLineX0 = "*   *   *   *   *\n* X *   *   *   *\n*   *   *   *   *\n";
	string middleLineX1 = "*   *   *   *   *\n*   * X *   *   *\n*   *   *   *   *\n";
	string middleLineX2 = "*   *   *   *   *\n*   *   * X *   *\n*   *   *   *   *\n";
	string middleLineX3 = "*   *   *   *   *\n*   *   *   * X *\n*   *   *   *   *\n";
public:
	Graphics(Player plr, Dungeon dung, Room *room) {
		p = plr; d = dung; r = room;
	}
	Player getPlayer() {return p;}
	Dungeon getDungeon() {return d;}
	Room* getRoom() {return r;}
	void setRoom(Room *room) {r = room;}

	void printSquares();
	void printMap(Player);
	bool checkDoor(Player);
};

//test function
void Graphics::printSquares() {
	cout << lineX0 << lineX1 << lineX2 << endl;
}

/*
 * TODO: expand map
 */
//displays map of dungeon and player location based on p.xPos & p.yPos
void Graphics::printMap(Player p) {
	if (p.getX() == 0) {
		if (p.getY() == 0) {
			cout << line << middleLine << lineX0;
		} else if (p.getY() == 1) {
			cout << line << middleLineX0 << line;
		} else if (p.getY() == 2) {
			cout <<  lineX0 << middleLine << line;
		}
	} else if (p.getX() == 1) {
		if (p.getY() == 0) {
			cout << line << middleLine << lineX1;
		} else if (p.getY() == 1) {
			cout << line << middleLineX1 << line;
		} else if (p.getY() == 2) {
			cout <<  lineX1 << middleLine << line;
		}
	} else if (p.getX() == 2) {
		if (p.getY() == 0) {
			cout << line << middleLine << lineX2;
		} else if (p.getY() == 1) {
			cout << line << middleLineX2 << line;
		} else if (p.getY() == 2) {
			cout <<  lineX2 << middleLine << line;
		}
	} else if (p.getX() == 3) {
		if (p.getY() == 0) {
			cout << line << middleLine << lineX3;
		} else if (p.getY() == 1) {
			cout << line << middleLineX3 << line;
		} else if (p.getY() == 2) {
			cout <<  lineX3 << middleLine << line;
		}
	} else {																						//error check
		cout << "******NO PLAYER******\n";
		cout << line << line << line;
	}
}

//if player is on door, room ends
bool Graphics::checkDoor(Player plr) {
	if (plr.getX() == r->getDoorX() && plr.getY() == r->getDoorY()) {
		return true;
	}
	return false;
}

#endif /* GRAPHICS_H_ */
