#pragma once
// SDL
#include <SDL.h>
#include "Labirint.h"

class MovableCharacter
{
public:
	MovableCharacter();
	~MovableCharacter();
	void setLabirint(Labirint* ptrLabirint);
	void setPosition(int _x, int _y, int _rotateDegree);
	int getLabirintX();
	int getLabirintY();
	float getPositionX();
	float getPositionY();
	int getHeight();
	void setHeight(int _height);
	float getDrawingRotateDegree();
	bool moveForward();
	bool moveBack();
	void rotateLeft();
	void rotateRight();
	void update();
private:
	bool moveInDirection(int dir);
	int xLabirint, yLabirint, rotateDegree;
	float xPosition, yPosition, drawingRotateDegree;
	int height = 2;
	Labirint* labirint;
	int timeXMovement, timeYMovement, timeDirectionMovement;
};

