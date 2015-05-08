#include "Drawer.h"


Drawer::Drawer()
{
	m_vaoIDWall = 0;
	m_vboIDWall = 0;

	m_vaoIDGround = 0;
	m_vboIDGround = 0;

	m_programID = 0;
	m_wallTextureID = 0;
	m_suzanneTextureID = 0;
	m_mesh = 0;
}

Drawer::~Drawer()
{
}

Drawer::Vertex Drawer::GetCylinerVertex(float u, float v)
{
	u *= 2 * 3.1415f;
	v *= 1;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 1;

	return{ glm::vec3(v, r*cu, r*su), glm::vec3(1, 1, 0), glm::vec2(0, 0) };
}


void Drawer::InitGeometry() {

	Vertex vertWall[] =
	{
		//          x,  y, z               nx,ny,nz			 s, t
		{ glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0) },
		{ glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
		{ glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0) },
		{ glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec2(0, 1 / 3.0f) },
		{ glm::vec3(0, 1, 1), glm::vec3(0, 1, 0), glm::vec2(0, 2 / 3.0f) },
		{ glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 2 / 3.0f) },
		{ glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec2(1, 1 / 3.0f) },
	};

	Vertex vertGround[] = {
		{ glm::vec3(0, 0, 0), glm::vec3(0.62, 0.60, 0.11), glm::vec2(0, 0) },
		{ glm::vec3(0, 0, 1), glm::vec3(0.62, 0.60, 0.11), glm::vec2(1, 0) },
		{ glm::vec3(1, 0, 1), glm::vec3(0.62, 0.60, 0.11), glm::vec2(1, 1) },
		{ glm::vec3(1, 0, 0), glm::vec3(0.62, 0.60, 0.11), glm::vec2(0, 1) },
	};



	// indexpuffer adatai
	GLushort indicesWall[] =
	{
		4, 5, 7,
		5, 6, 7,
		5, 0, 1,
		5, 4, 0,
		2, 3, 6,
		3, 7, 6,
		0, 4, 3,
		4, 7, 3,
		1, 6, 5,
		1, 2, 6
	};

	GLushort indicesGround[] = {
		0, 1, 3,
		3, 1, 2
	};

	InitCylinder();
	InitDiamondGeometry();
	loadGeometry(m_vaoIDWall, m_vboIDWall, m_ibIDWall, vertWall, indicesWall, sizeof(vertWall), sizeof(indicesWall));
	loadGeometry(m_vaoIDGround, m_vboIDGround, m_ibIDGround, vertGround, indicesGround, sizeof(vertGround), sizeof(indicesGround));

}

void Drawer::InitCylinder() {
	const int N = 30;
	const int M = 2;

	Vertex vertCylinder[(N + 1)*(M + 1)];
	for (int i = 0; i <= N; ++i){
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vertCylinder[i + j*(N + 1)] = GetCylinerVertex(u, v);
		}
	}


	// indexpuffer adatai: NxM négyszög = 2xNxM háromszög = háromszöglista esetén 3x2xNxM index
	GLushort indices[3 * 2 * (N)*(M)+2 * 3 * (N - 2)];
	for (int i = 0; i<N; ++i)
	for (int j = 0; j<M; ++j)
	{
		indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j)*	(N + 1);
		indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j)*	(N + 1);
		indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1)*(N + 1);
		indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j)*	(N + 1);
		indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1)*(N + 1);
		indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1)*(N + 1);
	}

	for (int i = 0; i < N - 2; i++) {
		indices[3 * 2 * (N)*(M)+3 * i] = 0;
		indices[3 * 2 * (N)*(M)+3 * i + 1] = i + 2;
		indices[3 * 2 * (N)*(M)+3 * i + 2] = i + 1;

		indices[3 * 2 * (N)*(M)+3 * i + 3 * (N - 2)] = M*(N + 1);
		indices[3 * 2 * (N)*(M)+3 * i + 1 + 3 * (N - 2)] = i + 1 + (M)*(N + 1);
		indices[3 * 2 * (N)*(M)+3 * i + 2 + 3 * (N - 2)] = i + 2 + M*(N + 1);
	}
	loadGeometry(m_vaoIDCylinder, m_vboIDCylinder, m_ibIDCylinder, vertCylinder, indices, sizeof(vertCylinder), sizeof(indices));
}

