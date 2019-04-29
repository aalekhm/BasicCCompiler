#include "Engine\KeyboardManager.h"

bool KeyboardManager::m_bKeys[DEF_MAX_KEYS];
unsigned int KeyboardManager::m_iVirtualKeycode = 0;
KeyboardManager::KeyboardManager() {
	for(int i = 0; i < DEF_MAX_KEYS; i++)
		m_bKeys[i] = false;
}

bool KeyboardManager::isKeyPressed(int iKeyID) {
	if(iKeyID < DEF_MAX_KEYS) {
		return m_bKeys[iKeyID];
	}

	return false;
}

void KeyboardManager::keyPressed(unsigned int iVirtualKeycode, unsigned short ch) {
	if(ch < DEF_MAX_KEYS) {
		m_bKeys[ch] = true;
		m_iVirtualKeycode = iVirtualKeycode;
	}
}

void KeyboardManager::keyReleased(unsigned int iVirtualKeycode, unsigned short ch) {
	if(ch < DEF_MAX_KEYS) {
		m_bKeys[ch] = false;
		m_iVirtualKeycode = iVirtualKeycode;
	}
}

KeyboardManager::~KeyboardManager() {

}