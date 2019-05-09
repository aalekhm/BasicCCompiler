#include "Engine/EngineManager.h"

EngineManager*	EngineManager::m_pEngineManager;

EngineManager::EngineManager()
	:	m_pTimer(NULL),
		m_pKeyboardManager(NULL),
		m_pMouseManager(NULL),
		m_pHWnd(NULL),		

		m_iViewportW(0),
		m_iViewportH(0),
		m_iState(UNINITIALIZED),
	 	m_bInitialized(false),

		m_iFrameCount(0),
		m_iFrameRate(0),
		m_dLastElapsedFPSTimeMs(0.0f)
{
	m_pEngineManager = this;
}

EngineManager* EngineManager::getInstance() {
	GL_ASSERT(m_pEngineManager);
	return m_pEngineManager;
}

void EngineManager::startup(HWND pHWnd) {
	m_pHWnd = pHWnd;
	RECT rClientRect;
	GetClientRect(m_pHWnd, &rClientRect);
	setViewport(rClientRect.right - rClientRect.left, rClientRect.bottom - rClientRect.top);

	m_pKeyboardManager = new KeyboardManager();
	m_pMouseManager = new MouseManager();
	m_pTimer = new Timer();
	initTimer();

	m_iState = RUNNING;
}

HWND EngineManager::getWindowHandle() {
	return m_pHWnd;
}

void EngineManager::setWindowText(const char* sTitle)
{
	if (sTitle != nullptr && strlen(sTitle) > 0)
	{
		SetWindowText(m_pHWnd, sTitle);
	}
}

void EngineManager::shutdown() {
	if(m_iState != UNINITIALIZED) {

		m_iState = UNINITIALIZED;
	}
}

void EngineManager::exit() {
	shutdown();
}

EngineManager::State EngineManager::getState() {
	return m_iState;
}

void EngineManager::setViewport(int w, int h) {
	m_iViewportW = w;
	m_iViewportH = h;
}

void EngineManager::initTimer() {
	m_pTimer->initTimer();
}

Timer*	EngineManager::getTimer() {
	return m_pTimer;
}

bool EngineManager::isKeyPressed(int iKeyID) {
	return KeyboardManager::isKeyPressed(iKeyID);
}

void EngineManager::keyPressed(unsigned int iVirtualKeycode, unsigned short ch) {
	m_pKeyboardManager->keyPressed(iVirtualKeycode, ch);
	keyPressedEx(iVirtualKeycode, ch);
}

void EngineManager::keyReleased(unsigned int iVirtualKeycode, unsigned short ch) {
	m_pKeyboardManager->keyReleased(iVirtualKeycode, ch);
	keyReleasedEx(iVirtualKeycode, ch);
}

void EngineManager::frame() {
	if(!m_bInitialized) {
		initialize();
		m_bInitialized = true;
	}

	if(m_iState == RUNNING) {
		m_pTimer->startFrame();

		update((float)m_pTimer->getDeltaTimeMs());
		render((float)m_pTimer->getDeltaTimeMs());

		updateFPS();

		m_pTimer->endFrame();
	}
}

void EngineManager::onMouseDown(int mCode, int x, int y) {
	m_pMouseManager->onMouseDown(mCode, x, y);
	onMouseDownEx(mCode, x, y);
}

void EngineManager::onMouseMove(int mCode, int x, int y) {
	if(mCode == 0) {
		m_pMouseManager->onMouseHover(mCode, x, y);
	}
	else {
		m_pMouseManager->onMouseMove(mCode, x, y);
	}

	onMouseMoveEx(mCode, x, y);
}

void EngineManager::onMouseWheel(WPARAM wParam, LPARAM lParam) {
	//m_pMouseManager->onMouseWheel(wParam, lParam);
	onMouseWheelEx(wParam, lParam);
}

void EngineManager::onMouseUp(int mCode, int x, int y) {
	m_pMouseManager->onMouseUp(mCode, x, y);
	onMouseUpEx(mCode, x, y);
}

void EngineManager::setLMouseStatus(bool bPressed, int mouseX, int mouseY) {
	m_pMouseManager->setLMouseStatus(bPressed, mouseX, mouseY);
}

void EngineManager::setMMouseStatus(bool bPressed, int mouseX, int mouseY) {
	m_pMouseManager->setMMouseStatus(bPressed, mouseX, mouseY);
}

void EngineManager::setRMouseStatus(bool bPressed, int mouseX, int mouseY) {
	m_pMouseManager->setRMouseStatus(bPressed, mouseX, mouseY);
}

void EngineManager::setMouseMove(WPARAM keyState, int mouseX, int mouseY) {
	m_pMouseManager->setMouseMove(keyState, mouseX, mouseY);
}

MouseManager* EngineManager::getMouseManager() const {
	return m_pMouseManager;
}

unsigned int EngineManager::getWidth() {
	return m_iViewportW;
}

unsigned int EngineManager::getHeight() {
	return m_iViewportH;
}

//double gLastElapsedTimeMillis = -1.0f;

void EngineManager::updateFPS() {
	++m_iFrameCount;

	if( (m_pTimer->getFrameElapsedTime() - m_dLastElapsedFPSTimeMs) >= 1000.0f) {
		m_iFrameRate = m_iFrameCount;
		m_iFrameCount = 0;
		m_dLastElapsedFPSTimeMs = m_pTimer->getElapsedTimeInMilliSec();

		char sText[255];
		sprintf_s(sText, 255, "FPS: %d", m_iFrameRate);
		setWindowText(sText);
	}
}

unsigned int EngineManager::getFPS() {
	return m_iFrameRate;
}

EngineManager::~EngineManager() {

}