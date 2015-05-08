#pragma once

// GLEW
#include <GL/glew.h>

#include "TransformationManager.h"

#include <GL/GLU.h>
#include <math.h>
#include <GL/GLU.h>
// mesh
#include "ObjParser_OGL3.h"
#include "GLUtils.h"
#include "MovableCharacter.h"


class Drawer
{
public:
	struct Vertex
	{
		glm::vec3 p; // pozíció
		glm::vec3 c; // szín
		glm::vec2 t; // textúra koordináták
	};
	Drawer();
	~Drawer();
	void Init(TransformationManager* transManager);
	void Clean();
	void Clear();
	void DrawGround();
	void DrawSuzanne();
	void drawLabirint();
private:
	void loadGeometry(GLuint &m_vaoID, GLuint &m_vboID, GLuint &m_ibID, Vertex* vert, GLushort* indeces, int sizeOfVert, int sizeOfIndex);
	void drawElement(GLuint m_vaoID, GLuint m_textureID, glm::mat4& mvp, int numberOfVertices);
	void Drawer::InitCylinder();
	void InitGeometry();
	void InitDiamondGeometry();
	Vertex GetCylinerVertex(float u, float v);
	Labirint* labirint;
	const int N = 30;
	const int M = 30;


	GLuint m_programID; // shaderek programja
	// mátrixok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;

	// OpenGL-es dolgok
	GLuint m_vaoIDWall; // vertex array object erõforrás azonosító
	GLuint m_vboIDWall; // vertex buffer object erõforrás azonosító
	GLuint m_ibIDWall;  // index buffer object erõforrás azonosító

	GLuint m_vaoIDGround; // vertex array object erõforrás azonosító
	GLuint m_vboIDGround; // vertex buffer object erõforrás azonosító
	GLuint m_ibIDGround;  // index buffer object erõforrás azonosító

	GLuint m_vaoIDDiamond; // vertex array object erõforrás azonosító
	GLuint m_vboIDDiamond; // vertex buffer object erõforrás azonosító
	GLuint m_ibIDDiamond;  // index buffer object erõforrás azonosító


	GLuint m_vaoIDCylinder; // vertex array object erõforrás azonosító
	GLuint m_vboIDCylinder; // vertex buffer object erõforrás azonosító
	GLuint m_ibIDCylinder;  // index buffer object erõforrás azonosító

	GLuint m_wallTextureID;
	GLuint m_suzanneTextureID;

	// mesh adatok
	Mesh *m_mesh;
	TransformationManager* transformationManager;
};

