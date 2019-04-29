#include "Engine\MouseManager.h"
#include "Engine\EngineManager.h"

MouseManager::MouseManager() 
	:	m_bLMouseDown(false),
		m_bMMouseDown(false),
		m_bRMouseDown(false),
		m_iCode(0),
		m_iMouseX(0),
		m_iMouseY(0),
		m_iLastMouseX(0),
		m_iLastMouseY(0),
		m_iVirtualKeyCode(0)
{

}

void MouseManager::onMouseDown(int mCode, int x, int y) {
	m_iCode = mCode;
	m_iMouseX = x;
	m_iMouseY = y;
}

void MouseManager::onMouseHover(int mCode, int x, int y) {
	m_iCode = mCode;
	m_iMouseX = x;
	m_iMouseY = y;
}

void MouseManager::onMouseMove(int mCode, int x, int y) {
	m_iCode = mCode;
	m_iMouseX = x;
	m_iMouseY = y;

	m_iLastMouseX = x;
	m_iLastMouseY = y;
}

void MouseManager::onMouseUp(int mCode, int x, int y) {
	m_iCode = mCode;
	m_iMouseX = x;
	m_iMouseY = y;
}

//void MouseManager::onMouseWheel(WPARAM wParam, LPARAM lParam) {
//
//}

void MouseManager::setLMouseStatus(bool bPressed, int mouseX, int mouseY) {
	m_bLMouseDown = bPressed;

	m_iMouseX = mouseX;
	m_iMouseY = mouseY;
}

void MouseManager::setMMouseStatus(bool bPressed, int mouseX, int mouseY) {
	m_bMMouseDown = bPressed;

	m_iMouseX = mouseX;
	m_iMouseY = mouseY;
}

void MouseManager::setRMouseStatus(bool bPressed, int mouseX, int mouseY) {
	m_bRMouseDown = bPressed;

	m_iMouseX = mouseX;
	m_iMouseY = mouseY;
}

void MouseManager::setMouseMove(unsigned int keyState, int mouseX, int mouseY) {
	m_iVirtualKeyCode = keyState;

	m_iMouseX = mouseX;
	m_iMouseY = mouseY;
}

int MouseManager::getMousePosX() {
	return m_iMouseX;
}

int MouseManager::getMousePosY() {
	return m_iMouseY;
}

MouseManager::~MouseManager() {

}