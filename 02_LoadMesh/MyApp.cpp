#include "MyApp.h"


CMyApp::CMyApp(void)
{
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	suzanne.setLabirint(&labirint);
	transformationManager.Init(&labirint, &suzanne);
	drawer.Init(&transformationManager);
	return true;
}

void CMyApp::Clean()
{
	drawer.Clean();
}

void CMyApp::Update()
{
	transformationManager.Update();
	suzanne.update();
}


void CMyApp::Render()
{
	drawer.Clear();
	drawer.DrawGround();
	drawer.DrawSuzanne();
	drawer.drawLabirint();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		suzanne.moveForward();
		//transformationManager.moveForward();
		break;
	case SDLK_s:
		suzanne.moveBack();
		//transformationManager.moveBack();
		break;
	case SDLK_a:
		suzanne.rotateLeft();
		//transformationManager.moveLeft();
		break;
	case SDLK_d:
		suzanne.rotateRight();
		//transformationManager.moveRight();
		break;
	case SDLK_c:
		transformationManager.switchViewMod();
		//transformationManager.moveRight();
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (m_left_pressed)
	{
		transformationManager.rotateMouse(mouse.xrel / 100.0f, mouse.yrel / 100.0f);
	}
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
	{
		m_left_pressed = true;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
	{
		m_left_pressed = false;
	}
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	
	glViewport(0, 0, _w, _h);

	transformationManager.setPerspectiveTransf(glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f));		// tavoli vagosik
}

