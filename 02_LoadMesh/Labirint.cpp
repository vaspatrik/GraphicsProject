#include "Labirint.h"


Labirint::Labirint()
{
	srand(time(NULL));
	createLabirint();
}

Labirint::~Labirint()
{
}

std::pair<int, int> Labirint::getRandomFreeRoom() {
	int x, y;
	do {
		x = rand() % WIDTH;
		y = rand() % HEIGHT;
	} while ((rooms[y][x].hasDiamond) || (rooms[y][x].hasCoin));

	return std::pair<int, int>(x, y);
}

bool Labirint::isCollision(int x1, int y1, int x2, int y2) {
	if ((x2 >= WIDTH) || (x2 < 0) || (y2 >= HEIGHT) || (y2 < 0) ||
		(x1 >= WIDTH) || (x1 < 0) || (y1 >= HEIGHT) || (y1 < 0)) {
		return true;
	}
	int x = x2 - x1;
	int y = y2 - y1;
	if ((y == 0) && (abs(x) == 1)) {
		switch (x)
		{
		case 1:
			return rooms[y1][x1].wall[0];
		case -1:
			return rooms[y2][x2].wall[0];
		default:
			break;
		}
	}
	else if ((x == 0) && (abs(y) == 1)) {
		switch (y)
		{
		case 1:
			return rooms[y1][x1].wall[1];
		case -1:
			return rooms[y2][x2].wall[1];
		default:
			break;
		}
	}
	else {
		std::cout << "WHAT???\n";
		return false;
	}

}

int Labirint::getWallProbability() {
	return wallProbability;
}

void Labirint::setWallProbability(int probability){
	wallProbability = probability;
}

Labirint::Room Labirint::getRoom(int x, int y){
	return rooms[y][x];
}

void Labirint::createLabirint() {
	int wallChance = 0;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			for (int k = 0; k < 2; k++) {
				wallChance = rand() % 100;
				rooms[i][j].wall[k] = (wallChance < wallProbability);
			}
		}
	}
	generateCoins();
	generateDiamonds();
}

void Labirint::generateCoins() {

	for (int i = 0; i < coinNumber; i++)
	{
		int x, y;
		do {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
		} while ((rooms[y][x].hasDiamond) || (rooms[y][x].hasCoin));
		rooms[y][x].hasCoin = true;
	}
}


void Labirint::generateDiamonds() {

	for (int i = 0; i < diamondNumber; i++)
	{
		int x, y;
		do {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
		} while ((rooms[y][x].hasDiamond) || (rooms[y][x].hasCoin));
		rooms[y][x].hasDiamond = true;
	}
}
