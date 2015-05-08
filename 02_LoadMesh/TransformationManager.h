#pragma once

// SDL
#include <SDL.h>
#include <SDL_opengl.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <math.h>

#include "Labirint.h"
#include "MovableCharacter.h"

class TransformationManager
{
public:
	const static int THIRD_PERSON_VIEW = 0;
	const static int TOP_DOWN_VIEW = 1;

	TransformationManager();
	~TransformationManager();
	void Init(Labirint* _labirint, MovableCharacter* _suzanne);
	void Update();
	void moveForward();
	void moveLeft();
	void moveRight();
	void moveBack();
	void rotateMouse(float xrel, float yrel);
	void switchViewMod();
	void setPerspectiveTransf(glm::mat4& proj);
	static glm::vec2 rotateDirection(float degree);
	Labirint* getLabirint();
	glm::mat4 getGroundTransformation();
	glm::mat4 getSurrandingWallTransformation(int index);
	glm::mat4 getCoinTransformation(int y, int x);
	glm::mat4 getDiamondTransformation(int y, int x);
	glm::mat4 getRoomTransformation(int y, int x, int wallIndex);
	glm::mat4 getSuzanneTransformation();
	glm::mat4 getWallTransformation(int wallIndex, float length);
private:
	Labirint* labirint;
	MovableCharacter* suzanne;
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	glm::vec3 m_eye;
	glm::vec3 m_at;
	glm::vec3 m_up = glm::vec3(0, 1, 0);
	glm::vec3 m_fw = glm::normalize(glm::vec3(-10, -10, -10));
	float m_u = atan2f(-10, -10), m_v = acosf(-10 / sqrt(300));
	int view_mod = THIRD_PERSON_VIEW;

};

