#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768
#define NOT !

#include <windows.h>					// Header File For Windows
#include "Engine/EngineManager.h"
#include "wglext.h"

HDC			m_pHDC		= NULL;			// Private GDI Device Context
HGLRC		m_pHRC		= NULL;			// Permanent Rendering Context
HWND		m_pHWnd		= NULL;			// Holds Our Window Handle
HINSTANCE	m_pHInstance;				// Holds The Instance Of The Application

bool		m_bActive = false;			// Window Active Flag
bool		m_bFULLSCREEN = true;		// Fullscreen Flag Set To TRUE By Default

//////////////////////////////////////////////////////////
EngineManager* m_pEngineManager;
//////////////////////////////////////////////////////////

unsigned short getChar(WPARAM wParam, LPARAM lParam) 
{
	BYTE* kbs = new BYTE[255];
	GetKeyboardState(kbs);

	UINT nScanCode = (lParam>>16) & 0xff;
	WORD ch = 0;
	ToAscii(wParam, nScanCode, kbs, &ch, 0);

	return ch;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) 
	{
		case WM_ACTIVATE:
		{
			if(!HIWORD(wParam))
				m_bActive = true;
			else
				m_bActive = false;

			return 0;
		}
		case WM_SYSCOMMAND:
		{
			switch(wParam) 
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
		}
		break;
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN:
		{
			m_pEngineManager->keyPressed(wParam, getChar(wParam, lParam));
			return 0;
		}
		case WM_KEYUP:
		{
			m_pEngineManager->keyReleased(wParam, getChar(wParam, lParam));
			return 0;
		}
		case WM_SIZE:
		{
			// set viewport

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			m_pEngineManager->onMouseDown(MK_LBUTTON, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_LBUTTONUP:
		{
			m_pEngineManager->onMouseUp(MK_LBUTTON, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_MBUTTONDOWN:
		{
			m_pEngineManager->onMouseDown(MK_MBUTTON, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_MBUTTONUP:
		{
			m_pEngineManager->onMouseUp(MK_MBUTTON, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_RBUTTONDOWN:
		{
			m_pEngineManager->onMouseDown(MK_RBUTTON, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_RBUTTONUP:
		{
			m_pEngineManager->onMouseUp(MK_RBUTTON, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_MOUSEWHEEL:
		{
			m_pEngineManager->onMouseWheel(wParam, lParam);
		}
		break;
		case WM_MOUSEMOVE:
		{
			m_pEngineManager->onMouseMove((int)wParam, (int)LOWORD(lParam), (int)HIWORD(lParam));
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GLvoid killGLWindow() 
{
	if(m_bFULLSCREEN) 
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}


	if(m_pHRC) {
		if(NOT wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, "Release of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		if(!wglDeleteContext(m_pHRC))
			MessageBox(NULL, "Release of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		m_pHRC = NULL;
	}

	if(m_pHDC && NOT ReleaseDC(m_pHWnd, m_pHDC))
	{
		MessageBox(NULL, "Release of Device Context failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_pHDC = NULL;
	}

	if(m_pHWnd && NOT DestroyWindow(m_pHWnd))
	{
		MessageBox(NULL, "Release of mHWnd failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_pHWnd = NULL;
	}

	if(NOT UnregisterClass("OpenGL", m_pHInstance))
	{
		MessageBox(NULL, "UnRegisterClass failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_pHInstance = NULL;
	}
}

bool QueryExtensions()
{
	typedef BOOL(WINAPI *FPCPFARB)(HDC, const int32_t*, const float*, uint32_t, int32_t*, uint32_t*);
	typedef BOOL(WINAPI *FPGPFARB)(HDC, int32_t, int32_t, uint32_t, const int32_t*, int32_t*);

	FPCPFARB wglChoosePixelFormatARB = (FPCPFARB)wglGetProcAddress("wglChoosePixelFormatARB");
	FPGPFARB wglGetPixelFormatAttribivARB = (FPGPFARB)wglGetProcAddress("wglGetPixelFormatAttribivARB");

	int multisample = 16;
	auto btst = [](uint32_t _reg, uint32_t _mask) { return (_reg & _mask) != 0; };
	int32_t pixelFormatAttribList[] = 
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_STEREO_ARB, 0,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, 1,
		WGL_SAMPLES_ARB, multisample,
		0, // End
	};

	const int32_t pixelFormatAttribListSize = sizeof(pixelFormatAttribList) / sizeof(int32_t);
	int32_t pixelFormats[255];
	UINT numFormats = 0;

	wglChoosePixelFormatARB(m_pHDC, pixelFormatAttribList, nullptr, 255, &pixelFormats[0], &numFormats);
	while (numFormats <= 0 && multisample > 0) 
	{
		multisample >>= 1;
		pixelFormatAttribList[pixelFormatAttribListSize - 2] = 8;
		pixelFormatAttribList[pixelFormatAttribListSize - 4] = (multisample > 0 ? 1 : 0);
		wglChoosePixelFormatARB(m_pHDC, pixelFormatAttribList, nullptr, 255, &pixelFormats[0], &numFormats);
	}

	if (numFormats <= 0) 
	{
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	if (NOT SetPixelFormat(m_pHDC, pixelFormats[0], &pfd) <= 0)
	{
		return false;
	}

	return true;
}

bool createGLWindow(char* title, int width, int height, int bits, bool isFullScreen) 
{
	int			iPixelFormat;
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		windowRect;

	windowRect.left		= (long)0;
	windowRect.right	= (long)width;
	windowRect.top		= (long)0;
	windowRect.bottom	= (long)height;

	m_bFULLSCREEN = isFullScreen;

	m_pHInstance		= GetModuleHandle(NULL);
	
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hInstance		= m_pHInstance;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.lpszClassName	= "OpenGL";
	wc.lpszMenuName		= NULL;
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	
	if(NOT RegisterClass(&wc)) 
	{
		MessageBox(NULL, "Failed to Register the Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(m_bFULLSCREEN) 
	{
		DEVMODE	dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= width;
		dmScreenSettings.dmPelsHeight	= height;
		dmScreenSettings.dmBitsPerPel	= bits;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) 
		{
			if(MessageBox(NULL, "The requested FullScreen Mode is not supported\n by your Video Card. Use Windowed Mode instead?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				m_bFULLSCREEN = false;
			else 
			{
				MessageBox(NULL, "Program will now close.", "ERROR", MB_OK | MB_ICONSTOP);
				return false;
			}
		}
	}

	if(m_bFULLSCREEN) 
	{
		dwExStyle	= WS_EX_APPWINDOW;
		dwStyle		= WS_POPUP;
		ShowCursor(false);
	}
	else 
	{
		dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle		= WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);

	if(!(m_pHWnd = CreateWindowEx(	dwExStyle,
									"OpenGL",
									title,
									dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
									0, 0,
									windowRect.right - windowRect.left,
									windowRect.bottom - windowRect.top,
									NULL,
									NULL,
									m_pHInstance,
									NULL)
	)) {
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(NOT(m_pHDC = GetDC(m_pHWnd))) 
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Unable to get Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if(NOT(iPixelFormat = ChoosePixelFormat(m_pHDC, &pfd)))
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Unable to choose Pixel format.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(NOT SetPixelFormat(m_pHDC, 57/*iPixelFormat*/, &pfd)) 
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Unable to set Pixel format.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(NOT(m_pHRC = wglCreateContext(m_pHDC))) 
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Unable to create wgl context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(NOT wglMakeCurrent(m_pHDC, m_pHRC)) 
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Unable to make wgl context","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	//if (NOT QueryExtensions())
	//{
	//	wglDeleteContext(m_pHRC);
	//	killGLWindow();								// Reset The Display
	//	MessageBox(NULL, "Unable to init glewinit", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	//	return false;
	//}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) 
	{
		wglDeleteContext(m_pHRC);
		killGLWindow();								// Reset The Display
		MessageBox(NULL,"Unable to init glewinit","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(m_pHWnd, true);
	SetForegroundWindow(m_pHWnd);
	SetFocus(m_pHWnd);


	return true;
}

int winMainLoop() 
{
	//if(MessageBox(NULL, "Would you like to run in FULLSCREEN mode?", "Start FullScreen", MB_YESNO) == IDNO)
		m_bFULLSCREEN = false;

	if(NOT createGLWindow("Dream3D!!!", SCREEN_WIDTH, SCREEN_HEIGHT, 16, m_bFULLSCREEN))
		return 0;

	m_pEngineManager = EngineManager::getInstance();
	m_pEngineManager->startup(m_pHWnd);
	
	MSG		msg;
	bool	done = false;

	while(true) 
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT) 
			{
				m_pEngineManager->exit();
				return msg.wParam;
			}
		}
		else 
		{
			m_pEngineManager->frame();
			SwapBuffers(m_pHDC);

			if(m_pEngineManager->getState() == EngineManager::UNINITIALIZED)
				break;

			if(EngineManager::isKeyPressed(VK_F1)) 
			{
				m_pEngineManager->keyPressed(0, VK_F1);
				killGLWindow();
				m_bFULLSCREEN = !m_bFULLSCREEN;
				if(NOT createGLWindow("Dream3D!!!", SCREEN_WIDTH, SCREEN_HEIGHT, 16, m_bFULLSCREEN)) 
				{
					return 0;
				}
			}
		}
	}

	return 0;
}

extern "C" int APIENTRY WinMain(	HINSTANCE	hInstance, 
									HINSTANCE	hPrevInstance,
									LPSTR		lpCmdLine,
									int			nCmdShow
) {
	if(winMainLoop() == 0)
		killGLWindow();

	return 0;
}