void Drawer::InitDiamondGeometry() {
	Vertex vertDiamond[] =
	{
		//          x,  y, z               nx,ny,nz				 s, t
		{ glm::vec3(-1 / 2.0f, 0, sqrt(3) / 2.0f), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		{ glm::vec3(1 / 2.0f, 0, sqrt(3) / 2.0f), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		{ glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		{ glm::vec3(1 / 2.0f, 0, -sqrt(3) / 2.0f), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		{ glm::vec3(-1 / 2.0f, 0, -sqrt(3) / 2.0f), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		{ glm::vec3(-1, 0, 0), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		
		{ glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
		{ glm::vec3(0, -2, 0), glm::vec3(1, 1, 1), glm::vec2(0, 0) },
	};

	// indexpuffer adatai
	GLushort indicesDiamond[] =
	{
		5, 0, 6,
		0, 1, 6,
		1, 2, 6,
		2, 3, 6,
		3, 4, 6,
		4, 5, 6,

		0, 5, 7,
		1, 0, 7,
		2, 1, 7,
		3, 2, 7,
		4, 3, 7,
		5, 4, 7
	
		
	};
	loadGeometry(m_vaoIDDiamond, m_vboIDDiamond, m_ibIDDiamond, vertDiamond, indicesDiamond, sizeof(vertDiamond), sizeof(indicesDiamond));
}

void Drawer::Init(TransformationManager* transManager) {

	transformationManager = transManager;
	labirint = transManager->getLabirint();

	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	//
	// geometria letrehozasa
	//
	InitGeometry();
	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
		0,				// a VAO-beli azonosító index
		"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation(m_programID, 1, "vs_in_col");
	glBindAttribLocation(m_programID, 2, "vs_in_tex0");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy(aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");

	m_loc_texture = glGetUniformLocation(m_programID, "textureImg");

	//
	// egyéb erõforrások betöltése
	//

	// textúra betöltése
	m_wallTextureID = TextureFromFile("wall_texture.bmp");
	m_suzanneTextureID = TextureFromFile("suzanne_texture.png");

	// mesh betoltese
	m_mesh = ObjParser::parse("Suzanne.obj");
	m_mesh->initBuffers();
}

void Drawer::Clean() {
	delete m_mesh;
	glDeleteTextures(1, &m_wallTextureID);
	glDeleteTextures(1, &m_suzanneTextureID);

	glDeleteBuffers(1, &m_vboIDWall);
	glDeleteVertexArrays(1, &m_vaoIDWall);
	glDeleteVertexArrays(1, &m_ibIDWall);

	glDeleteBuffers(1, &m_vboIDGround);
	glDeleteVertexArrays(1, &m_vaoIDGround);
	glDeleteVertexArrays(1, &m_ibIDGround);

	glDeleteBuffers(1, &m_vboIDCylinder);
	glDeleteVertexArrays(1, &m_vaoIDCylinder);
	glDeleteVertexArrays(1, &m_ibIDCylinder);


	glDeleteBuffers(1, &m_vboIDDiamond);
	glDeleteVertexArrays(1, &m_vaoIDDiamond);
	glDeleteVertexArrays(1, &m_ibIDDiamond);


	glDeleteProgram(m_programID);
}


void Drawer::Clear() {
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Drawer::DrawGround()
{
	glUseProgram(m_programID);
	// majd küldjük át a megfelelõ mátrixokat!
	drawElement(
		m_vaoIDGround, 0, 
		transformationManager->getGroundTransformation(), 
		6);
	glUseProgram(0);
}



void Drawer::DrawSuzanne()
{
	// a mesh kirajzolasahoz hasznalt shader bekapcsolasa
	glUseProgram(m_programID);

	
	glm::mat4 mvp = transformationManager->getSuzanneTransformation();
	
	// majd küldjük át a megfelelõ mátrixokat!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre töltsünk át adatot
		1,			// egy darab mátrixot
		GL_FALSE,	// NEM transzponálva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot
	
	// aktiváljuk a 0-és textúra mintavételezõ egységet
	glActiveTexture(GL_TEXTURE0);
	// aktiváljuk a generált textúránkat 
	glBindTexture(GL_TEXTURE_2D, m_suzanneTextureID);
	// textúra mintavételezõ és shader-beli sampler2D összerendelése
	glUniform1i(m_loc_texture,	// ezen azonosítójú sampler 2D
		0);				// olvassa az ezen indexû mintavételezõt
	

	m_mesh->draw();

	glUseProgram(0);
}


void Drawer::drawLabirint() {
	glUseProgram(m_programID);
	
	for (int i = 0; i < 4; i++)
	{
		// majd küldjük át a megfelelõ mátrixokat!
		drawElement(m_vaoIDWall, m_wallTextureID, 
					transformationManager->getSurrandingWallTransformation(i),
					30);
	}

	glm::mat4 mvp;
	
	for (int i = 0; i < Labirint::HEIGHT; i++) {
		for (int j = 0; j < Labirint::WIDTH; j++) {
			Labirint::Room room = labirint->getRoom(j, i);
			for (int k = 0; k < 2; k++) {
				if (room.wall[k]) {
					drawElement(m_vaoIDWall, 
						m_wallTextureID, 
						transformationManager->getRoomTransformation(i, j, k),
						30);
				}
			}

			if (room.hasCoin) {
				drawElement(m_vaoIDCylinder,
					0,
					transformationManager->getCoinTransformation(i, j),
					3 * 2 * (N)*(M)+2 * 3 * (N - 2));
			}
			if (room.hasDiamond) {
				drawElement(m_vaoIDDiamond,
					0,
					transformationManager->getDiamondTransformation(i, j),
					36);
			}

		}
	}
	glUseProgram(0);

}


void Drawer::drawElement(GLuint m_vaoID, GLuint m_textureID, glm::mat4& mvp, int numberOfVertices) {
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre töltsünk át adatotmvp
		1,			// egy darab mátrixot
		GL_FALSE,	// NEM transzponálva
		&mvp[0][0]); // innen olvasva a 16 x sizeof(float)-nyi adatot

	if (m_textureID != 0) {
		// aktiváljuk a 0-és textúra mintavételezõ egységet
		glActiveTexture(GL_TEXTURE0);
		// aktiváljuk a generált textúránkat 
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		// textúra mintavételezõ és shader-beli sampler2D összerendelése
		glUniform1i(m_loc_texture,	// ezen azonosítójú sampler 2D
			0);				// olvassa az ezen indexû mintavételezõt
	}
	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
	glDrawElements(GL_TRIANGLES,		// primitív típus
		numberOfVertices,					// hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,	// indexek tipusa
		0);			// indexek cime

	if (m_textureID != 0) {
		// textúra kikapcsolása
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// VAO kikapcsolasa
	glBindVertexArray(0);

}


void Drawer::loadGeometry(GLuint &m_vaoID, GLuint &m_vboID, GLuint &m_ibID, Vertex *vert, GLushort* indeces, int sizeOfVert, int sizeOfIndex) {
	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);

	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		sizeOfVert,		// ennyi bájt nagyságban
		vert,	// errõl a rendszermemóriabeli címrõl olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat


	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
		);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// textúrakoordináták bekapcsolása a 2-es azonosítójú attribútom csatornán
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(glm::vec3)));

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	// a VAO észreveszi, hogy bind-olunk egy index puffert és feljegyzi, hogy melyik volt ez!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndex, indeces, GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
}