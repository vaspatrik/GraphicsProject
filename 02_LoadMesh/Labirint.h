#pragma once

#include <cstdlib>
#include <ctime> 
#include <iostream>
#include <vector>

class Labirint
{
public:
	struct Room {
		bool wall[2];
		bool hasCoin = false;
		bool hasDiamond = false;
	};

	static const int WIDTH = 20;
	static const int HEIGHT = 20;
	static const int ROOM_HEIGHT = 5;
	static const int ROOM_LENGTH = 10;


	Labirint();
	~Labirint();

	int getWallProbability();
	void setWallProbability(int probability);
	void createLabirint();
	void generateCoins();
	void generateDiamonds();
	std::pair<int, int> getRandomFreeRoom();
	Room getRoom(int x, int y);
	bool isCollision(int x1, int y1, int x2, int y2);
private:
	Room rooms[HEIGHT][WIDTH];
	int wallProbability = 40;
	int coinNumber = 100;
	int diamondNumber = 10;
};

