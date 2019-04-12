#include "extern/input.h"

#include <string.h>

	InputHandler::InputHandler(){
		memset((void*)m_keyboardKeys, 0, sizeof(m_keyboardKeys) / sizeof(ButtonEvent));
		memset((void*)m_mouseButtons, 0, sizeof(m_mouseButtons) / sizeof(ButtonEvent));
		m_timer.start();
	}

	InputHandler::ButtonState InputHandler::queryMouseButton(InputHandler::MouseButton _button) const {
		return m_mouseButtons[_button].m_state;
	}

	InputHandler::ButtonState InputHandler::queryKeyboardButton(char _character) const{
		return m_keyboardKeys[_character].m_state;
	}

	void InputHandler::resetMouseButtonState(InputHandler::MouseButton _button){
		m_mouseButtons[_button].m_state = InputHandler::IDLE;
	}

	void InputHandler::resetKeyboardButtonState(char _character){
		m_keyboardKeys[_character].m_state = InputHandler::IDLE;
	}

	void InputHandler::queryMousePosition(int& _mousePosX, int& _mousePosY) const{
		_mousePosX = m_mousePosX;
		_mousePosY = m_mousePosY;
	}

	double InputHandler::elapsedTime(){
		return m_timer.getElapsedTime();
	}

	void InputHandler::injectMouseButton(InputHandler::MouseButton _button, InputHandler::ButtonState _state){
		m_mouseButtons[_button].m_state = _state;
	}

	void InputHandler::injectKeyboardButton(char _character, InputHandler::ButtonState _state){
		m_keyboardKeys[_character].m_state = _state;
	}

	void InputHandler::injectMousePosition(int _mousePosX, int _mousePosY){
		m_mousePosX = _mousePosX;
		m_mousePosY = _mousePosY;
	}
	//-----------------------------------Timer---------------------------------
	InputHandler::Timer::Timer(){
		QueryPerformanceFrequency(&frequency);
		startCount.QuadPart = 0;
		endCount.QuadPart = 0;
		stopped = 1;
		startTimeInMicroSec = 0;
		endTimeInMicroSec = 0;
	}

	InputHandler::Timer::~Timer(){

	}

	void InputHandler::Timer::start(){
		stopped = 0;
		QueryPerformanceCounter(&startCount);
		startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
	}

	void InputHandler::Timer::stop(){
		stopped = 1;
		QueryPerformanceCounter(&endCount);
	}

	double InputHandler::Timer::getElapsedTime(){
		if (!stopped)
			QueryPerformanceCounter(&endCount);

		endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
		return 0.000001 * (endTimeInMicroSec - startTimeInMicroSec);
	}
