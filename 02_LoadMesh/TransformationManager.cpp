#include "TransformationManager.h"



TransformationManager::TransformationManager()
{
}


TransformationManager::~TransformationManager()
{
}

void TransformationManager::Init(Labirint* _labirint, MovableCharacter* _suzanne) {
	labirint = _labirint;
	suzanne = _suzanne;
	// vetítési mátrix létrehozása
	m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);
}


void TransformationManager::Update() {
	// nézeti transzformáció beállítása
	switch (view_mod) {
	case THIRD_PERSON_VIEW:
		m_at = glm::vec3(glm::translate<float>(suzanne->getPositionX(), 0, suzanne->getPositionY()) *
			glm::rotate<float>(suzanne->getDrawingRotateDegree(), 0, 1, 0)*
			glm::vec4(0, 0.0f, Labirint::ROOM_LENGTH, 1));
		m_eye = glm::vec3(glm::translate<float>(suzanne->getPositionX(), 2 * suzanne->getHeight(), suzanne->getPositionY()) *
			glm::rotate<float>(suzanne->getDrawingRotateDegree(), 0, 1, 0)*
			glm::vec4(0, 3 * suzanne->getHeight(), -3 * Labirint::ROOM_LENGTH / 2.0f, 1));
		break;
	case TOP_DOWN_VIEW:
		m_at = glm::vec3(glm::translate<float>(suzanne->getPositionX(), 0, suzanne->getPositionY()) *
			glm::vec4(0, 0.0f, Labirint::ROOM_LENGTH, 1));
		m_eye = glm::vec3(glm::translate<float>(suzanne->getPositionX(), 2 * suzanne->getHeight(), suzanne->getPositionY()) *
			glm::vec4(0, 30 * suzanne->getHeight(), -10 * Labirint::ROOM_LENGTH / 2.0f, 1));
		break;
	}

	m_matView = glm::lookAt(m_eye,		// honnan nézzük a színteret
		m_at,		// a színtér melyik pontját nézzük
		m_up);		// felfelé mutató irány a világban
}

Labirint* TransformationManager::getLabirint() {
	return labirint;
}

glm::vec2 TransformationManager::rotateDirection(float degree) {
	glm::vec4 dir = glm::vec4(0, 0, 1, 1);
	dir = glm::rotate<float>(degree, 0,1,0) * dir;
	return glm::vec2(dir.x, dir.z);
}

void TransformationManager::setPerspectiveTransf(glm::mat4& proj) {
	m_matProj = proj;
}

glm::mat4 TransformationManager::getCoinTransformation(int y, int x) {
	float degree = ((SDL_GetTicks() % 4000) / 4000.0f) * 360;
	m_matWorld = glm::translate<float>(Labirint::ROOM_LENGTH* x + Labirint::ROOM_LENGTH / 2, Labirint::ROOM_HEIGHT, Labirint::ROOM_LENGTH* y + Labirint::ROOM_LENGTH / 2) *
		glm::rotate<float>(degree, 0, 1, 0) *
		glm::scale(glm::vec3(Labirint::ROOM_LENGTH *0.05f, Labirint::ROOM_HEIGHT*0.5f, Labirint::ROOM_LENGTH *0.3));

	return m_matProj * m_matView * m_matWorld;
}

glm::mat4 TransformationManager::getDiamondTransformation(int y, int x) {
	float degree = ((SDL_GetTicks() % 4000) / 4000.0f) * 360;
	m_matWorld = glm::translate<float>(Labirint::ROOM_LENGTH* x + Labirint::ROOM_LENGTH / 2, Labirint::ROOM_HEIGHT, Labirint::ROOM_LENGTH* y + Labirint::ROOM_LENGTH / 2) *
		glm::rotate<float>(degree, 0, 1, 0);

	return m_matProj * m_matView * m_matWorld;
}

glm::mat4 TransformationManager::getGroundTransformation() {
	glm::mat4 m_matWorld = glm::scale(glm::vec3(Labirint::ROOM_LENGTH*Labirint::WIDTH, 1, Labirint::ROOM_LENGTH*Labirint::HEIGHT));
	return m_matProj * m_matView * m_matWorld;
}

glm::mat4 TransformationManager::getSurrandingWallTransformation(int index) {
	glm::mat4 roomWallTransformation = getWallTransformation(index, Labirint::ROOM_LENGTH* Labirint::WIDTH);

	m_matWorld = roomWallTransformation *
		glm::scale(glm::vec3(Labirint::ROOM_LENGTH* Labirint::WIDTH, Labirint::ROOM_HEIGHT, 1));
	return m_matProj * m_matView * m_matWorld;
}

glm::mat4 TransformationManager::getRoomTransformation(int y,  int x, int wallIndex) {
	if (labirint->getRoom(x, y).wall[wallIndex]) {
		m_matWorld = glm::translate<float>(Labirint::ROOM_LENGTH* x, 0, Labirint::ROOM_LENGTH* y) *
			getWallTransformation(wallIndex, Labirint::ROOM_LENGTH) *
			glm::scale(glm::vec3(Labirint::ROOM_LENGTH, Labirint::ROOM_HEIGHT, 1));

		return m_matProj * m_matView * m_matWorld;
	}
	else {
		glm::mat4(0.0f);
	}
}

glm::mat4 TransformationManager::getWallTransformation(int wallIndex, float length) {
	switch (wallIndex)
	{
	case 0:
		return	glm::translate<float>(length, 0, 0) * 
				glm::rotate<float>(-90, 0, 1, 0);
	case 1:
		return glm::translate<float>(0, 0, length);
	case 2:
		return glm::mat4(1.0f);
	case 3:
		return glm::rotate<float>(-90, 0, 1, 0);

	}
}

glm::mat4 TransformationManager::getSuzanneTransformation(){
	int size = suzanne->getHeight();
	m_matWorld =glm::translate<float>(suzanne->getPositionX(), suzanne->getHeight()/2.0f, suzanne->getPositionY()) *
			glm::scale<float>(size, size, size) *
			glm::rotate<float>(suzanne->getDrawingRotateDegree(), 0, 1, 0);
	return m_matProj * m_matView * m_matWorld;
}

void TransformationManager::rotateMouse(float xrel, float yrel)
{
	m_u += xrel;
	m_v += yrel;
	m_v = glm::clamp(m_v, 0.1f, 3.01f);
	float cu = cosf(m_u), su = sinf(m_u), sv = sinf(m_v), cv = cosf(m_v);
	m_fw = glm::vec3(cu*sv, cv, su*sv);
	m_at = m_eye + m_fw;
}

void TransformationManager::moveForward() {
	m_eye += m_fw*glm::mat3(10.0f);
	m_at += m_fw*glm::mat3(10.0f);
}

void TransformationManager::moveLeft() {
	glm::vec3 st = glm::normalize(glm::cross(m_fw, m_up));
	m_eye -= st;
	m_at -= st;
}

void TransformationManager::moveRight() {
	glm::vec3 st = glm::normalize(glm::cross(m_fw, m_up));
	m_eye += st;
	m_at += st;
}

void TransformationManager::moveBack() {
	m_eye -= m_fw*glm::mat3(10.0f);
	m_at -= m_fw*glm::mat3(10.0f);
}

void TransformationManager::switchViewMod() {
	view_mod = (view_mod + 1) % 2;
}


