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


	// indexpuffer adatai: NxM n�gysz�g = 2xNxM h�romsz�g = h�romsz�glista eset�n 3x2xNxM index
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

	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	//
	// geometria letrehozasa
	//
	InitGeometry();
	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
		0,				// a VAO-beli azonos�t� index
		"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation(m_programID, 1, "vs_in_col");
	glBindAttribLocation(m_programID, 2, "vs_in_tex0");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
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

		std::cout << "[app.Init()] S�der Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");

	m_loc_texture = glGetUniformLocation(m_programID, "textureImg");

	//
	// egy�b er�forr�sok bet�lt�se
	//

	// text�ra bet�lt�se
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
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Drawer::DrawGround()
{
	glUseProgram(m_programID);
	// majd k�ldj�k �t a megfelel� m�trixokat!
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
	
	// majd k�ldj�k �t a megfelel� m�trixokat!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre t�lts�nk �t adatot
		1,			// egy darab m�trixot
		GL_FALSE,	// NEM transzpon�lva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot
	
	// aktiv�ljuk a 0-�s text�ra mintav�telez� egys�get
	glActiveTexture(GL_TEXTURE0);
	// aktiv�ljuk a gener�lt text�r�nkat 
	glBindTexture(GL_TEXTURE_2D, m_suzanneTextureID);
	// text�ra mintav�telez� �s shader-beli sampler2D �sszerendel�se
	glUniform1i(m_loc_texture,	// ezen azonos�t�j� sampler 2D
		0);				// olvassa az ezen index� mintav�telez�t
	

	m_mesh->draw();

	glUseProgram(0);
}


void Drawer::drawLabirint() {
	glUseProgram(m_programID);
	
	for (int i = 0; i < 4; i++)
	{
		// majd k�ldj�k �t a megfelel� m�trixokat!
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
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre t�lts�nk �t adatotmvp
		1,			// egy darab m�trixot
		GL_FALSE,	// NEM transzpon�lva
		&mvp[0][0]); // innen olvasva a 16 x sizeof(float)-nyi adatot

	if (m_textureID != 0) {
		// aktiv�ljuk a 0-�s text�ra mintav�telez� egys�get
		glActiveTexture(GL_TEXTURE0);
		// aktiv�ljuk a gener�lt text�r�nkat 
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		// text�ra mintav�telez� �s shader-beli sampler2D �sszerendel�se
		glUniform1i(m_loc_texture,	// ezen azonos�t�j� sampler 2D
			0);				// olvassa az ezen index� mintav�telez�t
	}
	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	glDrawElements(GL_TRIANGLES,		// primit�v t�pus
		numberOfVertices,					// hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,	// indexek tipusa
		0);			// indexek cime

	if (m_textureID != 0) {
		// text�ra kikapcsol�sa
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// VAO kikapcsolasa
	glBindVertexArray(0);

}


void Drawer::loadGeometry(GLuint &m_vaoID, GLuint &m_vboID, GLuint &m_ibID, Vertex *vert, GLushort* indeces, int sizeOfVert, int sizeOfIndex) {
	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);

	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
		sizeOfVert,		// ennyi b�jt nagys�gban
		vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
		GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat


	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
		);

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); // ez lesz majd a sz�n
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// text�rakoordin�t�k bekapcsol�sa a 2-es azonos�t�j� attrib�tom csatorn�n
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(glm::vec3)));

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_ibID);
	// a VAO �szreveszi, hogy bind-olunk egy index puffert �s feljegyzi, hogy melyik volt ez!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndex, indeces, GL_STATIC_DRAW);

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
}