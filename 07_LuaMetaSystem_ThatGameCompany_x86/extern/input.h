#ifndef INPUT_H
#define INPUT_H

#if _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#include <stdint.h>

	class InputHandler{
	public:
		InputHandler();
		~InputHandler();

		enum ButtonState {
			IDLE = 0,
			PRESSED,
			RELEASED
		};

		enum MouseButton {
			LEFT_MOUSE_BUTTON,
			MIDDLE_MOUSE_BUTTON,
			RIGHT_MOUSE_BUTTON
		};

		struct ButtonEvent{
			ButtonState m_state;
		};

		ButtonState queryMouseButton(MouseButton _button) const;
		void resetMouseButtonState(MouseButton _button);
		ButtonState queryKeyboardButton(char _character) const;
		void resetKeyboardButtonState(char _character);
		void queryMousePosition(int& _mousePosX, int& _mousePosY) const;
		double elapsedTime();

		void injectMouseButton(MouseButton _button, ButtonState _state);
		void injectKeyboardButton(char _character, ButtonState _state);
		void injectMousePosition(int _mousePosX, int _mousePosY);


	private:
		int m_mousePosX;
		int m_mousePosY;

		ButtonEvent m_keyboardKeys[255];
		ButtonEvent m_mouseButtons[3];

		class Timer {
		public:
			Timer();
			~Timer();
			void start();
			void stop();
			double getElapsedTime();
		private:
			double startTimeInMicroSec;
			double endTimeInMicroSec;
			char stopped;

			LARGE_INTEGER frequency;
			LARGE_INTEGER startCount;
			LARGE_INTEGER endCount;
		} m_timer;
	};

#endif //INPUT_H
