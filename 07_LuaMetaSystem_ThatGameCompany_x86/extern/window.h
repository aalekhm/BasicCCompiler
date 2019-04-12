#ifndef WINDOW_H
#define WINDOW_H

#include "extern/input.cpp"
#include "extern/gl.h"


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	enum WindowOptions{
		RGB16 = 1 << 0,
		RGB24 = 1 << 1,
		RGB32 = 1 << 2,
		RGB64 = 1 << 3,

		ALPHA4 = 1 << 4,
		ALPHA8 = 1 << 5,
		ALPHA12 = 1 << 6,
		ALPHA16 = 1 << 7,

		STENCIL4 = 1 << 8,
		STENCIL8 = 1 << 9,
		STENCIL16 = 1 << 10,

		DEPTH8 = 1 << 11,
		DEPTH16 = 1 << 12,
		DEPTH24 = 1 << 13,
		DEPTH32 = 1 << 14,

		MULTISAMPLE2 = 1 << 15,
		MULTISAMPLE4 = 1 << 16,
		MULTISAMPLE8 = 1 << 17,
		MULTISAMPLE16 = 1 << 18,
		MULTISAMPLE32 = 1 << 19,

		FRAME_SYNC = 1 << 20,
        FULLSCREEN = 1 << 21,
		ADAPTIVE_FRAME_SYNC = 1 << 22,
		STEREO = 1 << 23,
		DOUBLE_BUFFER = 1 << 24,
        DEBUG_CONTEXT = 1 << 25,
		CORE_PROFILE = 1 << 26
	};
	
	class Window{
	public:
		Window(const char* _name, int32_t _width, int32_t _height, int32_t _xpos, int32_t _ypos, uint32_t _options);
		Window() : m_inputHandler(new InputHandler()), m_initialized(false), m_active(false) {}

		~Window();

		virtual bool processAllMessages() const;
		virtual void swapBuffers() const { SwapBuffers(m_hdc); }

		virtual void windowResolution(int& _width, int& _height) const;
		virtual void desktopResolution(int& _width, int& _height) const;

		virtual bool isFullscreen() const;
		virtual bool setFullscreen(bool _fullscreen);
		virtual bool isMinimized() const { return IsIconic(m_hwnd); };
		virtual bool setMinimized(bool _minimized) { return ShowWindow(m_hwnd, (_minimized ? SW_MINIMIZE : SW_SHOW)); };

		bool initialized() const { return m_initialized; }
		bool active() const { return m_active; }
		void deactivate() { m_active = false; }

		InputHandler::ButtonState getMouseButton(InputHandler::MouseButton _button) const { return m_inputHandler->queryMouseButton(_button); }
		InputHandler::ButtonState getKey(char _character) const { return m_inputHandler->queryKeyboardButton(_character); };
		/**
		* Returns the Mouse Position in OpenGL Viewport Coordinates
		*/
		void getMousePos(float& _mousePosX, float& _mousePosY) const {
			int mw, mh;
			m_inputHandler->queryMousePosition(mw, mh);
			int w, h;
			windowResolution(w, h);
			_mousePosX = static_cast<float>(mw) / static_cast<float>(w);
			_mousePosY = 1.0f - static_cast<float>(mh) / static_cast<float>(h);
		};

		double elapsedTime() const{ return m_inputHandler->elapsedTime(); }

		InputHandler* inputHandler(){ return m_inputHandler; }
		const InputHandler* inputHandler() const { return m_inputHandler; }

	private:
		InputHandler* m_inputHandler;

		HDC m_hdc;
		HWND m_hwnd;
		WINDOWPLACEMENT m_windowPlacement;

		bool m_initialized;
		bool m_active;

		bool createWindow(TCHAR* _caption, TCHAR* _classname, ULONG _style, int32_t _x, int32_t _y, int32_t _width, int32_t _height);

		bool createOpenGLWindow(TCHAR* _name, int32_t _width, int32_t _height, int32_t _xpos, int32_t _ypos, uint32_t _options);
	};
	



#endif WINDOW_H
