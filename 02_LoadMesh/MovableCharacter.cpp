#include "TransformationManager.h"



MovableCharacter::MovableCharacter() {
	timeXMovement = timeYMovement = timeDirectionMovement = 0;
}

void MovableCharacter::setLabirint(Labirint* ptrLabirint) {
	labirint = ptrLabirint;
	std::pair<int, int> room = labirint->getRandomFreeRoom();
	int dir = ((room.first + room.second) / static_cast<float>(Labirint::WIDTH + Labirint::HEIGHT)) * 4;
	switch (dir)
	{
	case 0:
		dir = 0;
		break;
	case 1:
		dir = -90;
		break;
	case 2:
		dir = 90;
		break;
	case 3:
		dir = 180;
		break;
	default:
		break;
	}
	//setPosition(room.first, room.second, dir);
	setPosition(0, 0, 0);
}

MovableCharacter::~MovableCharacter() {
}

void MovableCharacter::setPosition(int _x, int _y, int _rotateDegree) {

	xLabirint = _x;
	yLabirint = _y;
	xPosition = (xLabirint*Labirint::ROOM_LENGTH + Labirint::ROOM_LENGTH / 2.0f);
	yPosition = (yLabirint*Labirint::ROOM_LENGTH + Labirint::ROOM_LENGTH / 2.0f);
	rotateDegree = _rotateDegree;
	drawingRotateDegree = rotateDegree;
}

int MovableCharacter::getLabirintX() {
	return xLabirint;
}

int MovableCharacter::getLabirintY() {
	return yLabirint;
}

float MovableCharacter::getPositionX() {
	return xPosition;
}

float MovableCharacter::getPositionY() {
	return yPosition;
}

float MovableCharacter::getDrawingRotateDegree() {
	return drawingRotateDegree;
}

bool MovableCharacter::moveForward() {
	return moveInDirection(1);
}

bool MovableCharacter::moveBack() {
	return moveInDirection(-1);
}

bool MovableCharacter::moveInDirection(int dir) {
	glm::vec2 mv = TransformationManager::rotateDirection(rotateDegree);
	int localX = round(xLabirint + dir* mv.x);
	int localY = round(yLabirint + dir* mv.y);

	if (labirint->isCollision(xLabirint, yLabirint, localX, localY)) {
		return false;
	}

	if (localX != xLabirint) {
		xLabirint = localX;
		timeXMovement = SDL_GetTicks();
	}

	if (localY != yLabirint) {
		yLabirint = localY;
		timeYMovement = SDL_GetTicks();
	}

	return true;
}

void MovableCharacter::rotateLeft() {
	rotateDegree += 90;
	if (rotateDegree > 180) {
		rotateDegree = -90;
	}
	
	timeDirectionMovement = SDL_GetTicks();
}

void MovableCharacter::rotateRight(){
	rotateDegree -= 90;
	if (rotateDegree < -180) {
		rotateDegree = 90;
	}
	timeDirectionMovement = SDL_GetTicks();
}

void MovableCharacter::update() {
	float degree = static_cast<float>(rotateDegree);
	if (degree != drawingRotateDegree) {
		int sign = degree*drawingRotateDegree;
		if (sign < 0) {
			drawingRotateDegree *= -1;
		}

		drawingRotateDegree += (degree - drawingRotateDegree)*(((SDL_GetTicks() - timeDirectionMovement)% 500) / 500.0f);
	}
	float pos = (xLabirint*Labirint::ROOM_LENGTH + Labirint::ROOM_LENGTH / 2.0f);
	if (pos != xPosition) {
		xPosition += (pos - xPosition)*(((SDL_GetTicks() - timeXMovement) % 500) / 500.0f);
	}

	pos = (yLabirint*Labirint::ROOM_LENGTH + Labirint::ROOM_LENGTH / 2.0f);
	if (pos != yPosition) {
		yPosition += (pos - yPosition)*(((SDL_GetTicks() - timeYMovement) % 500) / 500.0f);
	}
}


int  MovableCharacter::getHeight() {
	return height;
}

void  MovableCharacter::setHeight(int _height) {
	height = _height;
}
